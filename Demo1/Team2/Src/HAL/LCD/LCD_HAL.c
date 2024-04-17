/*
 * LCD.c
 *
 *  Created on: Mar 22, 2024
 *      Author: Osman
 */


/* ============================================================================ */
/*                                  INCLUDEDS                           	    */
/* ============================================================================ */

#include "MCAL/GPIO/GPIO.h"
#include "HAL/LCD/LCD_priv.h"
#include "HAL/LCD/LCD_HAL.h"
#include "HAL/LCD/LCD_CFG.h"

#include <stdint.h>
/* ============================================================================ */
/*                                   MACROS                             	    */
/* ============================================================================ */

/* helping macros to concat the LCD configurations into their macros */
#define CONCAT_H(d7,d6,d5,d4,d3,d2,d1,d0)       0b##d7##d6##d5##d4##d3##d2##d1##d0
#define CONCAT(d7,d6,d5,d4,d3,d2,d1,d0)         CONCAT_H(d7,d6,d5,d4,d3,d2,d1,d0)


#define LCD_DISPLAY_CONTROL     CONCAT(0,0,0,0,1,LCD_DISPLAY,LCD_CURSER,LCD_BLINKING)
#define LCD_ENTRY_MODE          CONCAT(0,0,0,0,0,1,LCD_INCREMENT_DECREMENT,LCD_SHIFTING)

#define LCD_CLEAR_DISPLAY               0x01    /* Clear display command */
#define LCD_SET_DDR_ADDRESS            0X80    /* Set pos of DDR command */

#if LCD_MODE == LCD_8_BIT_MODE
#define FUNCTION_SET        CONCAT(0,0,1,1,LCD_LINES_NUMBER,LCD_FORMAT,0,0)
#define LCD_PINS_NUM        8

#elif LCD_MODE == LCD_4_BIT_MODE
#define FUNCTION_SET        CONCAT(0,0,1,0,LCD_LINES_NUMBER,LCD_FORMAT,0,0)
#define LCD_PINS_NUM        4

#endif

#define ENABLE_PIN_LOW      0U
#define ENABLE_PIN_HIGH     1U


#define HIGH_NIBBLE     4U
#define LOW_NIBBLE      0U

#if LCD_MODE == LCD_8_BIT_MODE
#define LATCH_TARGET_COUNT    2U        /* Number of times latch needs to be called */
#elif LCD_MODE == LCD_4_BIT_MODE
#define LATCH_TARGET_COUNT    4U        /* Number of times latch needs to be called */
#endif

#ifndef NULL
#define NULL ((void* )0)
#endif


/* ============================================================================ */
/*                                  	TYPES                           	    */
/* ============================================================================ */

typedef void (*LCDCallBack)(void);

/* stores the state of a user request.
    Available types are:
        - WRITE_STRING
        - CLEAR_SCREEN
        - SET_POS 
    If state is busy, that means a process is currently being handled and
    can't accept new requests*/
typedef struct
{
    uint8_t type;
    uint8_t state;
    LCDCallBack cb;
    const uint8_t * data;
    uint8_t length;
    uint8_t currIndex;
    uint8_t row;
    uint8_t col;
    uint8_t command;
}LCD_Request_t;



/* States of the LCD */
typedef enum {LCD_OFF, LCD_INIT, LCD_OPERATION}LCD_State_t;


/* Init Process states */
#if LCD_MODE == LCD_8_BIT_MODE
typedef enum {
    POWER_ON_STATE,
    FUNCTIONAL_SET_STATE,
    DISPLAY_CTRL_STATE,
    CLEAR_DISPLAY_STATE,
    ENTRY_MODE_STATE,
    END_STATE
}LCD_Init_States_t;

#elif LCD_MODE == LCD_4_BIT_MODE
typedef enum {
    POWER_ON_STATE,
    PRE_FUNCTIONAL_SET_STATE,
    FUNCTIONAL_SET_STATE,
    DISPLAY_CTRL_STATE,
    CLEAR_DISPLAY_STATE,
    ENTRY_MODE_STATE,
    END_STATE
}LCD_Init_States_t;
#endif

/* Request states */
typedef enum
{
    AVAILABLE,
    BUSY,
}LDC_RequestStates_t;

/* User Request types */
typedef enum
{
    NONE,
    WRITE_STRING,
    WRITE_BUFFER,
    CLEAR_SCREEN,
    SET_CURSOR_POS,
    SEND_COMMAND,
}LCD_RequestTypes_t;

/* ============================================================================ */
/*                                  	EXTERNS                           	    */
/* ============================================================================ */

/* LCD Pins array */
extern const LCD_Pin_t LCD_Pins[LCD_PINS_NUM];

/* ============================================================================ */
/*                                  GLOBAL VARIABLES                    	    */
/* ============================================================================ */


static LCD_State_t G_LCDState = LCD_OFF;        /* States of LCD */

static LCD_Request_t Requests[REQUEST_BUFFER_SIZE];     /* Buffer of requests */
static uint8_t currReqIdx;

static uint8_t G_OperationLatchCount;       /* counter used by operation requests to check if number of latches required is reached */



/* ============================================================================ */
/*                                  STATIC FUNCTIONS                           	*/
/* ============================================================================ */

/**
 * @brief Hanldes the writing of LCD pins, also enabling and disabling the latch.
 * It also handles the case of 8-bit mode and 4-bit mode
 * 
 * @param byte The byte to be written on LCD pins
 */
static void Latch(uint8_t byte)
{
    #if LCD_MODE == LCD_4_BIT_MODE
    static uint8_t currNibble = HIGH_NIBBLE;
    #endif

    static uint8_t EnablePinState = ENABLE_PIN_LOW;      /* State of the Enable Pin */
    uint8_t iter;

    if(EnablePinState == ENABLE_PIN_LOW)
    {
        for(iter = 0; iter < LCD_PINS_NUM; iter++)
        {
            /* set the lcd pin state with the value corresponding to the bit in the sent byte */

            #if LCD_MODE == LCD_8_BIT_MODE
            GPIO_SetPinState( LCD_Pins[iter].Port, LCD_Pins[iter].Pin, (byte >> iter)&1 );
            #elif LCD_MODE == LCD_4_BIT_MODE
            GPIO_SetPinState( LCD_Pins[iter].Port, LCD_Pins[iter].Pin, (byte >> (iter+currNibble))&1 );
            #endif

        }

        EnablePinState = ENABLE_PIN_HIGH;
    }
    else
    {
        EnablePinState = ENABLE_PIN_LOW;

        #if LCD_MODE == LCD_4_BIT_MODE
        /* Change the Nibble to be latched to the opposite Nibble
            Since in 4-bit mode, the high nibble is handled first then
            low nibble, then each time a nibble is latched we need to change
            the nibble to be latched to the opposite nibble */
        currNibble = (currNibble == HIGH_NIBBLE)? LOW_NIBBLE : HIGH_NIBBLE;
        #endif
    }
    GPIO_SetPinState(LCD_PORT_E, LCD_PIN_E, EnablePinState);
}


static void WriteCommand(uint8_t command)
{
    GPIO_SetPinState(LCD_PORT_RS, LCD_PIN_RS, 0);   
    GPIO_SetPinState(LCD_PORT_RW, LCD_PIN_RW, 0);   
    Latch(command);
}


static void WriteData(uint8_t data)
{

    GPIO_SetPinState(LCD_PORT_RS, LCD_PIN_RS, 1);   
    GPIO_SetPinState(LCD_PORT_RW, LCD_PIN_RW, 0); 

    Latch(data);
}


#if LCD_MODE == LCD_8_BIT_MODE

static void InitProcess(void)
{
    static LCD_Init_States_t InitState = POWER_ON_STATE;
    static uint8_t counter = 0;

    switch(InitState)
    {
        case POWER_ON_STATE:
            /* Delay for 30 ms */
            counter += LCD_UPDATE_PERIODICITY_MS;

            if(counter >= 30)
            {
                /* Move to next state if dealy reaches 30ms */
                counter = 0;
                InitState = FUNCTIONAL_SET_STATE;
            }
            break;

        case FUNCTIONAL_SET_STATE:
            counter++;
            if(counter == LATCH_TARGET_COUNT)
            {
                counter = 0;
                InitState = DISPLAY_CTRL_STATE;
            }
            WriteCommand(FUNCTION_SET);
            break;

        case DISPLAY_CTRL_STATE:
            counter++;
            if(counter == LATCH_TARGET_COUNT)
            {
                counter = 0;
                InitState = CLEAR_DISPLAY_STATE;
            }
            WriteCommand(LCD_DISPLAY_CONTROL);
            break;

        case CLEAR_DISPLAY_STATE:
            counter++;
            if(counter == LATCH_TARGET_COUNT)
            {
                counter = 0;
                InitState = ENTRY_MODE_STATE;
            }
            WriteCommand(LCD_CLEAR_DISPLAY);
            break;
                    
        case ENTRY_MODE_STATE:
            counter++;
            if(counter == LATCH_TARGET_COUNT)
            {
                counter = 0;
                InitState = END_STATE;
            }
            WriteCommand(LCD_ENTRY_MODE);
            break;

        case END_STATE:
            G_LCDState = LCD_OPERATION;
            break;
        
        default:
            break;
    }
}

#elif LCD_MODE == LCD_4_BIT_MODE

static void InitProcess(void)
{
    static LCD_Init_States_t InitState = POWER_ON_STATE;
    static uint8_t counter = 0;

    switch(InitState)
    {
        case POWER_ON_STATE:
            /* Delay for 30 ms */
            counter += LCD_UPDATE_PERIODICITY_MS;

            if(counter >= 30)
            {
                /* Move to next state if dealy reaches 30ms */
                counter = 0;
                InitState = PRE_FUNCTIONAL_SET_STATE;
            }
            break;

        /* The first latch in the Functional Set step as it needs to be done manually */
        case PRE_FUNCTIONAL_SET_STATE:
            counter++;
            if(counter == 1)
            {
                GPIO_SetPinState(LCD_Pins[0].Port, LCD_Pins[0].Pin, GPIO_STATE_LOW);
                GPIO_SetPinState(LCD_Pins[1].Port, LCD_Pins[1].Pin, GPIO_STATE_HIGH);
                GPIO_SetPinState(LCD_Pins[2].Port, LCD_Pins[2].Pin, GPIO_STATE_LOW);
                GPIO_SetPinState(LCD_Pins[3].Port, LCD_Pins[3].Pin, GPIO_STATE_LOW);

                GPIO_SetPinState(LCD_PORT_E, LCD_PIN_E, GPIO_STATE_HIGH);

            }
            else if(counter == 2)
            {
                GPIO_SetPinState(LCD_PORT_E, LCD_PIN_E, GPIO_STATE_LOW);

                counter = 0;
                InitState = FUNCTIONAL_SET_STATE;
            }
            break;

        case FUNCTIONAL_SET_STATE:
            counter++;
            if(counter == LATCH_TARGET_COUNT)
            {
                counter = 0;
                InitState = DISPLAY_CTRL_STATE;
            }
            WriteCommand(FUNCTION_SET);
            break;

        case DISPLAY_CTRL_STATE:
            counter++;
            if(counter == LATCH_TARGET_COUNT)
            {
                counter = 0;
                InitState = CLEAR_DISPLAY_STATE;
            }
            WriteCommand(LCD_DISPLAY_CONTROL);
            break;

        case CLEAR_DISPLAY_STATE:
            counter++;
            if(counter == LATCH_TARGET_COUNT)
            {
                counter = 0;
                InitState = ENTRY_MODE_STATE;
            }
            WriteCommand(LCD_CLEAR_DISPLAY);
            break;
                    
        case ENTRY_MODE_STATE:
            counter++;
            if(counter == LATCH_TARGET_COUNT)
            {
                counter = 0;
                InitState = END_STATE;
            }
            WriteCommand(LCD_ENTRY_MODE);
            break;

        case END_STATE:
            G_LCDState = LCD_OPERATION;
            break;
        
        default:
            break;
    }
}


#endif


/* Iterates through the string and prints each character */
static void WriteString(void)
{

    if(Requests[currReqIdx].data[Requests[currReqIdx].currIndex] != '\0')
    {
        WriteData(Requests[currReqIdx].data[Requests[currReqIdx].currIndex]);
        G_OperationLatchCount++;

        if(G_OperationLatchCount == LATCH_TARGET_COUNT)
        {
            Requests[currReqIdx].currIndex++;
            G_OperationLatchCount = 0;

        }     
    }
    else
    {
        Requests[currReqIdx].state = AVAILABLE;
        Requests[currReqIdx].type = NONE;

    }
}


static void WriteBuffer(void)
{

    if(Requests[currReqIdx].currIndex < Requests[currReqIdx].length)
    {
        WriteData(Requests[currReqIdx].data[Requests[currReqIdx].currIndex]);
        G_OperationLatchCount++;

        if(G_OperationLatchCount == LATCH_TARGET_COUNT)
        {
            Requests[currReqIdx].currIndex++;
            G_OperationLatchCount = 0;

        }     
    }
    else
    {
        Requests[currReqIdx].state = AVAILABLE;
        Requests[currReqIdx].type = NONE;

    }
}


static void ClearScreen(void)
{
    if(G_OperationLatchCount == LATCH_TARGET_COUNT)
    {
        G_OperationLatchCount = 0;

        Requests[currReqIdx].state = AVAILABLE;
        Requests[currReqIdx].type = NONE;
    }
    else
    {
        WriteCommand(LCD_CLEAR_DISPLAY);
        G_OperationLatchCount++;
    }

}

static void SetCursorPos(void)
{

    if(G_OperationLatchCount == LATCH_TARGET_COUNT)
    {
        G_OperationLatchCount = 0;

        Requests[currReqIdx].state = AVAILABLE;
        Requests[currReqIdx].type = NONE;
    }
    else
    {
        if(Requests[currReqIdx].row == 0)
        {
            WriteCommand(LCD_SET_DDR_ADDRESS + Requests[currReqIdx].col);
        }
        else if (Requests[currReqIdx].row == 1)
        {
            WriteCommand(LCD_SET_DDR_ADDRESS + (Requests[currReqIdx].col + 0x40) );
        }

        G_OperationLatchCount++;

    }

}

static void SendCommand(void)
{
    if(G_OperationLatchCount == LATCH_TARGET_COUNT)
    {
        G_OperationLatchCount = 0;

        Requests[currReqIdx].state = AVAILABLE;
        Requests[currReqIdx].type = NONE;
    }
    else
    {  
        WriteCommand(Requests[currReqIdx].command);
        G_OperationLatchCount++;
    }
}


/* If the LCD finished Initialization and is in Operation mode */

static void OperationProcess(void)
{
    if(Requests[currReqIdx].state == BUSY)
    {
        if(Requests[currReqIdx].type == WRITE_STRING)
        {
            WriteString();
        }
        else if(Requests[currReqIdx].type == WRITE_BUFFER)
        {
            WriteBuffer();
        }
        else if(Requests[currReqIdx].type == CLEAR_SCREEN)
        {
            ClearScreen();
        }
        else if(Requests[currReqIdx].type == SET_CURSOR_POS)
        {
            SetCursorPos();
        }
        else if(Requests[currReqIdx].type == SEND_COMMAND)
        {
            SendCommand();
        }
    }
    else
    {
		currReqIdx++;

		if(currReqIdx == REQUEST_BUFFER_SIZE)
		{
			currReqIdx = 0;
		}

        /* if no request after the previous request that means there are no more
        buffered requests so reset the index back to 0 */
		if(Requests[currReqIdx].state == AVAILABLE)
		{
			currReqIdx = 0;
		}
    }
}

/* ============================================================================ */
/*                                 IMPLEMENTATION                           	*/
/* ============================================================================ */

/* The runnable of LCD */
void LCD_Update(void)
{
    if(G_LCDState == LCD_INIT)
    {
        InitProcess();
    }
    else if(G_LCDState == LCD_OPERATION)
    {
        OperationProcess();
    }
    else
    {
        /* do nothing */
    }
}


LCD_ErrorStatus_t LCD_Init(void)
{
    LCD_ErrorStatus_t Ret_ErrorStatus = LCD_OK;
    uint8_t iter;
    GPIO_Pin_t lcdPin;
    lcdPin.Mode = GPIO_MODE_OP_PP;
    lcdPin.AF = GPIO_AF_SYSTEM;
    lcdPin.Speed = GPIO_SPEED_HIGH;

    /* Initialize the LCD pins */
    for(iter = 0; iter < LCD_PINS_NUM; iter++)
    {
        lcdPin.Pin = LCD_Pins[iter].Pin;
        lcdPin.Port = LCD_Pins[iter].Port;

        GPIO_Init(&lcdPin);
    }

    lcdPin.Pin = LCD_PIN_RS;
    lcdPin.Port = LCD_PORT_RS;
    GPIO_Init(&lcdPin);

    lcdPin.Pin = LCD_PIN_RW;
    lcdPin.Port = LCD_PORT_RW;
    GPIO_Init(&lcdPin);

    lcdPin.Pin = LCD_PIN_E;
    lcdPin.Port = LCD_PORT_E;
    GPIO_Init(&lcdPin);


    /* Change LCD state from OFF to Init */
    G_LCDState = LCD_INIT;

    return Ret_ErrorStatus;
}


LCD_ErrorStatus_t LCD_WriteStringAsync(const uint8_t * string)
{
    LCD_ErrorStatus_t Ret_ErrorStatus = LCD_OK;
    uint8_t iter;
    if(string == NULL)
    {
        Ret_ErrorStatus = LCD_NOK;
    }
    else
    {
        for(iter = 0; iter < REQUEST_BUFFER_SIZE; iter++)
        {
            if(Requests[iter].state == AVAILABLE)
            {
              Requests[iter].state = BUSY;
              Requests[iter].type = WRITE_STRING;
              Requests[iter].data = string;
              Requests[iter].currIndex = 0;
              break;
            }
        }
    }

    return Ret_ErrorStatus;
}


LCD_ErrorStatus_t LCD_WriteBufferAsync(const uint8_t * data, uint16_t size)
{
    LCD_ErrorStatus_t Ret_ErrorStatus = LCD_OK;

    uint8_t iter;

    if(data == NULL)
    {
        Ret_ErrorStatus = LCD_NOK;
    }
    else
    {
        for(iter = 0; iter < REQUEST_BUFFER_SIZE; iter++)
        {
            if(Requests[iter].state == AVAILABLE)
            {
              Requests[iter].state = BUSY;
              Requests[iter].type = WRITE_BUFFER;

              Requests[iter].data = data;
              Requests[iter].length = size;
              Requests[iter].currIndex = 0;
              break;
            }
        }
    }

    return Ret_ErrorStatus;
}


LCD_ErrorStatus_t LCD_ClearScreenAsync(void)
{
    uint8_t iter;

    for(iter = 0; iter < REQUEST_BUFFER_SIZE; iter++)
    {
        if(Requests[iter].state == AVAILABLE)
        {
            Requests[iter].state = BUSY;
            Requests[iter].type = CLEAR_SCREEN;
            break;
        }
    }
    
    return 0;
}


LCD_ErrorStatus_t LCD_SetCursorPosAsync(uint8_t row, uint8_t col)
{
    uint8_t iter;

    for(iter = 0; iter < REQUEST_BUFFER_SIZE; iter++)
    {
        if(Requests[iter].state == AVAILABLE)
        {
            Requests[iter].state = BUSY;
            Requests[iter].type = SET_CURSOR_POS;

            Requests[iter].row = row;
            Requests[iter].col = col;

            break;
        }
    }

    return 0;
}


LCD_ErrorStatus_t LCD_SendCommandAsync(uint8_t command)
{
     uint8_t iter;

    for(iter = 0; iter < REQUEST_BUFFER_SIZE; iter++)
    {
        if(Requests[iter].state == AVAILABLE)
        {
            Requests[iter].state = BUSY;
            Requests[iter].type = SEND_COMMAND;
            Requests[iter].command = command;
            break;
        }
    }
    return 0;
}
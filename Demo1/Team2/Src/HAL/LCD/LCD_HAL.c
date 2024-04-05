/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "LCD_HAL.h"
#include "GPIO.h"
#include "RCC.h"


/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#ifndef NULL
#define NULL (void *)0
#endif

#define RS 0
#define RW 1
#define EN 2

#define REQ_TYPE_WRITE   0
#define REQ_TYPE_CLEAR   1
#define REQ_TYPE_SET_POS 2
#define REQ_TYPE_NOREQ   3

#define REQ_STATE_BUSY  0
#define REQ_STATE_READY 1

#define LCD_INIT_STATE_POWER_ON                         0
#define LCD_INIT_STATE_FUNCTIONAL_SET_1                 1
#define LCD_INIT_STATE_FUNCTIONAL_SET_2                 2
#define LCD_INIT_STATE_FUNCTIONAL_SET_3                 3
#define LCD_INIT_STATE_DISPLAY_ON_OFF_CONTROL_1         4
#define LCD_INIT_STATE_DISPLAY_ON_OFF_CONTROL_2         5
#define LCD_INIT_STATE_DISPLAY_CLEAR_1                  6
#define LCD_INIT_STATE_DISPLAY_CLEAR_2                  7
#define LCD_INIT_STATE_ENTRY_MODE_SET_1                 8
#define LCD_INIT_STATE_ENTRY_MODE_SET_2                 9
#define LCD_INIT_STATE_END                              10

#define LCD_SendMOSTSIG                                 0
#define LCD_SENDLEASTSIG                                1
#define LCD_SENDENLOW                                   2

#define LCD_INPUT_COMMAND  GPIO_SET_PIN_LOW
#define LCD_INPUT_DATA     GPIO_SET_PIN_HIGH


#define FOUR_BIT_MODE 7
#define EIGHT_BIT_MODE 11



/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/


typedef struct{
    uint8_t* s;
    volatile uint8_t len;
    volatile uint8_t state;
    volatile uint8_t type;
    volatile uint8_t xpos;
    volatile uint8_t ypos;
}User_Req_t;

typedef struct{
    uint8_t cursorPos;
    CB_t CB; 
}write_t;


/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
volatile static uint8_t GLOBAL_LCD_STATE = INIT_STATE;
volatile static User_Req_t User_Req ={
    .s = NULL,
    .len = 0,
    .state = REQ_STATE_READY,
    .type = REQ_TYPE_NOREQ,
    .xpos = 0,
    .ypos = 0
};
volatile static write_t write_process = {
    .cursorPos = 0,
    .CB= NULL
};
extern const LCD_Pins_t LCD_Connection;
volatile static uint8_t LCD_INPUT_FINISHED = 0;
static volatile uint8_t Pos_Flag = 2;
/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/
static void LCD_writeProc(void);
static void LCD_clearProc(void);
static void LCD_setPosProc(void);
static void LCD_GPIO_init(void);
static void LCD_writeInitCommand (uint8_t command);
static void LCD_ActionEnableHighMostSig(uint8_t input, uint8_t type);
static void LCD_ActionEnableHighLeastSig(uint8_t input);
static void LCD_ActionEnableLow (void);
// static void LCD_writeCommand(uint8_t command);
// static void LCD_writeData(uint8_t data);
// static void LCD_write(uint8_t type, uint8_t input);
/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/


void LCD_TASK(void)
{
    if(GLOBAL_LCD_STATE == INIT_STATE)
    {
        LCD_init();
    }
    else if(GLOBAL_LCD_STATE == OPERATION_STATE)
    {
        if (User_Req.state == REQ_STATE_BUSY)
        {
            switch(User_Req.type)
            {
                case REQ_TYPE_WRITE:
                    LCD_writeProc();
                    break;
                case REQ_TYPE_CLEAR:
                    LCD_clearProc();
                    break;
                case REQ_TYPE_SET_POS:
                    LCD_setPosProc();
                    break;
                case REQ_TYPE_NOREQ:
                    break;
                default:
                    break;
            }
        }
    }
    else if(GLOBAL_LCD_STATE == IDEL_STATE )
    {

    }
    else
    {

    }
}

LCD_errorStatus_t LCD_writeString( uint8_t* string, uint8_t length, uint8_t xpos, uint8_t ypos, CB_t CB)
{
    LCD_errorStatus_t LCD_errorStatus = LCD_NotOk;

    if(string == NULL)
    {
        LCD_errorStatus = LCD_NullPtr;
    }
    else if(length <= 0)
    {
        LCD_errorStatus = LCD_WrongLength;
    }
    else if(xpos < 0)
    {
        LCD_errorStatus = LCD_WrongXpos;
    }
    else if(ypos < 0)
    {
        LCD_errorStatus = LCD_WrongYpos;
    }
    else if(CB == NULL)
    {
        LCD_errorStatus = LCD_NullPtr;        
    }
    else
    {
        LCD_errorStatus = LCD_Ok;

        if(GLOBAL_LCD_STATE == OPERATION_STATE && User_Req.state == REQ_STATE_READY)
        {
            User_Req.state = REQ_STATE_BUSY;
            User_Req.s = string;
            User_Req.len = length;
            User_Req.type = REQ_TYPE_WRITE;
            User_Req.xpos = xpos;
            User_Req.ypos = ypos;
            write_process.CB = CB;
            write_process.cursorPos = 0;
            
        }
    }

    return LCD_errorStatus;
}

LCD_errorStatus_t LCD_getState(uint8_t* LCD_state)
{
    LCD_errorStatus_t LCD_errorStatus = LCD_NotOk;

    if(LCD_state == NULL)
    {
        LCD_errorStatus = LCD_NullPtr;
    }
    else
    {
        LCD_errorStatus = LCD_Ok;
        *LCD_state = GLOBAL_LCD_STATE;
    }
    return LCD_errorStatus;
}

LCD_errorStatus_t LCD_clearScreen(CB_t CB)
{
    LCD_errorStatus_t LCD_errorStatus = LCD_NotOk;
    
    if(CB == NULL)
    {
        LCD_errorStatus = LCD_NullPtr;
    }
    else
    {
        LCD_errorStatus = LCD_Ok;
        if(GLOBAL_LCD_STATE == OPERATION_STATE && User_Req.state == REQ_STATE_READY)
        {
            User_Req.state = REQ_STATE_BUSY;
            User_Req.type = REQ_TYPE_CLEAR;
            write_process.CB = CB;
        }
    }
    return LCD_errorStatus;
}

LCD_errorStatus_t LCD_setCursorPosition(uint8_t xpos , uint8_t ypos, CB_t CB)
{
    LCD_errorStatus_t LCD_errorStatus = LCD_NotOk;

    if(xpos <= 0)
    {
        LCD_errorStatus = LCD_WrongXpos;
    }
    else if(ypos <= 0)
    {
        LCD_errorStatus = LCD_WrongYpos;
    }
    else if(CB == NULL)
    {
        LCD_errorStatus = LCD_NullPtr;
    }
    else
    {
        LCD_errorStatus = LCD_Ok;
        if(GLOBAL_LCD_STATE == OPERATION_STATE && User_Req.state == REQ_STATE_READY)
        {
            User_Req.state = REQ_STATE_BUSY;
            User_Req.xpos = xpos;
            User_Req.ypos = ypos;
            User_Req.type = REQ_TYPE_SET_POS;
            write_process.cursorPos = 0;
            write_process.CB = CB;
        }
    }
    return LCD_errorStatus;
}

void LCD_init(void)
{
    static uint8_t LCD_init_state = LCD_INIT_STATE_POWER_ON;
    static uint8_t time = 0;
    time++;
    static uint8_t init_flag = 0;
    switch (LCD_init_state)
    {
        
    case LCD_INIT_STATE_POWER_ON:
        if(init_flag == 0)
        {
            LCD_GPIO_init();
            init_flag = 1;
        }       
        if(time == 50)
        {
            LCD_init_state = LCD_INIT_STATE_FUNCTIONAL_SET_1;
        }
        break;

    case LCD_INIT_STATE_FUNCTIONAL_SET_1:
        LCD_writeInitCommand(FUNCTION_SET_1);
        if(time == 53)
        {
            LCD_init_state = LCD_INIT_STATE_FUNCTIONAL_SET_2;
        }
        break;
    case LCD_INIT_STATE_FUNCTIONAL_SET_2:
        LCD_writeInitCommand(FUNCTION_SET_2);
        if(time == 56)
        {
            LCD_init_state = LCD_INIT_STATE_FUNCTIONAL_SET_3;
        }
        break;
    case LCD_INIT_STATE_FUNCTIONAL_SET_3:
        LCD_writeInitCommand(FUNCTION_SET_3);
        if(time == 59)
        {
            LCD_init_state = LCD_INIT_STATE_DISPLAY_ON_OFF_CONTROL_1;
        }
        break; 

    case LCD_INIT_STATE_DISPLAY_ON_OFF_CONTROL_1:
        LCD_writeInitCommand(DISPLAY_ON_OFF_CONTROL_1);
        if(time == 62)
        {
            LCD_init_state =LCD_INIT_STATE_DISPLAY_ON_OFF_CONTROL_2;
        }
        break;
    case LCD_INIT_STATE_DISPLAY_ON_OFF_CONTROL_2:
        LCD_writeInitCommand(DISPLAY_ON_OFF_CONTROL_2);
        if(time == 65)
        {
            LCD_init_state =LCD_INIT_STATE_DISPLAY_CLEAR_1;
        }
        break;
    case LCD_INIT_STATE_DISPLAY_CLEAR_1:
        LCD_writeInitCommand(DISPLAY_CLEAR_1);
        if(time == 68)
        {
            LCD_init_state =LCD_INIT_STATE_DISPLAY_CLEAR_2;
        }
        break;
    case LCD_INIT_STATE_DISPLAY_CLEAR_2:
        if (time <= 71)
        {
            LCD_writeInitCommand(DISPLAY_CLEAR_2);
        }  
        if(time == 72)
        {
            LCD_init_state =LCD_INIT_STATE_ENTRY_MODE_SET_1;
        }
        break;
    case LCD_INIT_STATE_ENTRY_MODE_SET_1:
        LCD_writeInitCommand(ENTRY_MODE_SET_1);
        if(time == 75)
        {
            LCD_init_state =LCD_INIT_STATE_ENTRY_MODE_SET_2;
        }
        break;
    case LCD_INIT_STATE_ENTRY_MODE_SET_2:
        LCD_writeInitCommand(ENTRY_MODE_SET_2);
        if(time == 78)
        {
            LCD_init_state = LCD_INIT_STATE_END;;
        }
        break;
    case LCD_INIT_STATE_END:
        GLOBAL_LCD_STATE = OPERATION_STATE;
        time = 0;
        break;

    default:
        break;
    }
    
}

void LCD_GPIO_init(void)
{
    GPIO_Pin_t Pin ;
    Pin.Mode = GPIO_OUT_PP;
    Pin.Speed = GPIO_SPEED_MEDIUM;
    int i = 0;

    for(i = 0; i < LCD_MODE; i++)
    {
        Pin.Port = LCD_Connection.LCD_DataPort[i];
        Pin.Pin =   LCD_Connection.LCD_DataPin[i];
        if(Pin.Port == GPIO_PORTA)
        {
            RCC_enableAHBPeriCLK(RCC_AHB1_GPIOA);
        }
        else if(Pin.Port == GPIO_PORTB)
        {
            RCC_enableAHBPeriCLK(RCC_AHB1_GPIOB);
        }
        else if(Pin.Port == GPIO_PORTC)
        {
            RCC_enableAHBPeriCLK(RCC_AHB1_GPIOC);
        }
        GPIO_init(&Pin);
        GPIO_SetPinValue(Pin.Port, Pin.Pin, GPIO_SET_PIN_LOW);
    }
}

void LCD_writeInitCommand(uint8_t command)
{
    static uint8_t counter = 0;
    if(LCD_MODE == FOUR_BIT_MODE)
    {
        
        if (counter == 0)
        {
            GPIO_SetPinValue(LCD_Connection.LCD_DataPort[RS], LCD_Connection.LCD_DataPin[RS], GPIO_SET_PIN_LOW);
            GPIO_SetPinValue(LCD_Connection.LCD_DataPort[RW], LCD_Connection.LCD_DataPin[RW], GPIO_SET_PIN_LOW);
            GPIO_SetPinValue(LCD_Connection.LCD_DataPort[3], LCD_Connection.LCD_DataPin[3], ( (command>>4) & 1));
            GPIO_SetPinValue(LCD_Connection.LCD_DataPort[4], LCD_Connection.LCD_DataPin[4], ( (command>>5) & 1));
            GPIO_SetPinValue(LCD_Connection.LCD_DataPort[5], LCD_Connection.LCD_DataPin[5], ( (command>>6) & 1));
            GPIO_SetPinValue(LCD_Connection.LCD_DataPort[6], LCD_Connection.LCD_DataPin[6], ( (command>>7) & 1));
            GPIO_SetPinValue(LCD_Connection.LCD_DataPort[EN], LCD_Connection.LCD_DataPin[EN],GPIO_SET_PIN_HIGH);                   
        }
        if(counter == 2)
        {
            GPIO_SetPinValue(LCD_Connection.LCD_DataPort[EN], LCD_Connection.LCD_DataPin[EN],GPIO_SET_PIN_LOW);
        }
        counter++;
        
        if(counter == 3)
        {
            counter = 0;
        }
        
    }
}

// void LCD_writeCommand(uint8_t command)
// {
//     static uint8_t counter = 0;

//     if(LCD_MODE == FOUR_BIT_MODE)
//     {
//         if(counter == 0)
//         {
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[RS], LCD_Connection.LCD_DataPin[RS], GPIO_SET_PIN_LOW);
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[RW], LCD_Connection.LCD_DataPin[RW], GPIO_SET_PIN_LOW);
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[3], LCD_Connection.LCD_DataPin[3], ( (command>>4) & 1));
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[4], LCD_Connection.LCD_DataPin[4], ( (command>>5) & 1));
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[5], LCD_Connection.LCD_DataPin[5], ( (command>>6) & 1));
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[6], LCD_Connection.LCD_DataPin[6], ( (command>>7) & 1));
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[EN], LCD_Connection.LCD_DataPin[EN],GPIO_SET_PIN_HIGH);       
//         }
//         else if(counter == 2)
//         {
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[EN], LCD_Connection.LCD_DataPin[EN],GPIO_SET_PIN_LOW);
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[3], LCD_Connection.LCD_DataPin[3], ( (command>>0) & 1));
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[4], LCD_Connection.LCD_DataPin[4], ( (command>>1) & 1));
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[5], LCD_Connection.LCD_DataPin[5], ( (command>>2) & 1));
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[6], LCD_Connection.LCD_DataPin[6], ( (command>>3) & 1));
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[EN], LCD_Connection.LCD_DataPin[EN],GPIO_SET_PIN_HIGH);
//         }
//         else if(counter == 4)
//         {
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[EN], LCD_Connection.LCD_DataPin[EN],GPIO_SET_PIN_LOW);
//         }
//         counter++;       
        
//     }    
// }

// static void LCD_writeData(uint8_t data)
// {
//     static uint8_t counter = 0;

//     if(LCD_MODE == FOUR_BIT_MODE)
//     {
//         if(counter == 0)
//         {
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[RS], LCD_Connection.LCD_DataPin[RS], GPIO_SET_PIN_HIGH);
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[RW], LCD_Connection.LCD_DataPin[RW], GPIO_SET_PIN_LOW);
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[3], LCD_Connection.LCD_DataPin[3], ( (data>>4) & 1));
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[4], LCD_Connection.LCD_DataPin[4], ( (data>>5) & 1));
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[5], LCD_Connection.LCD_DataPin[5], ( (data>>6) & 1));
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[6], LCD_Connection.LCD_DataPin[6], ( (data>>7) & 1));
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[EN], LCD_Connection.LCD_DataPin[EN],GPIO_SET_PIN_HIGH);       
//         }
//         else if(counter == 2)
//         {
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[EN], LCD_Connection.LCD_DataPin[EN],GPIO_SET_PIN_LOW);
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[3], LCD_Connection.LCD_DataPin[3], ( (data>>0) & 1));
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[4], LCD_Connection.LCD_DataPin[4], ( (data>>1) & 1));
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[5], LCD_Connection.LCD_DataPin[5], ( (data>>2) & 1));
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[6], LCD_Connection.LCD_DataPin[6], ( (data>>3) & 1));
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[EN], LCD_Connection.LCD_DataPin[EN],GPIO_SET_PIN_HIGH);
//         }
//         else if(counter == 4)
//         {
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[EN], LCD_Connection.LCD_DataPin[EN],GPIO_SET_PIN_LOW);
//         }
//         counter++;       
        
//     }        
// }

// void LCD_write(uint8_t type, uint8_t input)
// {
//     static uint8_t counter = 0;

//     if(LCD_MODE == FOUR_BIT_MODE)
//     {
//         if(counter == 0)
//         {
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[RS], LCD_Connection.LCD_DataPin[RS], type);
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[RW], LCD_Connection.LCD_DataPin[RW], GPIO_SET_PIN_LOW);
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[3], LCD_Connection.LCD_DataPin[3], ( (input>>4) & 1));
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[4], LCD_Connection.LCD_DataPin[4], ( (input>>5) & 1));
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[5], LCD_Connection.LCD_DataPin[5], ( (input>>6) & 1));
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[6], LCD_Connection.LCD_DataPin[6], ( (input>>7) & 1));
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[EN], LCD_Connection.LCD_DataPin[EN],GPIO_SET_PIN_HIGH);       
//         }
//         else if(counter == 2)
//         {
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[EN], LCD_Connection.LCD_DataPin[EN],GPIO_SET_PIN_LOW);
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[3], LCD_Connection.LCD_DataPin[3], ( (input>>0) & 1));
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[4], LCD_Connection.LCD_DataPin[4], ( (input>>1) & 1));
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[5], LCD_Connection.LCD_DataPin[5], ( (input>>2) & 1));
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[6], LCD_Connection.LCD_DataPin[6], ( (input>>3) & 1));
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[EN], LCD_Connection.LCD_DataPin[EN],GPIO_SET_PIN_HIGH);
//         }
//         if(counter == 4)
//         {
//             GPIO_SetPinValue(LCD_Connection.LCD_DataPort[EN], LCD_Connection.LCD_DataPin[EN],GPIO_SET_PIN_LOW);
//             LCD_INPUT_FINISHED = 1;
//             counter = 0;
//         }
        
//         counter++;

//         if(counter == 5)
//         {
//             counter = 0;
//         }
        
//     }           
// }


void LCD_ActionEnableHighMostSig(volatile uint8_t input, uint8_t type)
{
    if(LCD_MODE == FOUR_BIT_MODE)
    {
            GPIO_SetPinValue(LCD_Connection.LCD_DataPort[RS], LCD_Connection.LCD_DataPin[RS], type);
            GPIO_SetPinValue(LCD_Connection.LCD_DataPort[RW], LCD_Connection.LCD_DataPin[RW], GPIO_SET_PIN_LOW);
            GPIO_SetPinValue(LCD_Connection.LCD_DataPort[3], LCD_Connection.LCD_DataPin[3], ( (input>>4) & 1));
            GPIO_SetPinValue(LCD_Connection.LCD_DataPort[4], LCD_Connection.LCD_DataPin[4], ( (input>>5) & 1));
            GPIO_SetPinValue(LCD_Connection.LCD_DataPort[5], LCD_Connection.LCD_DataPin[5], ( (input>>6) & 1));
            GPIO_SetPinValue(LCD_Connection.LCD_DataPort[6], LCD_Connection.LCD_DataPin[6], ( (input>>7) & 1));
            GPIO_SetPinValue(LCD_Connection.LCD_DataPort[EN], LCD_Connection.LCD_DataPin[EN],GPIO_SET_PIN_HIGH);   
    }    
}

void LCD_ActionEnableHighLeastSig(uint8_t input)
{
    if(LCD_MODE == FOUR_BIT_MODE)
    {
        GPIO_SetPinValue(LCD_Connection.LCD_DataPort[EN], LCD_Connection.LCD_DataPin[EN],GPIO_SET_PIN_LOW);
        GPIO_SetPinValue(LCD_Connection.LCD_DataPort[3], LCD_Connection.LCD_DataPin[3], ( (input>>0) & 1));
        GPIO_SetPinValue(LCD_Connection.LCD_DataPort[4], LCD_Connection.LCD_DataPin[4], ( (input>>1) & 1));
        GPIO_SetPinValue(LCD_Connection.LCD_DataPort[5], LCD_Connection.LCD_DataPin[5], ( (input>>2) & 1));
        GPIO_SetPinValue(LCD_Connection.LCD_DataPort[6], LCD_Connection.LCD_DataPin[6], ( (input>>3) & 1));
        GPIO_SetPinValue(LCD_Connection.LCD_DataPort[EN], LCD_Connection.LCD_DataPin[EN],GPIO_SET_PIN_HIGH);
    }    
}

void LCD_ActionEnableLow(void)
{
    if(LCD_MODE == FOUR_BIT_MODE)
    {
        GPIO_SetPinValue(LCD_Connection.LCD_DataPort[EN], LCD_Connection.LCD_DataPin[EN],GPIO_SET_PIN_LOW); 
    }    
}


void LCD_writeProc(void)
{
    // static uint8_t counter = 0;
    // if(LCD_INPUT_FINISHED == 1)
    // {
    //     counter++;
    //     LCD_INPUT_FINISHED = 0;
    // }
    // if(counter < User_Req.len)
    // {
    //     LCD_write(LCD_INPUT_DATA, User_Req.s[counter]);
    // }
    // else if(counter == User_Req.len)
    // {
    //     counter = 0;
    //     User_Req.state = REQ_STATE_READY;
    //     if(write_process.CB)
    //     {
    //         write_process.CB();
    //     }
    // }
    static uint8_t LCD_sendingState = LCD_SendMOSTSIG;
    static uint8_t timer = 0;

    if (Pos_Flag == 0)
    {
        if(write_process.cursorPos < User_Req.len) 
        {
            timer++; 
            switch(LCD_sendingState)
            {
                case LCD_SendMOSTSIG:
                    if(timer == 1)
                    {
                        LCD_ActionEnableHighMostSig(User_Req.s[write_process.cursorPos], LCD_INPUT_DATA);
                    }   
                    if(timer == 2)
                    {
                        LCD_sendingState = LCD_SENDLEASTSIG;
                    }
                    break;
                case LCD_SENDLEASTSIG:
                    if(timer == 3)
                    {
                        LCD_ActionEnableHighLeastSig(User_Req.s[write_process.cursorPos]);
                    }
                    
                    if(timer == 4)
                    {
                        LCD_sendingState = LCD_SENDENLOW;
                    }
                    break;
                case LCD_SENDENLOW:
                    if(timer == 5)
                    {
                        LCD_ActionEnableLow();
                    }
                    if(timer == 6)
                    {
                        LCD_sendingState = LCD_SendMOSTSIG;
                        write_process.cursorPos++;
                        timer = 0;
                    }
                    break;
                default:
                    break;
                    
            }
        }

        if(write_process.cursorPos == User_Req.len)
        {
            User_Req.type = REQ_TYPE_NOREQ;
            User_Req.state = REQ_STATE_READY;
            if(write_process.CB)
            {
                write_process.CB();
            }
        }
    }
    if(timer == 0 && write_process.cursorPos == 0)
    {
        LCD_setPosProc();
    }
}
    

void LCD_clearProc(void)
{
    static uint8_t LCD_clearState = LCD_INIT_STATE_DISPLAY_CLEAR_1;
    static uint8_t time = 0;
    time++;
    // static counter = 0;
    // counter++;
    // if(counter < 5)
    // {
    //     LCD_write(LCD_INPUT_COMMAND, DISPLAY_CLEAR_1);
    // }
    // else if(counter < 9)
    // {
    //     LCD_write(LCD_INPUT_COMMAND, DISPLAY_CLEAR_2);
    // }
    // else if(counter < 13)
    // {
    //     LCD_write(LCD_INPUT_COMMAND, ENTRY_MODE_SET_1);
    // }
    // else if(counter < 17)
    // {
    //     LCD_write(LCD_INPUT_COMMAND, ENTRY_MODE_SET_2);
    // }
    // else
    // {
    //     User_Req.state = REQ_STATE_READY;
    //     counter = 0;
    //     if(write_process.CB)
    //     {
    //         write_process.CB();
    //     }
    // }
    switch(LCD_clearState)
    {
    case LCD_INIT_STATE_DISPLAY_CLEAR_1:
        LCD_writeInitCommand(DISPLAY_CLEAR_1);
        if(time == 2)
        {
            LCD_clearState =LCD_INIT_STATE_DISPLAY_CLEAR_2;
        }
        break;
    case LCD_INIT_STATE_DISPLAY_CLEAR_2:
        LCD_writeInitCommand(DISPLAY_CLEAR_2);
        if(time == 4)
        {
            LCD_clearState =LCD_INIT_STATE_ENTRY_MODE_SET_1;
        }
        break;
    case LCD_INIT_STATE_ENTRY_MODE_SET_1:
        LCD_writeInitCommand(ENTRY_MODE_SET_1);
        if(time == 6)
        {
            LCD_clearState =LCD_INIT_STATE_ENTRY_MODE_SET_2;
        }
        break;
    case LCD_INIT_STATE_ENTRY_MODE_SET_2:
        LCD_writeInitCommand(ENTRY_MODE_SET_2);
        if(time == 8)
        {
            LCD_clearState = LCD_INIT_STATE_END;;
        }
        break;
    case LCD_INIT_STATE_END:
        time = 0;
        LCD_clearState =LCD_INIT_STATE_DISPLAY_CLEAR_1;
        User_Req.type = REQ_TYPE_NOREQ;
        User_Req.state = REQ_STATE_READY;
        if(write_process.CB)
        {
            write_process.CB();
        }
        break;
    }

}

void LCD_setPosProc(void)
{
    uint32_t LOC_var = 0;
    static uint8_t timer = 0;
    timer++;
    static uint8_t LCD_setPosState = LCD_SendMOSTSIG;
    if(User_Req.xpos == 0)
    {
        LOC_var = User_Req.ypos;
    }
    else if(User_Req.xpos == 1)
    {
        LOC_var = User_Req.ypos + 0x40 ;
    }
    switch(LCD_setPosState)
    {
        case LCD_SendMOSTSIG:
            Pos_Flag = 1;
            if(timer == 1)
            {
                LCD_ActionEnableHighMostSig((LOC_var+128), LCD_INPUT_COMMAND);
            }
            
            if(timer == 2)
            {
                LCD_setPosState = LCD_SENDLEASTSIG;
            }
            break;
        case LCD_SENDLEASTSIG:
            if(timer == 3)
            {
                LCD_ActionEnableHighLeastSig((LOC_var+128));
            }     
            if(timer == 4)
            {
                LCD_setPosState = LCD_SENDENLOW;
            }
            break;
        case LCD_SENDENLOW:
            if(timer == 5)
            {
                LCD_ActionEnableLow();
            }     
            if(timer == 6)
            {
                LCD_setPosState = LCD_SendMOSTSIG;
                timer = 0;
                //User_Req.state = REQ_STATE_READY;
                //User_Req.type = REQ_TYPE_NOREQ;
                Pos_Flag = 0;
                if(write_process.CB)
                {
                    write_process.CB();
                }
            }
            break;
        default:
            break;
            
    }
}
/******************************************************************************
*
* Module: Keypad
*
* File Name: Keypad.c
*
* Description: Source file for Keypad.c module
*
* Author: Momen Elsayed Shaban
*
*******************************************************************************/
/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "HAL/KEYPAD/Keypad.h"
#include "MCAL/GPIO/GPIO.h"

#ifndef NULL_PTR
#define NULL_PTR ((void*)0)
#endif

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
extern KEYPAD_strConfig_t KEYPAD_strConfig;										/* External variable for keypad configuration */
	
extern char KEYPAD_Charchters[KEYPAD_NUMBER_OF_ROWS][KEYPAD_NUMBER_OF_COLS];     /* External array for keypad characters */

uint16_t Switch_State[KEYPAD_NUMBER_OF_ROWS][KEYPAD_NUMBER_OF_COLS];


/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/
KEYPAD_enumError_t KEYPAD_Init()
{
    /* Initial state for the KEYPAD local variable is OK */
    KEYPAD_enumError_t loc_KEYPADError = KEYPAD_enumOK;

    /* Local Structure From PORT Module That will be Passed to PORT_setPinConfig API to setup the Configurations for the KEYPAD */
    GPIO_Pin_t loc_PORTstr;

    /* Local Iterator Variable that Used in For Loops in the Function */
    uint8_t loc_iterator = 0;

    /* Check for Errors If the User Entered a Number of Rows Or Columns with a value equal or less than 0 which is invalid */
    if (KEYPAD_NUMBER_OF_ROWS <= 0)
    {
        loc_KEYPADError = KEYPAD_enumInvalidRowsNumber;    /* Assign the specified error value to the local error variable */
    }
    else if (KEYPAD_NUMBER_OF_COLS <= 0)
    {
        loc_KEYPADError = KEYPAD_enumInvalidColsNumber;    /* Assign the specified error value to the local error variable */
    }
    else if (KEYPAD_strConfig.KEYPAD_enumConnection != KEYPAD_enumPullUp || KEYPAD_strConfig.KEYPAD_enumConnection != KEYPAD_enumPullDown)
    {
        loc_KEYPADError = KEYPAD_enumInvalidConnectionType;/* Assign the specified error value to the local error variable */
    }
    else if (KEYPAD_strConfig.KEYPAD_enumLogicType != KEYPAD_enumLogicHigh || KEYPAD_strConfig.KEYPAD_enumLogicType != KEYPAD_enumLogicLow)
    {
        loc_KEYPADError = KEYPAD_enumInvalidLogicType;		/* Assign the specified error value to the local error variable */
    }
    else
    {
        /* Do nothing */
    }

    /* Iterates Over the Configurations structure to Initialize the Rows with Input Internal Pull Up Resistor */
    for (loc_iterator = 0; loc_iterator < KEYPAD_NUMBER_OF_ROWS; loc_iterator++)
    {
        loc_PORTstr.Port = KEYPAD_strConfig.KEYPAD_strROWS[loc_iterator].KEYPAD_enumPORT;			/* Assign The Keypad Rows Ports to the local Port Struct */
        loc_PORTstr.Pin = KEYPAD_strConfig.KEYPAD_strROWS[loc_iterator].KEYPAD_enumPin;			/* Assign The Keypad Cols Pins to the Local Port Struct */
        loc_PORTstr.Mode = GPIO_MODE_IN_PU;			
        loc_PORTstr.Speed = GPIO_SPEED_HIGH;											/* Configure the Required Pin to Input Pull Up */
        GPIO_Init(&loc_PORTstr);																        /* Sending the Configuration to the Port To Make the Configurations Take Action */
    }

    /* Iterates Over the Configurations structure to Initialize the Cols with Output Logic High */
    for (loc_iterator = 0; loc_iterator < KEYPAD_NUMBER_OF_COLS; loc_iterator++)
    {
        loc_PORTstr.Port = KEYPAD_strConfig.KEYPAD_strCOLS[loc_iterator].KEYPAD_enumPORT;			/* Assign The Keypad Rows Ports to the local Port Struct */
        loc_PORTstr.Pin = KEYPAD_strConfig.KEYPAD_strCOLS[loc_iterator].KEYPAD_enumPin;			/* Assign The Keypad Cols Pins to the Local Port Struct */
        loc_PORTstr.Mode = GPIO_MODE_OP_PP;			
        loc_PORTstr.Speed = GPIO_SPEED_HIGH;												/* Configure the Required Pin to Output */
        GPIO_Init(&loc_PORTstr);					
        GPIO_SetPinState(KEYPAD_strConfig.KEYPAD_strCOLS[loc_iterator].KEYPAD_enumPORT,KEYPAD_strConfig.KEYPAD_strCOLS[loc_iterator].KEYPAD_enumPin,GPIO_STATE_HIGH);												    /* Sending the Configuration to the Port To Make the Configurations Take Action */
    }

    return loc_KEYPADError; /* Return an Error Status from the Function */
}

KEYPAD_enumError_t KEYPAD_getPressedKey(char* Add_KEYPAD_enumSwitch)
{
    /* Initial state for the KEYPAD local variable is OK */
    KEYPAD_enumError_t loc_KEYPADError = KEYPAD_enumOK;
    /* Local iterator rows variable used in inner for-loops in the function */
    uint8_t loc_iteratorRows = 0;
    /* Local iterator cols variable used in outer for-loops in the function */
    uint8_t loc_iteratorCols = 0;
    uint8_t loc_checkValue = GPIO_STATE_LOW;
    /* Determine the expected logic level based on keypad connection and logic type */
    if (KEYPAD_strConfig.KEYPAD_enumConnection == KEYPAD_enumPullUp && KEYPAD_strConfig.KEYPAD_enumLogicType == KEYPAD_enumLogicHigh)
    {
        loc_checkValue = GPIO_STATE_LOW;
    }
    else if (KEYPAD_strConfig.KEYPAD_enumConnection == KEYPAD_enumPullUp && KEYPAD_strConfig.KEYPAD_enumLogicType == KEYPAD_enumLogicLow)
    {
        loc_checkValue = GPIO_STATE_HIGH;
    }
    else if (KEYPAD_strConfig.KEYPAD_enumConnection == KEYPAD_enumPullDown && KEYPAD_strConfig.KEYPAD_enumLogicType == KEYPAD_enumLogicHigh)
    {
        loc_checkValue = GPIO_STATE_LOW;
    }
    else if (KEYPAD_strConfig.KEYPAD_enumConnection == KEYPAD_enumPullDown && KEYPAD_strConfig.KEYPAD_enumLogicType == KEYPAD_enumLogicLow)
    {
        loc_checkValue = GPIO_STATE_HIGH;
    }
    else
    {
        loc_KEYPADError = KEYPAD_enumInvalidConnectionType;
    }
    /* Iterates over the keypad matrix by setting the columns with logic high and then checking each row */
    for (loc_iteratorCols = 0; loc_iteratorCols < KEYPAD_NUMBER_OF_COLS; loc_iteratorCols++)
    {
        for (loc_iteratorRows = 0; loc_iteratorRows < KEYPAD_NUMBER_OF_ROWS; loc_iteratorRows++)
        {
            /* Check if any switch is pressed or not */
            if(Switch_State[loc_iteratorCols][loc_iteratorRows] == loc_checkValue)
            {
                /* In case of any switch pressed, return the corresponding row and column in the user-configured characters */
                *Add_KEYPAD_enumSwitch = KEYPAD_Charchters[loc_iteratorRows][loc_iteratorCols];
            }
            else
            {
                /* Do nothing */
            }
        }
    }
    /* Check the null pointer */
    if (Add_KEYPAD_enumSwitch == NULL_PTR)
    {
        loc_KEYPADError = KEYPAD_enumNullPtr;
    }
    else
    {
        /* Do nothing */
    }

    /* Return an error status from the function */
    return loc_KEYPADError;
}
/*Every 6 ms*/
void Keypad_Runnable(void)
{
    uint8_t loc_iteratorRows = 0;
    uint8_t loc_iteratorCols = 0;
	uint8_t SW_CurrentState;
    static uint8_t prev[KEYPAD_NUMBER_OF_COLS][KEYPAD_NUMBER_OF_ROWS] = {0};
	static uint8_t counts[KEYPAD_NUMBER_OF_COLS][KEYPAD_NUMBER_OF_ROWS] = {0};

    /* Iterates over the keypad matrix by setting the columns with logic high and then checking each row */
    for (loc_iteratorCols = 0; loc_iteratorCols < KEYPAD_NUMBER_OF_COLS; loc_iteratorCols++)
    {
        /* Set each column of the keypad matrix with logic high to check every row */
        GPIO_SetPinState(KEYPAD_strConfig.KEYPAD_strCOLS[loc_iteratorCols].KEYPAD_enumPORT,KEYPAD_strConfig.KEYPAD_strCOLS[loc_iteratorCols].KEYPAD_enumPin,GPIO_STATE_LOW);

        for (loc_iteratorRows = 0; loc_iteratorRows < KEYPAD_NUMBER_OF_ROWS; loc_iteratorRows++)
        {
            /* Read the value for each switch in each row and hold it in loc_pinValue */
            GPIO_GetPinState(KEYPAD_strConfig.KEYPAD_strROWS[loc_iteratorRows].KEYPAD_enumPORT,KEYPAD_strConfig.KEYPAD_strROWS[loc_iteratorRows].KEYPAD_enumPin,&SW_CurrentState);
            if (SW_CurrentState == prev[loc_iteratorCols][loc_iteratorRows])
            {
                counts[loc_iteratorCols][loc_iteratorRows]++;
            }
            else
            {
                counts[loc_iteratorCols][loc_iteratorRows] = 0;
            }
            if(counts[loc_iteratorCols][loc_iteratorRows] == 5)
            {
                Switch_State[loc_iteratorCols][loc_iteratorRows] = SW_CurrentState;
                counts[loc_iteratorCols][loc_iteratorRows] = 0;
            }
            prev[loc_iteratorCols][loc_iteratorRows]= SW_CurrentState;
        }

        /* Set the value of the column to high again to check the next one */
        GPIO_SetPinState(KEYPAD_strConfig.KEYPAD_strCOLS[loc_iteratorCols].KEYPAD_enumPORT,KEYPAD_strConfig.KEYPAD_strCOLS[loc_iteratorCols].KEYPAD_enumPin,GPIO_STATE_HIGH);
    }
}
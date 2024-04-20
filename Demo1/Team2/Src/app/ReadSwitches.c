/******************************************************************************
*
* Module: ReadSwitches
*
* File Name: ReadSwitches.c
*
* Description: Source file for ReadSwitches Module in App Layer for Demo1
*
* Author: Momen Elsayed Shaban
*
*******************************************************************************/
/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "HAL/KEYPAD/Keypad.h"
#include "COMM/LIN_SLAVE/LIN_SlaveAppData.h"
#include "MCAL/GPIO/GPIO.h"
/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

enum switches{
    Up,
    Down,
    Left,
    Right,
    Start,
    Stop,
    Edit,
    Mode,
    Ok
};

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/

uint16_t Controls;

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/
/*Task Every 60 ms*/
/*Must be delayed for 30 ms the time to set the status of switches*/
void readSwitches (void)
{
    char pressedKey;
    Controls = 0;
    KEYPAD_getPressedKey(&pressedKey);
    
    switch (pressedKey)
    {
        case 'U':
            Controls = (1 << Up);
            break;
        case 'D':
            Controls = (1 << Down);
            break;
        case 'L':
            Controls = (1 << Left);
            break;
        case 'R':
            Controls = (1 << Right);
            break;
        case 'S':
            Controls = (1 << Start);
            break;
        case 'P':
            Controls = (1 << Stop);
            break;
        case 'E':
            Controls = (1 << Edit);
            break;
        case 'M':
            Controls = (1 << Mode);
            break;
        case 'K':
            Controls = (1 << Ok);
            GPIO_SetPinState(GPIO_PORT_B, GPIO_PIN_13,GPIO_STATE_HIGH);
            break;
        default:
            Controls = 0;
            break;
    }

}
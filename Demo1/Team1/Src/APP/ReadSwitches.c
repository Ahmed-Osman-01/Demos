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

u16 Switches_Status = 0;

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/
/*Task Every 60 ms*/
/*Must be delayed for 30 ms the time to set the status of switches*/
void readSwitches (void)
{
    char pressedKey;
    Switches_Status = 0;
    KEYPAD_getPressedKey(&pressedKey);
    
    switch (pressedKey)
    {
        case 'U':
            Switches_Status = (1 << Up);
            break;
        case 'D':
            Switches_Status = (1 << Down);
            break;
        case 'L':
            Switches_Status = (1 << Left);
            break;
        case 'R':
            Switches_Status = (1 << Right);
            break;
        case 'S':
            Switches_Status = (1 << Start);
            break;
        case 'P':
            Switches_Status = (1 << Stop);
            break;
        case 'E':
            Switches_Status = (1 << Edit);
            break;
        case 'M':
            Switches_Status = (1 << Mode);
            break;
        case 'K':
            Switches_Status = (1 << Ok);
            break;
        default:
            Switches_Status = 0;
            break;
    }
}
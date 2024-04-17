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
#include "HAL/LIN/LIN_MASTER/LIN_MasterAppData.h"
/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
enum switches_Directions{
    Up,
    Down,
    Left,
    Right,
};


enum switches_Control{
    Start,
    Stop,
    Edit,
    Mode,
    Ok,
    Reset
};

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/

extern uint8_t Switches_CTRL_Control;
extern uint8_t Switches_CTRL_Directions;
/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/
/*Task Every 60 ms*/
/*Must be delayed for 30 ms the time to set the status of switches*/
void readSwitches (void)
{
    char pressedKey;
    Switches_CTRL_Control = 0;
    Switches_CTRL_Directions = 0;
    KEYPAD_getPressedKey(&pressedKey);
    
    switch (pressedKey)
    {
        case 'U':
            Switches_CTRL_Directions = (1 << Up);
            Switches_CTRL_Control = (1 << Reset);
            break;
        case 'D':
            Switches_CTRL_Directions = (1 << Down);
            break;
        case 'L':
            Switches_CTRL_Directions = (1 << Left);
            break;
        case 'R':
            Switches_CTRL_Directions = (1 << Right);
            break;
        case 'S':
            Switches_CTRL_Control = (1 << Start);
            break;
        case 'P':
            Switches_CTRL_Control = (1 << Stop);
            break;
        case 'E':
            Switches_CTRL_Control = (1 << Edit);
            break;
        case 'M':
            Switches_CTRL_Control = (1 << Mode);
            break;
        case 'K':
            Switches_CTRL_Control = (1 << Ok);
            break;
        default:
            Switches_CTRL_Control = 0;
            Switches_CTRL_Directions = 0;
            break;
    }
}
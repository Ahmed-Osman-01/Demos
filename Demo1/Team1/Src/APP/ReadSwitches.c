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
    Start,
    Stop,
    Edit,
    Mode,
    Ok,
};

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
extern uint16_t Switches_Status_send;
/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/
/*Task Every 60 ms*/
/*Must be delayed for 30 ms the time to set the status of switches*/
void readSwitches (void)
{
    uint8_t pressedKey = 0;
    Switches_Status_send = 0;
    KEYPAD_getPressedKey(&pressedKey);
    
    switch (pressedKey)
    {
        case 'U':
            Switches_Status_send = (1 << Up);
      //      Switches_CTRL_Control = (1 << Reset);
            break;
        case 'D':
            Switches_Status_send = (1 << Down);
            break;
        case 'L':
            Switches_Status_send = (1 << Left);
            break;
        case 'R':
            Switches_Status_send = (1 << Right);
            break;
        case 'S':
            Switches_Status_send = (1 << Start);
            break;
        case 'P':
            Switches_Status_send = (1 << Stop);
            break;
        case 'E':
            Switches_Status_send = (1 << Edit);
            break;
        case 'M':
            Switches_Status_send = (1 << Mode);
            break;
        case 'K':
            Switches_Status_send = (1 << Ok);
            break;
        default:
            Switches_Status_send = 0;
            break;
    }
}
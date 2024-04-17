/******************************************************************************
*
* Module: Edit Time Mode
*
* File Name: EditTimeMode.c
*
* Description: Source file for the EditTimeMode Module for Demo1
*
* Author: Momen Elsayed Shaban
*
*******************************************************************************/
/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include <stdint.h>
#include "HAL/LCD/LCD_HAL.h"


/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#ifndef NULL
#define NULL ((void*)0)
#endif
#define DATE_POS_Y          0
#define DAY2_POS_X          3
#define DAY1_POS_X          4

#define MONTH2_POS_X        6
#define MONTH1_POS_X        7

#define YEAR4_POS_X         9
#define YEAR3_POS_X         10
#define YEAR2_POS_X         11
#define YEAR1_POS_X         12

#define TIME_POS_Y          1
#define HOUR2_POS_X         2
#define HOUR1_POS_X         3

#define MINUTES2_POS_X      5
#define MINUTES1_POS_X      6

#define SECONDS2_POS_X      8
#define SECONDS1_POS_X      9

#define UP_MASK             0x0001
#define DOWN_MASK           0x0002
#define LEFT_MASK           0x0004
#define RIGHT_MASK          0x0008
#define EDIT_MASK           0x0040
#define OK_MASK             0x1000

enum cursorPos{
    x_pos,
    y_pos
};

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
// enum states{
//     Edit_Mode,
//     Edit_Digit,
// };



enum time {
    millisecond,
    sec1,
    sec2,
    min1,
    min2,
    hour1,
    hour2,
};


/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
volatile uint8_t Valid_X_Pos_Values[6] = {HOUR2_POS_X,HOUR1_POS_X,MINUTES2_POS_X,MINUTES1_POS_X,SECONDS2_POS_X,SECONDS1_POS_X};

volatile int8_t cursor_pos[2] = {
    [x_pos] = 2,
    [y_pos] = 1,
};

volatile int16_t EditedTime[7]={
    [millisecond] = 0,
    [sec1] = 0,
    [sec2] = 0,
    [min1] = 0,
    [min2] = 0,
    [hour1] = 0,
    [hour2] = 0,
};

//volatile uint8_t Edit_state = Edit_Mode;
volatile uint8_t validCursorPos = 0;
extern volatile uint16_t Switches_Status;
extern volatile uint16_t Time[7];
volatile uint8_t EditDigit_Flag = 0;
extern volatile uint8_t temp4[13];
volatile uint8_t trackCursorFlag = 0;
volatile uint8_t EditTimeFlag = 0;
/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/
void trackCursor(void)
{
    if(Switches_Status & UP_MASK)
    {
        cursor_pos[y_pos]--;
        if(cursor_pos[y_pos] < 0)
        {
            cursor_pos[y_pos] = 1;
        }
        trackCursorFlag = 1;
    }
    else if(Switches_Status & DOWN_MASK)
    {
        cursor_pos[y_pos]++;
        if(cursor_pos[y_pos] > 1)
        {
            cursor_pos[y_pos] = 0;
        }
        trackCursorFlag = 1;
    }
    else if(Switches_Status & LEFT_MASK)
    {
        cursor_pos[x_pos]--;
        if(cursor_pos[x_pos] < 0)
        {
            cursor_pos[x_pos] = 15;
        }
        trackCursorFlag = 1;
    }
    else if(Switches_Status & RIGHT_MASK)
    {
        cursor_pos[x_pos]++;
        if(cursor_pos[x_pos] > 15)
        {
            cursor_pos[x_pos] = 0;
        }
        trackCursorFlag = 1;
    }
    
}

void validateCursorPos(uint8_t x_pos , uint8_t y_pos)
{
    uint8_t iterator = 0;
    uint8_t valid_y_pos = 0;
    uint8_t valid_x_pos = 0;
    if(y_pos == 1)
    {
        valid_y_pos = 1;
    }
    else
    {
        valid_y_pos = 0;
    }
    for(iterator = 0; iterator < 6 ; iterator++)
    {
        if(x_pos == Valid_X_Pos_Values[iterator])
        {
            valid_x_pos = 1;
            break;
        }
        else
        {
            valid_x_pos = 0;
        }
    }
    validCursorPos = valid_y_pos & valid_x_pos;
}

void CopyTime (volatile uint16_t* srcTime,volatile uint16_t* destTime)
{
    uint8_t idx = 0;
    for(idx = 0; idx < 7; idx++)
    {
        destTime[idx] = srcTime[idx];
    }
}

void EditTime (void)
{
    if(cursor_pos[x_pos] == HOUR2_POS_X && cursor_pos[y_pos] == TIME_POS_Y )
    {
        EditDigit_Flag = (1 << hour2);
        if(Switches_Status & UP_MASK)
        {
            EditedTime[hour2]++;
            if(EditedTime[hour2] > 2)
            {
                EditedTime[hour2] = 0;
            }
            temp4[0] = EditedTime[hour2] + '0';
            EditTimeFlag = 1;
        }
        else if(Switches_Status & DOWN_MASK)
        {
            EditedTime[hour2]--;
            if(EditedTime[hour2] < 0)
            {
                EditedTime[hour2] = 2;
            }
            EditTimeFlag = 1;
        }
        else{
            /*Do Nothing*/
        }
    }
    else if (cursor_pos[x_pos] == HOUR1_POS_X && cursor_pos[y_pos] == TIME_POS_Y)
    {
        EditDigit_Flag = (1 << hour1);
        if (Switches_Status & UP_MASK)
        {
            EditedTime[hour1]++;
            if (EditedTime[hour1] > 4)
            {
                EditedTime[hour1] = 0;
            }
            temp4[1] = EditedTime[hour1] + '0';
            EditTimeFlag = 1;
        }
        else if (Switches_Status & DOWN_MASK)
        {
            EditedTime[hour1]--;
            if (EditedTime[hour1] < 0)
            {
                EditedTime[hour1] = 9;
            }
            temp4[1] = EditedTime[hour1] + '0';
            EditTimeFlag = 1;
        }
    }
    else if (cursor_pos[x_pos] == MINUTES2_POS_X && cursor_pos[y_pos] == TIME_POS_Y)
    {
        EditDigit_Flag = (1 << min2);
        if (Switches_Status & UP_MASK)
        {
            EditedTime[min2]++;
            if (EditedTime[min2] > 5)
            {
                EditedTime[min2] = 0;
            }
            temp4[3] = EditedTime[min2] + '0';
            EditTimeFlag = 1;
        }
        else if (Switches_Status & DOWN_MASK)
        {
            EditedTime[min2]--;
            if (EditedTime[min2] < 0)
            {
                EditedTime[min2] = 5;
            }
            temp4[3] = EditedTime[min2] + '0';
            EditTimeFlag = 1;

        }
    }
    else if (cursor_pos[x_pos] == MINUTES1_POS_X && cursor_pos[y_pos] == TIME_POS_Y)
    {
        EditDigit_Flag = (1 << min1);
        if (Switches_Status & UP_MASK)
        {
            EditedTime[min1]++;
            if (EditedTime[min1] > 9)
            {
                EditedTime[min1] = 0;
            }
            temp4[4] = EditedTime[min1] + '0';
            EditTimeFlag = 1;

        }
        else if (Switches_Status & DOWN_MASK)
        {
            EditedTime[min1]--;
            if (EditedTime[min1] < 0)
            {
                EditedTime[min1] = 9;
            }
            temp4[4] = EditedTime[min1] + '0';
            EditTimeFlag = 1;

        }

    }
    else if (cursor_pos[x_pos] == SECONDS2_POS_X && cursor_pos[y_pos] == TIME_POS_Y)
    {
        EditDigit_Flag = (1 << sec2);
        if (Switches_Status & UP_MASK)
        {
            EditedTime[sec2]++;
            if (EditedTime[sec2] > 5)
            {
                EditedTime[sec2] = 0;
            }
            temp4[6] = EditedTime[sec2] + '0';
            EditTimeFlag = 1;

        }
        else if (Switches_Status & DOWN_MASK)
        {
            EditedTime[sec2]--;
            if (EditedTime[sec2] < 0)
            {
                EditedTime[sec2] = 5;
            }
            temp4[6] = EditedTime[sec2] + '0';
            EditTimeFlag = 1;

        }
    }
    else if (cursor_pos[x_pos] == SECONDS1_POS_X && cursor_pos[y_pos] == TIME_POS_Y)
    {
        EditDigit_Flag = (1 << sec1);
        if (Switches_Status & UP_MASK)
        {
            EditedTime[sec1]++;
            if (EditedTime[sec1] > 9)
            {
                EditedTime[sec1] = 0;
            }
            temp4[7] = EditedTime[sec1] + '0';
            EditTimeFlag = 1;

        }
        else if (Switches_Status & DOWN_MASK)
        {
            EditedTime[sec1]--;
            if (EditedTime[sec1] < 0)
            {
                EditedTime[sec1] = 9;
            }
            temp4[7] = EditedTime[sec1] + '0';
            EditTimeFlag = 1;

        }
    }
    CopyTime((uint16_t *)EditedTime,Time);
}

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

// void EditTimeModeSM(void)
// {
//     CopyTime(Time,(uint16_t *)EditedTime);
//     switch(Edit_state)
//     {
//         case Edit_Mode:
//             trackCursor();
//             validateCursorPos(cursor_pos[x_pos],cursor_pos[y_pos]);
//             if((Switches_Status & EDIT_MASK) && validCursorPos)
//             {
//                 Edit_state = Edit_Digit;
//             }
//             break;
//         case Edit_Digit:
//             EditTime();
//             if(Switches_Status & OK_MASK)
//             {
//                 Edit_state = Edit_Mode;
//             }
//             break;
//     }
// }
#if 0
switch(Edit_state)
    {
        case Normal_Display:
            if(Switches_Status & EDIT_MASK)
            {
                EditMode_flag = 0;
                Edit_state = Edit_Mode;
            }
            break;
        case Edit_Mode:
            trackCursor();
            if(Switches_Status & EDIT_MASK && EditMode_flag == 0)
            {
                Edit_state = Edit_Digit;
            }

    }
#endif
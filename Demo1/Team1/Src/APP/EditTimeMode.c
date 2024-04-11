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
#include "LIB/std_types.h"
#include "HAL/LCD/LCD.h"


/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
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
enum states{
    Edit_Mode,
    Edit_Digit,
};



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
u8 Valid_X_Pos_Values[6] = {HOUR2_POS_X,HOUR1_POS_X,MINUTES2_POS_X,MINUTES1_POS_X,SECONDS2_POS_X,SECONDS1_POS_X};

s8 cursor_pos[2] = {
    [x_pos] = 0,
    [y_pos] = 0,
};

u16 EditedTime[7]={
    [millisecond] = 0,
    [sec1] = 0,
    [sec2] = 0,
    [min1] = 0,
    [min2] = 0,
    [hour1] = 0,
    [hour2] = 0,
};

u8 Edit_state = Edit_Mode;
static u8 validCursorPos = 0;
extern u16 Switches_Status;
extern u16 Time[7];
u8 EditDigit_Flag = 0;

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/
static void trackCursor(void)
{
    if(Switches_Status & UP_MASK)
    {
        cursor_pos[y_pos]--;
        if(cursor_pos[y_pos] < 0)
        {
            cursor_pos[y_pos] = 1;
        }
    }
    else if(Switches_Status & DOWN_MASK)
    {
        cursor_pos[y_pos]++;
        if(cursor_pos[y_pos] > 1)
        {
            cursor_pos[y_pos] = 0;
        }
    }
    else if(Switches_Status & LEFT_MASK)
    {
        cursor_pos[x_pos]--;
        if(cursor_pos[x_pos] < 0)
        {
            cursor_pos[x_pos] = 15;
        }
    }
    else if(Switches_Status & RIGHT_MASK)
    {
        cursor_pos[x_pos]++;
        if(cursor_pos[x_pos] > 15)
        {
            cursor_pos[x_pos] = 0;
        }
    }
}

static void validateCursorPos(u8 x_pos , u8 y_pos)
{
    u8 iterator = 0;
    u8 valid_y_pos = 0;
    u8 valid_x_pos = 0;
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

static void CopyTime (u16* srcTime, u16* destTime)
{
    u8 idx = 0;
    for(idx = 0; idx < 7; idx++)
    {
        destTime[idx] = srcTime[idx];
    }
}

static void EditTime (void)
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
        }
        else if(Switches_Status & DOWN_MASK)
        {
            EditedTime[hour2]--;
            if(EditedTime[hour2] < 0)
            {
                EditedTime[hour2] = 2;
            }
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
        }
        else if (Switches_Status & DOWN_MASK)
        {
            EditedTime[hour1]--;
            if (EditedTime[hour1] < 0)
            {
                EditedTime[hour1] = 9;
            }
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
        }
        else if (Switches_Status & DOWN_MASK)
        {
            EditedTime[min2]--;
            if (EditedTime[min2] < 0)
            {
                EditedTime[min2] = 5;
            }
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
        }
        else if (Switches_Status & DOWN_MASK)
        {
            EditedTime[min1]--;
            if (EditedTime[min1] < 0)
            {
                EditedTime[min1] = 9;
            }
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
        }
        else if (Switches_Status & DOWN_MASK)
        {
            EditedTime[sec2]--;
            if (EditedTime[sec2] < 0)
            {
                EditedTime[sec2] = 5;
            }
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
        }
        else if (Switches_Status & DOWN_MASK)
        {
            EditedTime[sec1]--;
            if (EditedTime[sec1] < 0)
            {
                EditedTime[sec1] = 9;
            }
        }
    }
    CopyTime((u16 *)EditedTime,Time);
}

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

void EditTimeModeSM(void)
{
    CopyTime(Time,(u16 *)EditedTime);
    switch(Edit_state)
    {
        case Edit_Mode:
            trackCursor();
            validateCursorPos(cursor_pos[x_pos],cursor_pos[y_pos]);
            if((Switches_Status & EDIT_MASK) && validCursorPos)
            {
                Edit_state = Edit_Digit;
            }
            break;
        case Edit_Digit:
            EditTime();
            if(Switches_Status & OK_MASK)
            {
                Edit_state = Edit_Mode;
            }
            break;
    }
}
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
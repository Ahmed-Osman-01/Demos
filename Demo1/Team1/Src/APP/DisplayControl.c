/******************************************************************************
*
* Module: Display Control
*
* File Name: DisplayControl.c
*
* Description: Source file for the Display Control Module
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
#define EDIT_MASK           0x0040
#define MODE_MASK           0x0080
#define OK_MASK             0x1000
#define HOUR2_MASK          0x0040

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
enum states{
    DateTimeMode,
    StopWatchMode,
    EditTimeMode,
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

enum date {
    day1,
    day2,
    month1,
    month2,
    year        
};

enum cursorPos{
    x_pos,
    y_pos
};

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
u8 displayState = DateTimeMode;
extern void EditTimeModeSM (void);
extern u16 Switches_Status;
extern u16 Time[7];
extern u16 Date[5];
extern u16 stopWatchTime[7];
extern s8 cursor_pos[2];
extern u8 EditedTime[7];
extern u8 EditDigit_Flag;

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/
static void DisplayTime(void)
{
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,2);
    LCD_writeData(Time[hour2]+48);
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,3);
    LCD_writeData(Time[hour1]+48);
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,4);
    LCD_WriteString(":");
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,5);
    LCD_writeData(Time[min2]+48);
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,6);
    LCD_writeData(Time[min1]+48);
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,7);
    LCD_WriteString(":");
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,8);
    LCD_writeData(Time[sec2]+48);
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,9);
    LCD_writeData(Time[sec1]+48);
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,10);
    LCD_WriteString(".");
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,11);
    LCD_WriteNumber(Time[millisecond]);
}

static void DisplayDate(void)
{
    LCD_GotoDDRAM_XY(LCD_CURSOR_FIRST_LINE,3);
    LCD_writeData(Date[day2] + 48);
    LCD_GotoDDRAM_XY(LCD_CURSOR_FIRST_LINE,4);
    LCD_writeData(Date[day1] + 48);
    LCD_GotoDDRAM_XY(LCD_CURSOR_FIRST_LINE,5);
    LCD_WriteString("/");
    LCD_GotoDDRAM_XY(LCD_CURSOR_FIRST_LINE,6);
    LCD_writeData(Date[month2]+48);
    LCD_GotoDDRAM_XY(LCD_CURSOR_FIRST_LINE,7);
    LCD_writeData(Date[month1]+48);
    LCD_GotoDDRAM_XY(LCD_CURSOR_FIRST_LINE,8);
    LCD_WriteString("/");
    LCD_GotoDDRAM_XY(LCD_CURSOR_FIRST_LINE,9);
    LCD_WriteNumber(Date[year]);
}

static void DisplayStopWatch(void)
{
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,2);
    LCD_writeData(stopWatchTime[hour2]+48);
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,3);
    LCD_writeData(stopWatchTime[hour1]+48);
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,4);
    LCD_WriteString(":");
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,5);
    LCD_writeData(stopWatchTime[min2]+48);
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,6);
    LCD_writeData(stopWatchTime[min1]+48);
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,7);
    LCD_WriteString(":");
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,8);
    LCD_writeData(stopWatchTime[sec2]+48);
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,9);
    LCD_writeData(stopWatchTime[sec1]+48);
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,10);
    LCD_WriteString(".");
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,11);
    LCD_WriteNumber(stopWatchTime[millisecond]);
}

static void DisplayEditedTime(void)
{
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,2);
    LCD_writeData(EditedTime[hour2]+48);
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,3);
    LCD_writeData(EditedTime[hour1]+48);
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,4);
    LCD_WriteString(":");
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,5);
    LCD_writeData(EditedTime[min2]+48);
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,6);
    LCD_writeData(EditedTime[min1]+48);
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,7);
    LCD_WriteString(":");
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,8);
    LCD_writeData(EditedTime[sec2]+48);
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,9);
    LCD_writeData(EditedTime[sec1]+48);
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,10);
    LCD_WriteString(".");
    LCD_GotoDDRAM_XY(LCD_CURSOR_SECOND_LINE,11);
    LCD_WriteNumber(EditedTime[millisecond]);
}
/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

void displayControl (void)
{
    switch (displayState)
    {
        case DateTimeMode:
            DisplayTime();
            DisplayDate();
            if(Switches_Status &  EDIT_MASK)
            {
                LCD_writeCommand(LCD_CLEAR_DISPLAY);
                displayState = EditTimeMode;
                //Switches_Status &= ~EDIT_MASK; We don't need this line
            }
            else if(Switches_Status & MODE_MASK)
            {
                LCD_writeCommand(LCD_CLEAR_DISPLAY);
                displayState = StopWatchMode;
               
            }
            else{
                /*Do Nothing*/
            }
        break;
        case StopWatchMode:
            DisplayDate();
            DisplayStopWatch();
            if(Switches_Status &  MODE_MASK)
            {
                LCD_writeCommand(LCD_CLEAR_DISPLAY);
                displayState = DateTimeMode;
            }
        break;
        case EditTimeMode:
            LCD_writeCommand(LCD_DISPLAY_CURSOR_ON);
            LCD_writeCommand(LCD_CURSOR_BLINK_ON);
            DisplayEditedTime();
            LCD_GotoDDRAM_XY(cursor_pos[y_pos],cursor_pos[x_pos]);
            if(Switches_Status & OK_MASK)
            {
                LCD_writeCommand(LCD_CLEAR_DISPLAY);
                displayState = DateTimeMode;
            }
        break;
    }
}
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
#include <stdint.h>
#include <stdlib.h>
#include "HAL/LCD/LCD_HAL.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define EDIT_MASK           0x0040
#define MODE_MASK           0x0080
#define OK_MASK             0x100
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
uint8_t displayState = DateTimeMode;
extern volatile uint16_t Switches_Status;
extern volatile uint16_t Time[7];
extern volatile uint16_t Date[5];
extern volatile uint16_t stopWatchTime[7];
extern volatile int8_t cursor_pos[2];
extern volatile int16_t EditedTime[7];
extern volatile uint8_t EditDigit_Flag;
extern volatile uint8_t trackCursorFlag;
extern volatile uint8_t EditTimeFlag;
volatile uint8_t temp1[13];
volatile uint8_t temp2[11]; 
volatile uint8_t temp3[13];
volatile uint8_t temp4[13];
volatile uint8_t EDIT_DIGIT_FLAG = 1;
volatile uint8_t NOT_EDIT_DIGIT = 0;
enum Editstates{
    Edit_Mode,
    Edit_Digit,
};
volatile uint8_t Edit_state = Edit_Mode;
extern volatile uint8_t validCursorPos;
extern void trackCursor(void);
extern void validateCursorPos(uint8_t x_pos , uint8_t y_pos);
extern void CopyTime (volatile uint16_t* srcTime,volatile uint16_t* destTime);
extern void EditTime (void);
/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/
static void DisplayTime(void)
{
 // Buffer for conversion and separators
    temp1[0] = Time[hour2] + '0';
    temp1[1] = Time[hour1] + '0';
    temp1[2] = ':';
    temp1[3] = Time[min2] + '0';
    temp1[4] = Time[min1] + '0';
    temp1[5] = ':';
    temp1[6] = Time[sec2] + '0';
    temp1[7] = Time[sec1] + '0';
    temp1[8] = '.';
    temp1[9] = (Time[millisecond] / 100) + '0';
    temp1[10] = ((Time[millisecond] / 10) % 10) + '0';
    temp1[11] = (Time[millisecond] % 10) + '0';
    temp1[12] = '\0'; // Null terminator
    LCD_SetCursorPosAsync(1,2);
    LCD_WriteBufferAsync((const uint8_t* )(temp1), 12);
}

static void DisplayDate(void)
{
    // Buffer for conversion and separators
    temp2[0] = Date[day2] + '0';
    temp2[1] = Date[day1] + '0';
    temp2[2] = '/';
    temp2[3] = Date[month2] + '0';
    temp2[4] = Date[month1] + '0';
    temp2[5] = '/';
    temp2[6] = (Date[year] / 1000) + '0';
    temp2[7] = ((Date[year] / 100) % 10) + '0';
    temp2[8] = ((Date[year] / 10) % 10) + '0';
    temp2[9] = (Date[year] % 10) + '0';
    temp2[10] = '\0'; // Null terminator
    LCD_SetCursorPosAsync(0,3);
    LCD_WriteBufferAsync((const uint8_t* )(temp2), 10);
}

static void DisplayStopWatch(void)
{
    // Buffer for conversion and separators
    temp3[0] = stopWatchTime[hour2] + '0';
    temp3[1] = stopWatchTime[hour1] + '0';
    temp3[2] = ':';
    temp3[3] = stopWatchTime[min2] + '0';
    temp3[4] = stopWatchTime[min1] + '0';
    temp3[5] = ':';
    temp3[6] = stopWatchTime[sec2] + '0';
    temp3[7] = stopWatchTime[sec1] + '0';
    temp3[8] = '.';
    temp3[9] = (stopWatchTime[millisecond] / 100) + '0';
    temp3[10] = ((stopWatchTime[millisecond] / 10) % 10) + '0';
    temp3[11] = (stopWatchTime[millisecond] % 10) + '0';
    temp3[12] = '\0'; // Null terminator
    LCD_SetCursorPosAsync(1,2);
    LCD_WriteBufferAsync((const uint8_t* )(temp3), 11);
}

static void DisplayEditedTime(void)
{
     // Buffer for conversion and separators
    temp4[0] = EditedTime[hour2] + '0';
    temp4[1] = EditedTime[hour1] + '0';
    temp4[2] = ':';
    temp4[3] = EditedTime[min2] + '0';
    temp4[4] = EditedTime[min1] + '0';
    temp4[5] = ':';
    temp4[6] = EditedTime[sec2] + '0';
    temp4[7] = EditedTime[sec1] + '0';
    temp4[8] = '.';
    temp4[9] = (EditedTime[millisecond] / 100) + '0';
    temp4[10] = ((EditedTime[millisecond] / 10) % 10) + '0';
    temp4[11] = (EditedTime[millisecond] % 10) + '0';
    temp4[12] = '\0'; // Null terminator
    LCD_SetCursorPosAsync(1,2);
    LCD_WriteBufferAsync((const uint8_t* )(temp4), 11);
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
                LCD_ClearScreenAsync();
                displayState = EditTimeMode;
                //Switches_Status &= ~EDIT_MASK; We don't need this line
                CopyTime(Time,(uint16_t *)EditedTime);     
                LCD_SetCursorPosAsync(1,2);   
                DisplayEditedTime();        
                LCD_SetCursorPosAsync(cursor_pos[y_pos],cursor_pos[x_pos]);
                NOT_EDIT_DIGIT = 1;
            }
            else if(Switches_Status & MODE_MASK)
            {
                LCD_ClearScreenAsync();
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
                LCD_ClearScreenAsync();
                displayState = DateTimeMode;
            }
        break;
        case EditTimeMode:

            switch(Edit_state)
            {
                case Edit_Mode:
                    trackCursor();
                    if(trackCursorFlag)
                    {
                        LCD_SetCursorPosAsync(cursor_pos[y_pos],cursor_pos[x_pos]);
                        trackCursorFlag = 0;
                    }    
                    validateCursorPos(cursor_pos[x_pos],cursor_pos[y_pos]);
                    if((Switches_Status & EDIT_MASK) && validCursorPos && !NOT_EDIT_DIGIT)
                    {
                        Edit_state = Edit_Digit;
                    }
                    NOT_EDIT_DIGIT = 0;
                    break;
                case Edit_Digit:
                    EditTime();
                    if(EditTimeFlag)
                    {
                        LCD_SetCursorPosAsync(1,2);
                        LCD_WriteBufferAsync((const uint8_t*)(temp4), 11);
                        LCD_SetCursorPosAsync(cursor_pos[y_pos],cursor_pos[x_pos]);
                        EditTimeFlag = 0;
                    }
                    if(Switches_Status & OK_MASK)
                    {  
                        Edit_state = Edit_Mode;
                        EDIT_DIGIT_FLAG = 1;
                    }
                    break;
            }
            if((Switches_Status & OK_MASK) && !EDIT_DIGIT_FLAG)
            {
                LCD_ClearScreenAsync();
                displayState = DateTimeMode;
            }
            EDIT_DIGIT_FLAG = 0;
        break;
    }
}
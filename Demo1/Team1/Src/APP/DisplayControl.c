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
#include "HAL/LCD/LCD_DRIVER.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define EDIT_MASK           0x0004
#define MODE_MASK           0x0008
#define OK_MASK             0x0010

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
    year1,
    year2,
    year3,
    year4      
};

enum cursorPos{
    x_pos,
    y_pos
};
/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
uint8_t displayState = DateTimeMode;
extern volatile uint8_t Switches_Status_Control;
extern volatile uint16_t Time[7];
extern volatile uint16_t Date[8];
extern volatile uint16_t stopWatchTime[7];
extern volatile sint8_t cursor_pos[2];
extern volatile sint16_t EditedTime[7];
extern volatile sint16_t EditedDate[8];
extern volatile uint8_t EditDigit_Flag;
volatile uint8_t temp1[13];
volatile uint8_t temp2[11]; 
volatile uint8_t temp3[13];
volatile uint8_t temp4[13];
volatile uint8_t temp5[11];
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
extern void CopyTime (uint16_t* srcTime,uint16_t* destTime,uint8_t size);
extern void EditTime (void);
extern void EditDate(void);
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
    LCD_SetCursorPositionAsync(1,2,NULL_PTR);
    LCD_WriteStringAsync((uint8_t*)(temp1), 12, NULL_PTR);
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
    temp2[6] = Date[year4] + '0';
    temp2[7] = Date[year3] + '0';
    temp2[8] = Date[year2] + '0';
    temp2[9] = Date[year1] + '0';
    temp2[10] = '\0'; // Null terminator
    LCD_SetCursorPositionAsync(0,3,NULL_PTR);
    LCD_WriteStringAsync((uint8_t*)(temp2), 10, NULL_PTR);
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
    LCD_SetCursorPositionAsync(1,2,NULL_PTR);
    LCD_WriteStringAsync((uint8_t*)(temp3), 12, NULL_PTR);
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
    LCD_SetCursorPositionAsync(1,2,NULL_PTR);
    LCD_WriteStringAsync((uint8_t*)(temp4), 12, NULL_PTR);
}

static void DisplayEditedDate(void)
{
    temp5[0] = EditedDate[day2] + '0';
    temp5[1] = EditedDate[day1] + '0';
    temp5[2] = '/';
    temp5[3] = EditedDate[month2] + '0';
    temp5[4] = EditedDate[month1] + '0';
    temp5[5] = '/';
    temp5[6] = Date[year4] + '0';
    temp5[7] = Date[year3] + '0';
    temp5[8] = Date[year2] + '0';
    temp5[9] = Date[year1] + '0';
    temp5[10] = '\0'; // Null terminator
    LCD_SetCursorPositionAsync(0,3,NULL_PTR);
    LCD_WriteStringAsync((uint8_t*)(temp5), 10, NULL_PTR);
}



/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

void displayControl (void)
{
    switch (displayState)
    {
        case DateTimeMode:
            LCD_WriteCommandAsync(LCD_COMM_CURSOR_OFF, NULL_PTR);
            DisplayTime();   
            DisplayDate();                                                                              
            if(Switches_Status_Control &  EDIT_MASK)
            {
                LCD_ClearScreenAsync(NULL_PTR);
                displayState = EditTimeMode;
                CopyTime(Time,(uint16_t *)EditedTime,7);
                CopyTime(Date,(uint16_t *)EditedDate,8);     
                cursor_pos[x_pos] = 0;
                cursor_pos[y_pos] = 0;  
                DisplayEditedTime();   
                DisplayEditedDate();     
                //LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL_PTR);
                NOT_EDIT_DIGIT = 1;
            }
            else if(Switches_Status_Control & MODE_MASK)
            {
                LCD_ClearScreenAsync(NULL_PTR);
                displayState = StopWatchMode;
               
            }
            else{
                /*Do Nothing*/
            }
        break;
        case StopWatchMode:
            DisplayDate();
            DisplayStopWatch();
            if(Switches_Status_Control &  MODE_MASK)
            {
                LCD_ClearScreenAsync(NULL_PTR);
                displayState = DateTimeMode;
            }
        break;
        case EditTimeMode:
            LCD_WriteCommandAsync(LCD_COMM_BLINK_ON_CURSOR_ON, NULL_PTR);
            switch(Edit_state)
            {
                case Edit_Mode:
                    trackCursor();
                    //validateCursorPos(cursor_pos[x_pos],cursor_pos[y_pos]);
                    if((Switches_Status_Control & EDIT_MASK) /*&& validCursorPos*/ && !NOT_EDIT_DIGIT)
                    {
                        Edit_state = Edit_Digit;
                    }
                    NOT_EDIT_DIGIT = 0;
                    break;
                case Edit_Digit:
                    EditTime();
                    EditDate();
                    if(Switches_Status_Control & OK_MASK)
                    {  
                        Edit_state = Edit_Mode;
                        EDIT_DIGIT_FLAG = 1;
                    }
                    break;
            }
            if((Switches_Status_Control & OK_MASK) && !EDIT_DIGIT_FLAG)
            {
                LCD_ClearScreenAsync(NULL_PTR);
                displayState = DateTimeMode;
                cursor_pos[x_pos] = 0;
                cursor_pos[y_pos] = 0; 
            }
            EDIT_DIGIT_FLAG = 0;
        break;
    }
}
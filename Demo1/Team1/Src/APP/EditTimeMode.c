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
#include "HAL/LCD/LCD_DRIVER.h"


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

enum cursorPos{
    x_pos,
    y_pos
};

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
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
/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
uint8_t Valid_X_Pos_Values[6] = {HOUR2_POS_X,HOUR1_POS_X,MINUTES2_POS_X,MINUTES1_POS_X,SECONDS2_POS_X,SECONDS1_POS_X};

sint8_t cursor_pos[2] = {
    [x_pos] = 0,
    [y_pos] = 0,
};

sint16_t EditedTime[7]={
    [millisecond] = 0,
    [sec1] = 0,
    [sec2] = 0,
    [min1] = 0,
    [min2] = 0,
    [hour1] = 0,
    [hour2] = 0,
};

sint16_t EditedDate[8] ={
    [day1] = 7,
    [day2] = 1,
    [month1] = 4,
    [month2] = 0,
    [year1] = 4,
    [year2] = 2,
    [year3] = 0,
    [year4] = 2,
};

uint8_t validCursorPos = 0;
extern uint16_t Switches_Status;
extern uint16_t Time[7];
extern uint16_t Date[8];
extern volatile uint8_t temp4[13];
extern volatile uint8_t temp5[11];
/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
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
        LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
    }
    else if(Switches_Status & DOWN_MASK)
    {
        cursor_pos[y_pos]++;
        if(cursor_pos[y_pos] > 1)
        {
            cursor_pos[y_pos] = 0;
        }
        LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
    }
    else if(Switches_Status & LEFT_MASK)
    {
        cursor_pos[x_pos]--;
        if(cursor_pos[x_pos] < 0)
        {
            cursor_pos[x_pos] = 15;
        }
        LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
    }
    else if(Switches_Status & RIGHT_MASK)
    {
        cursor_pos[x_pos]++;
        if(cursor_pos[x_pos] > 15)
        {
            cursor_pos[x_pos] = 0;
        }
        LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
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

void CopyTime (uint16_t* srcTime, uint16_t* destTime, uint8_t size)
{
    uint8_t idx = 0;
    for(idx = 0; idx < size; idx++)
    {
        destTime[idx] = srcTime[idx];
    }
}

void EditTime (void)
{
    if(cursor_pos[x_pos] == HOUR2_POS_X && cursor_pos[y_pos] == TIME_POS_Y )
    {
        if(Switches_Status & UP_MASK)
        {
            EditedTime[hour2]++;
            if(EditedTime[hour2] > 2)
            {
                EditedTime[hour2] = 0;
            }
            temp4[0] = EditedTime[hour2] + '0';
            LCD_WriteStringAsync((&temp4[0]), 1, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
            
        }
        else if(Switches_Status & DOWN_MASK)
        {
            EditedTime[hour2]--;
            if(EditedTime[hour2] < 0)
            {
                EditedTime[hour2] = 2;
            }
            temp4[0] = EditedTime[hour2] + '0';
            LCD_WriteStringAsync((&temp4[0]), 1, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
            
        }
        else{
            /*Do Nothing*/
        }
    }
    else if (cursor_pos[x_pos] == HOUR1_POS_X && cursor_pos[y_pos] == TIME_POS_Y)
    {
        if (Switches_Status & UP_MASK)
        {
            EditedTime[hour1]++;
            if (EditedTime[hour1] > 9)
            {
                EditedTime[hour1] = 0;
            }
            else
            {
                /*Do Nothing*/
            }
            temp4[1] = EditedTime[hour1] + '0';
            LCD_WriteStringAsync(&temp4[1], 1, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
        }
        else if (Switches_Status & DOWN_MASK)
        {
            EditedTime[hour1]--;
            if (EditedTime[hour1] < 0)
            {
                EditedTime[hour1] = 9;
            }
            temp4[1] = EditedTime[hour1] + '0';
            LCD_WriteStringAsync(&temp4[1], 1, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
           
        }
    }
    else if (cursor_pos[x_pos] == MINUTES2_POS_X && cursor_pos[y_pos] == TIME_POS_Y)
    {
        if (Switches_Status & UP_MASK)
        {
            EditedTime[min2]++;
            if (EditedTime[min2] > 5)
            {
                EditedTime[min2] = 0;
            }
            temp4[3] = EditedTime[min2] + '0';
            LCD_WriteStringAsync((&temp4[3]), 1, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
            
        }
        else if (Switches_Status & DOWN_MASK)
        {
            EditedTime[min2]--;
            if (EditedTime[min2] < 0)
            {
                EditedTime[min2] = 5;
            }
            temp4[3] = EditedTime[min2] + '0';
            LCD_WriteStringAsync((&temp4[3]), 1, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
            
        }
    }
    else if (cursor_pos[x_pos] == MINUTES1_POS_X && cursor_pos[y_pos] == TIME_POS_Y)
    {
        if (Switches_Status & UP_MASK)
        {
            EditedTime[min1]++;
            if (EditedTime[min1] > 9)
            {
                EditedTime[min1] = 0;
            }
            temp4[4] = EditedTime[min1] + '0';
            LCD_WriteStringAsync((&temp4[4]), 1, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
            
        }
        else if (Switches_Status & DOWN_MASK)
        {
            EditedTime[min1]--;
            if (EditedTime[min1] < 0)
            {
                EditedTime[min1] = 9;
            }
            temp4[4] = EditedTime[min1] + '0';
            LCD_WriteStringAsync((&temp4[4]), 1, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
            
        }

    }
    else if (cursor_pos[x_pos] == SECONDS2_POS_X && cursor_pos[y_pos] == TIME_POS_Y)
    {
        if (Switches_Status & UP_MASK)
        {
            EditedTime[sec2]++;
            if (EditedTime[sec2] > 5)
            {
                EditedTime[sec2] = 0;
            }
            temp4[6] = EditedTime[sec2] + '0';
            LCD_WriteStringAsync((&temp4[6]), 1, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
            
        }
        else if (Switches_Status & DOWN_MASK)
        {
            EditedTime[sec2]--;
            if (EditedTime[sec2] < 0)
            {
                EditedTime[sec2] = 5;
            }
            temp4[6] = EditedTime[sec2] + '0';
            LCD_WriteStringAsync((&temp4[6]), 1, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
            
        }
    }
    else if (cursor_pos[x_pos] == SECONDS1_POS_X && cursor_pos[y_pos] == TIME_POS_Y)
    {
        if (Switches_Status & UP_MASK)
        {
            EditedTime[sec1]++;
            if (EditedTime[sec1] > 9)
            {
                EditedTime[sec1] = 0;
            }
            temp4[7] = EditedTime[sec1] + '0';
            LCD_WriteStringAsync((&temp4[7]), 1, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
            
        }
        else if (Switches_Status & DOWN_MASK)
        {
            EditedTime[sec1]--;
            if (EditedTime[sec1] < 0)
            {
                EditedTime[sec1] = 9;
            }
            temp4[7] = EditedTime[sec1] + '0';
            LCD_WriteStringAsync((&temp4[7]), 1, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
            
        }
    }
    CopyTime((uint16_t *)EditedTime,Time,7);
}

void EditDate(void)
{
    if(cursor_pos[x_pos] == DAY2_POS_X && cursor_pos[y_pos] == DATE_POS_Y)
    {
        if(Switches_Status & UP_MASK)
        {
            EditedDate[day2]++;
            if(EditedDate[day2] > 3)
            {
                EditedDate[day2] = 0;
            }
            else if(EditedDate[day2] == 3)
            {
                EditedDate[day1] = 1;
            }
            else if(EditedDate[day2] == 0)
            {
                EditedDate[day1] = 1;
            }
            temp5[1] = EditedDate[day1] + '0';
            temp5[0] = EditedDate[day2] + '0';
            LCD_SetCursorPositionAsync(0,3,NULL);
            LCD_WriteStringAsync(temp5, 11, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
        }
        else if (Switches_Status & DOWN_MASK)
        {
            EditedDate[day2]--;
            if(EditedDate[day2] < 0)
            {
                EditedDate[day2] = 3;
            }
            temp5[0] = EditedDate[day2] + '0';
            LCD_SetCursorPositionAsync(0,3,NULL);
            LCD_WriteStringAsync(temp5, 11, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
        }
    }
    else if(cursor_pos[x_pos] == DAY1_POS_X && cursor_pos[y_pos] == DATE_POS_Y)
    {
        if(Switches_Status & UP_MASK)
        {
            EditedDate[day1]++;
            if(EditedDate[day2] == 3)
            {
                if(EditedDate[day1] > 1)
                {
                    EditedDate[day1] = 0;
                }
            }
            else if (EditedDate[day2] == 0)
            {
                if(EditedDate[day1] > 9)
                {
                    EditedDate[day1] = 1;
                }
            }
            else if(EditedDate[day2] <= 3)
            {
                if(EditedDate[day1] > 9)
                {
                    EditedDate[day1] = 0;
                }
            }
            else{
                /*Do Nothing*/
            }
            temp5[1] = EditedDate[day1] + '0';
            LCD_SetCursorPositionAsync(0,3,NULL);
            LCD_WriteStringAsync((temp5), 11, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
        }
        else if (Switches_Status & DOWN_MASK)
        {
            EditedDate[day1]--;
            if(EditedDate[day2] == 3)
            {
                if(EditedDate[day1] < 0)
                {
                    EditedDate[day1] = 1;
                }
            }
            else if (EditedDate[day2] == 0)
            {
                if(EditedDate[day1] < 1)
                {
                    EditedDate[day1] = 1;
                }
            }
            else if(EditedDate[day2] <= 3)
            {
                if(EditedDate[day1] < 1)
                {
                    EditedDate[day1] = 0;
                }
            }
            else{
                /*Do Nothing*/
            }
            temp5[1] = EditedDate[day1] + '0';
            LCD_SetCursorPositionAsync(0,3,NULL);
            LCD_WriteStringAsync((temp5), 11, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
        }
    }
    else if(cursor_pos[x_pos] == MONTH2_POS_X && cursor_pos[y_pos] == DATE_POS_Y)
    {
        if(Switches_Status & UP_MASK)
        {
            EditedDate[month2]++;
            if(EditedDate[month2] > 1)
            {
                EditedDate[month2] = 0;
            }
            else if(EditedDate[month2] == 1)
            {
                EditedDate[month1] = 1;
            }
            temp5[4] = EditedDate[month1] + '0';
            temp5[3] = EditedDate[month2] + '0';
            LCD_SetCursorPositionAsync(0,3,NULL);
            LCD_WriteStringAsync((temp5), 11, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
        }
        else if (Switches_Status & DOWN_MASK)
        {
            EditedDate[month2]--;
            if(EditedDate[month2] < 0)
            {
                EditedDate[month2] = 1;
            }
            temp5[3] = EditedDate[month2] + '0';
            LCD_SetCursorPositionAsync(0,3,NULL);
            LCD_WriteStringAsync((temp5), 11, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
        }
    }
    else if(cursor_pos[x_pos] == MONTH1_POS_X && cursor_pos[y_pos] == DATE_POS_Y)
    {
        if(Switches_Status & UP_MASK)
        {
            EditedDate[month1]++;
            if(EditedDate[month2] == 1)
            {
                if(EditedDate[month1] > 2)
                {
                    EditedDate[month1] = 0;
                }
            }
            else if (EditedDate[month2] == 0)
            {
                if(EditedDate[month1] > 9)
                {
                    EditedDate[month1] = 1;
                }
            }
            temp5[4] = EditedDate[month1] + '0';
            LCD_SetCursorPositionAsync(0,3,NULL);
            LCD_WriteStringAsync(temp5, 11, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
        }
        else if (Switches_Status & DOWN_MASK)
        {
            EditedDate[month1]--;
            if (EditedDate[month2] == 0)
            {
                if(EditedDate[month1] < 1)
                {
                    EditedDate[month1] = 9;
                }
            }
            else{
                if(EditedDate[month1] < 0)
                {
                    EditedDate[month1] = 0;
                }
            }
            temp5[4] = EditedDate[month1] + '0';
            LCD_SetCursorPositionAsync(0,3,NULL);
            LCD_WriteStringAsync(temp5, 11, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
        }
    }
    else if(cursor_pos[x_pos] == YEAR4_POS_X && cursor_pos[y_pos] == DATE_POS_Y)
    {
        uint8_t year1 = (EditedDate[year4] / 1000);
        if(Switches_Status & UP_MASK)
        {
            EditedDate[year4]++;
            if(EditedDate[year4] > 9)
            {
                EditedDate[year4] = 0;
            }
            temp5[6] = EditedDate[year4] + '0';
            LCD_SetCursorPositionAsync(0,3,NULL);
            LCD_WriteStringAsync(temp5, 11, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
        }
        else if (Switches_Status & DOWN_MASK)
        {
            EditedDate[year4]--;
            if(EditedDate[year4] < 0)
            {
                EditedDate[year4] = 9;
            }
            temp5[6] = EditedDate[year4] + '0';
            LCD_SetCursorPositionAsync(0,3,NULL);
            LCD_WriteStringAsync(temp5, 11, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
        }
    }
    else if(cursor_pos[x_pos] == YEAR3_POS_X && cursor_pos[y_pos] == DATE_POS_Y)
    {
        if(Switches_Status & UP_MASK)
        {
            EditedDate[year3]++;
            if(EditedDate[year3] > 9)
            {
                EditedDate[year3] = 0;
            }
            temp5[7] = EditedDate[year3] + '0';
            LCD_SetCursorPositionAsync(0,3,NULL);
            LCD_WriteStringAsync(temp5, 11, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
        }
        else if (Switches_Status & DOWN_MASK)
        {
            EditedDate[year3]--;
            if(EditedDate[year3] < 0)
            {
                EditedDate[year3] = 9;
            }
            temp5[7] = EditedDate[year3] + '0';
            LCD_SetCursorPositionAsync(0,3,NULL);
            LCD_WriteStringAsync(temp5, 11, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
        }
    }
    else if(cursor_pos[x_pos] == YEAR2_POS_X && cursor_pos[y_pos] == DATE_POS_Y)
    {
        if(Switches_Status & UP_MASK)
        {
            EditedDate[year2]++;
            if(EditedDate[year2] > 9)
            {
                EditedDate[year2] = 0;
            }
            temp5[8] = EditedDate[year2] + '0';
            LCD_SetCursorPositionAsync(0,3,NULL);
            LCD_WriteStringAsync(temp5, 11, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
        }
        else if (Switches_Status & DOWN_MASK)
        {
            EditedDate[year2]--;
            if(EditedDate[year2] < 0)
            {
                EditedDate[year2] = 9;
            }
            temp5[8] = EditedDate[year2] + '0';
            LCD_SetCursorPositionAsync(0,3,NULL);
            LCD_WriteStringAsync(temp5, 11, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
        }
    }
    else if(cursor_pos[x_pos] == YEAR1_POS_X && cursor_pos[y_pos] == DATE_POS_Y)
    {
        if(Switches_Status & UP_MASK)
        {
            EditedDate[year1]++;
            if(EditedDate[year1] > 9)
            {
                EditedDate[year1] = 0;
            }
            temp5[9] = EditedDate[year1] + '0';
            LCD_SetCursorPositionAsync(0,3,NULL);
            LCD_WriteStringAsync(temp5, 11, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
        }
        else if (Switches_Status & DOWN_MASK)
        {
            EditedDate[year1]--;
            if(EditedDate[year1] < 0)
            {
                EditedDate[year1] = 9;
            }
            temp5[9] = EditedDate[year1] + '0';
            LCD_SetCursorPositionAsync(0,3,NULL);
            LCD_WriteStringAsync(temp5, 11, NULL);
            LCD_SetCursorPositionAsync(cursor_pos[y_pos],cursor_pos[x_pos], NULL);
        }
    }
    CopyTime((uint16_t *)EditedDate,Date,8);
}
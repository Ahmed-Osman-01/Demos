/******************************************************************************
*
* Module:
*
* File Name:
*
* Description: Source file for the ..... driver for STM32F401xC
*
* Author: Momen Elsayed Shaban
*
*******************************************************************************/

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include <stdint.h>

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define MILLIS_PER_SECOND       1000
#define HOURS_PER_DAY           24
#define START_MASK              0x10
#define STOP_MASK               0x20

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
extern volatile uint16_t Switches_Status;

volatile uint8_t start_flag = 0;

uint16_t stopWatchTime[7] = {
    [millisecond] = 0, 
    [sec1] = 0,
    [sec2] = 0,
    [min1] = 0,
    [min2] = 0,
    [hour1] = 0,
    [hour2] = 0,
}; 

/********************************************************************************************************/
/*********************************************Static Functions*******************************************/
/********************************************************************************************************/
static void StopWatchFeature(void)
{
    if(Switches_Status & STOP_MASK)
    {
        uint8_t idx = 0;
        for(idx = 0 ; idx < 7; idx++)
        {
            stopWatchTime[idx] = 0;
        }
        start_flag = 0;
    }
    else if ((Switches_Status & START_MASK) || start_flag)
    {
        start_flag = 1;
        stopWatchTime[millisecond] += 100;
        if (stopWatchTime[millisecond] >= MILLIS_PER_SECOND) 
        {
        stopWatchTime[sec1] += stopWatchTime[millisecond] / MILLIS_PER_SECOND;
        stopWatchTime[millisecond] -= MILLIS_PER_SECOND;
        }

    
        stopWatchTime[sec2] += stopWatchTime[sec1] / 10;
        stopWatchTime[sec1] %= 10;
        if (stopWatchTime[sec2] >= 6) 
        {
		    stopWatchTime[sec2] -= 6;
		    stopWatchTime[min1]++;
	    }

    
        stopWatchTime[min2] += stopWatchTime[min1] / 10;
        stopWatchTime[min1] %= 10;
        if (stopWatchTime[min2] >= 6)
        {
            stopWatchTime[min2] -= 6;
            stopWatchTime[hour1]++;
        }

    
        stopWatchTime[hour2] += stopWatchTime[hour1] / 10;
        stopWatchTime[hour1] %= 10;
        if (stopWatchTime[hour1] >= 4)
        {
            stopWatchTime[hour1] -= 4;
		    stopWatchTime[hour2] -= 2;
        }
    }
    else {
        /*Do Nothing*/
    }
}

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

void stopWatch(void)
{
    StopWatchFeature();
}

#if 0
else if (Switches_Status & STOP_MASK)
    {
        u8 idx = 0;
        for(idx = 0 ; idx < 7; idx++)
        {
                stopWatchTime[idx] = 0;
        }
        start_flag = 0;
        reset_counter++;
        if(reset_counter == 2)
        {
            
            reset_counter = 0;
            
        }
    }
#endif
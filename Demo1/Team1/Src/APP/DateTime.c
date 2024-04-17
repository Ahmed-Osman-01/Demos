/******************************************************************************
*
* Module: DateTime
*
* File Name: DateTime.c
*
* Description: Source file for the DateTime Module That Calculate the Date and the Time
*
* Author: Momen Elsayed Shaban
*
*******************************************************************************/

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "LIB/std_types.h"


/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define MILLIS_PER_SECOND       1000
#define HOURS_PER_DAY           24

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
/************************************************Types***************************************************/
/********************************************************************************************************/

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
volatile uint16_t Time[7] = {
    [millisecond] = 800, 
    [sec1] = 9,
    [sec2] = 5,
    [min1] = 9,
    [min2] = 5,
    [hour1] = 3,
    [hour2] = 2,
}; 

volatile uint16_t Date[8] = {
    [day1] = 1,
    [day2] = 3,
    [month1] = 2,
    [month2] = 1,
    [year1] = 4,
    [year2] = 2,
    [year3] = 0,
    [year4] = 2,
};

volatile uint8_t isDay = 0;

/********************************************************************************************************/
/*********************************************Static Functions*******************************************/
/********************************************************************************************************/
static void TimeFeature(void)
{
    Time[millisecond] += 100;

    
    if (Time[millisecond] >= MILLIS_PER_SECOND) 
    {
        Time[sec1] += Time[millisecond] / MILLIS_PER_SECOND;
        Time[millisecond] -= MILLIS_PER_SECOND;
    }

    
    Time[sec2] += Time[sec1] / 10;
    Time[sec1] %= 10;
    if (Time[sec2] >= 6) 
    {
		Time[sec2] -= 6;
		Time[min1]++;
	}

    
    Time[min2] += Time[min1] / 10;
    Time[min1] %= 10;
    if (Time[min2] >= 6)
    {
        Time[min2] -= 6;
        Time[hour1]++;
    }

    
    Time[hour2] += Time[hour1] / 10;
    Time[hour1] %= 10;
    if (Time[hour1] >= 4 && Time[hour2] == 2)
    {
        Time[hour1] -= 4;
		Time[hour2] -= 2;
        isDay = 1;
    }
}

static void DateFeature (void)
{
    if (isDay)
    {
        Date[day1]++;
        Date[day2] += Date[day1] / 10;
        Date[day1] %= 10;
        if (Date[day2] == 3 && Date[day1] == 2)
        {
            Date[day2] -= 3;
            Date[day1] = 1;
            Date[month1]++;
        }

        Date[month2] += Date[month1] / 10;
        Date[month1] %= 10;
        if (Date[month2] == 1 && Date[month1] == 3)
        {
            Date[month2] -= 1;
            Date[month1] = 1;
            Date[year1]++;
            if (Date[year1] >= 10)
            {
                Date[year1] = 0;
                Date[year2]++;
                if (Date[year2] >= 10)
                {
                    Date[year2] = 0;
                    Date[year3]++;
                    if (Date[year3] >= 10)
                    {
                        Date[year3] = 0;
                        Date[year4]++;
                        if (Date[year4] >= 10)
                        {
                            // Handle year overflow if needed
                        }
                    }
                }
            }
        }
		isDay = 0;
	}
}

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

/*Every 100ms*/
void DateTime(void)
{
    TimeFeature();
    DateFeature();
}


void setTime (uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds)
{
    Time[hour1] = hours % 10;
    Time[hour2] = hours / 10;
    Time[min1] = minutes % 10;
    Time[min2] = minutes / 10;
    Time[sec1] = seconds % 10;
    Time[sec2] = seconds / 10;
    Time[millisecond] = milliseconds;
}
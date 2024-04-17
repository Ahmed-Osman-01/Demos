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
#include <stdint.h>


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
    year        
};

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
volatile uint16_t Time[7] = {
    [millisecond] = 0, /*First Delay is 30 ms*/
    [sec1] = 0,
    [sec2] = 3,
    [min1] = 6,
    [min2] = 2,
    [hour1] = 0,
    [hour2] = 2,
}; 

volatile uint16_t Date[5] = {
    [day1] = 6,
    [day2] = 1,
    [month1] = 4,
    [month2] = 0,
    [year] = 2024  
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
    if (Time[hour1] >= 4)
    {
        Time[hour1] -= 4;
		Time[hour2] -= 2;
        isDay = 1;
    }
}

static void DateFeature (void)
{
    if(isDay)
	{
		Date[day1] ++;
		Date[day2] += Date[day1] / 10;
		Date[day1] %= 10;
		if(Date[day2] == 3 && Date[day1] == 1)
		{
			Date[day2] -= 3;
			Date[day1] = 1;
			Date[month1]++;
		}
		
		Date[month2] += Date[month1] / 10;
		Date[month1] %= 10;
		if(Date[month2] == 1 && Date[month1] == 3)
		{
			Date[month2] -= 1;
			Date[month1] = 1;
			Date[year]++;        
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

void setDate (uint16_t years, uint8_t months, uint8_t days)
{
    Date[year] = years;
    Date[month1] = months % 10;
    Date[month2] = months / 10;
    Date[day1] = days % 10;
    Date[day2] = days / 10;
}
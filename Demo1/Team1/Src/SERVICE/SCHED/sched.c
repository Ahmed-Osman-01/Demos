/******************************************************************************
 *
 * Module: Scheduler
 *
 * File Name: sched.c
 *
 * Description: Source file for the Scheduler driver for STM32F401xC
 *
 * Author: Momen Elsayed Shaban
 *
 *******************************************************************************/

#include "MCAL/RCC/RCC.h"
#include "MCAL/SYSTICK/SYSTICK.h"
#include "SERVICE/SCHED/Runnables_List.h"
#include "SERVICE/SCHED/sched.h"

#define SCHED_TICK_TIME_MS 2


typedef struct
{
    runnable_t *Runnable;
    uint32_t Remining_time;
} runInfo_t;


static volatile uint32_t pendingTicks = 0;

extern const runnable_t Runnables_List[_Runnables_Num];

volatile runInfo_t runnableInfoList[_Runnables_Num];


static void Sched()
{
	uint32_t iterator = 0;
	for(iterator = 0 ; iterator < _Runnables_Num;iterator++)
	{
		if((runnableInfoList[iterator].Runnable->callBackFn) && ((runnableInfoList[iterator].Remining_time) == 0))
		{
			runnableInfoList[iterator].Remining_time = runnableInfoList[iterator].Runnable->periodicityMS;
			runnableInfoList[iterator].Runnable->callBackFn();
		}
		runnableInfoList[iterator].Remining_time -= SCHED_TICK_TIME_MS;
	}
}

void Sched_TickCallBack(void)
{
	pendingTicks++;
}

void Sched_Init()
{
	uint32_t iterator = 0;
	SYSTICK_setTimeMS(SCHED_TICK_TIME_MS);
	SYSTICK_setCallBack(Sched_TickCallBack, 0);
	for(iterator = 0 ; iterator < _Runnables_Num;iterator++)
	{
		runnableInfoList[iterator].Runnable = &Runnables_List[iterator];
		runnableInfoList[iterator].Remining_time = Runnables_List[iterator].firstDelayMS;
	}

}

void Sched_Start()
{
	SYSTICK_start(SYSTICK_CLK_AHB);
	while(1)
	{
		if(pendingTicks)
		{
			pendingTicks--;
			Sched();
		}
	}
}
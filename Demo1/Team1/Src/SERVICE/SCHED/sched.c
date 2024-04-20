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


static volatile uint32_t pendingTicks = 0;

extern const runnable_t Runnables_List[_Runnables_Num];

static void Sched()
{
	uint32_t iterator = 0;
	static uint32_t timeStamp = 0;
	for(iterator = 0 ; iterator < _Runnables_Num;iterator++)
	{
		if((Runnables_List[iterator].callBackFn) && ((timeStamp % Runnables_List[iterator].periodicityMS) == 0))
		{
			Runnables_List[iterator].callBackFn();
		}
	}
	timeStamp+= SCHED_TICK_TIME_MS;
}

void Sched_TickCallBack(void)
{
	pendingTicks++;
}

void Sched_Init()
{
	SYSTICK_setTimeMS(SCHED_TICK_TIME_MS);
	SYSTICK_setCallBack(Sched_TickCallBack, 0);
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



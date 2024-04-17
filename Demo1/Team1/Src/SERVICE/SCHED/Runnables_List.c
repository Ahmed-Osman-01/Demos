/*
 * Runnables_List.c
 *
 *  Created on: 11 Mar 2024
 *      Author: Momen El Sayed
 */

#include "SERVICE/SCHED/Runnables_List.h"
#include "SERVICE/SCHED/sched.h"

extern void LCD_Runnable(void);
extern void Keypad_Runnable (void);
extern void readSwitches (void);
extern void DateTime (void);
extern void displayControl (void);
extern void stopWatch(void);
extern void LIN_MasterTask(void);
extern void DataTransferTask(void);


const runnable_t Runnables_List[_Runnables_Num] =
{
		[Keypad_R] = {
			.name = "Keypad runnable for checking the pressed key",
			.periodicityMS = 6,
			//.firstDelayMS = 30,
			.callBackFn = &Keypad_Runnable
		},
		[readSwitches_R] = {
			.name = "ReadSwitches From Keypad and Assign to a variable",
			.periodicityMS = 60,
			//.firstDelayMS = 60,
			.callBackFn = &readSwitches
		},
		[LCD_R] = {
			.name = "LCD Runnable for LCD intialization and control",
			.periodicityMS = 2,
			//.firstDelayMS = 0,
			.callBackFn = &LCD_Runnable,
		},
		[DateTime_R]={
			.name = "Calculate Date and Time",
			.periodicityMS = 100,
			//.firstDelayMS = 0,
			.callBackFn = &DateTime
		},
		[StopWatch_R]={
			.name = "For stopwatch increment",
			.periodicityMS = 100,
			//.firstDelayMS = 0,
			.callBackFn = &stopWatch
		},
		[DisplayControl_R]=
		{
			.name = "Displaying on LCD",
			.periodicityMS = 200,
			//.firstDelayMS = 0,
			.callBackFn = &displayControl
		},
		[LIN_MasterTask_R]={
			.name = "Lin Master Task",
			.periodicityMS = 2,
			//.firstDelayMS = 0,
			.callBackFn = &LIN_MasterTask
		},
		[Data_Transfer_R]={
			.name = "TransferData",
			.periodicityMS = 60,
			//.firstDelayMS = 0,
			.callBackFn = &DataTransferTask
		}
};


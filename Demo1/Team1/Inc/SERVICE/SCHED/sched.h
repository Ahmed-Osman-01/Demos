/*
 * sched.h
 *
 *  Created on: 11 Mar 2024
 *      Author: Momen El Sayed
 */

#ifndef SCHED_H_
#define SCHED_H_

#include "LIB/std_types.h"

/*******************************************************************************
 *                                Type Decelerations                           *
 *******************************************************************************/
typedef void (*runnableCB_t) (void);

typedef struct{
	char* name;
	uint32_t periodicityMS;
	uint32_t firstDelayMS;
	runnableCB_t callBackFn;
}runnable_t;


/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
/*****************************************************
 * Function: Sched_Init
 * Description: Initializes the scheduler. Sets the SysTick timer period based on
 *              the defined tick time and assigns a callback function to be executed
 *              on every SysTick timer tick.
 *
 * Parameters:
 *   - None
 *
 * Return:
 *   - None
 *
 * Usage:
 *   Sched_Init(); // Call this function at the start to initialize the scheduler.
 *
 *****************************************************/
void Sched_Init();

/*****************************************************
 * Function: Sched_Start
 * Description: Starts the scheduler by enabling the SysTick timer and continuously
 *              checks for and executes scheduled tasks in an infinite loop. This
 *              function assumes the SysTick timer is configured to interrupt at a
 *              fixed interval, incrementing the pendingTicks counter.
 *
 * Parameters:
 *   - None
 *
 * Return:
 *   - None
 *
 * Usage:
 *   Sched_Start(); // Call this function after initializing the scheduler to start task scheduling.
 *
 * Notes:
 *   - This function starts the SysTick timer with the system clock (AHB) as its source.
 *   - The infinite loop within this function continuously checks for the presence of
 *     pending ticks.
 *****************************************************/
void Sched_Start();

#endif /* SCHED_H_ */

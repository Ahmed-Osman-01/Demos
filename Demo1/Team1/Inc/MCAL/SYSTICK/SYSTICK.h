/*
 * SYSTICK.h
 *
 *  Created on: 11 Mar 2024
 *      Author: Momen El Sayed
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "LIB/std_types.h"
#include "SYSTICK_Cfg.h"
/*******************************************************************************
 *                                Type Decelerations                           *
 *******************************************************************************/

#define SYSTICK_CLK_AHB 			0x00000007
#define SYSTICK_CLK_AHB_DIV_8 		0x00000003

typedef enum{
	SYSTICK_OK,
	SYSTICK_InvalidClkSrcValue,
	SYSTICK_InvalidTicksValue,
	SYSTICK_InvalidCallBackIndex,
	SYSTICK_NullPtr
}SYSTICK_ErrorStatus_t;

typedef void (*SYSTICKCallBackFn_t) (void);


/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
/*****************************************************
 * Function: SYSTICK_start
 * Description: Configures and starts the SysTick timer with the specified clock source.
 *
 * Parameters:
 *   - SYSTICK_Clk: Clock source for SysTick timer.
 *                  - SYSTICK_CLK_AHB: Use AHB clock as the source.
 *                  - SYSTICK_CLK_AHB_DIV_8: Use AHB/8 clock as the source.
 *
 * Return:
 *   - SYSTICK_ErrorStatus_t: Status of the operation.
 *     - SYSTICK_OK: Operation successful.
 *     - SYSTICK_InvalidClkSrcValue: Returned if an invalid clock source value is provided.
 *
 * Usage:
 *   SYSTICK_ErrorStatus_t status = SYSTICK_start(SYSTICK_CLK_AHB);
 *   if(status == SYSTICK_OK) {
 *     // SysTick timer started successfully
 *   }
 *
 * Notes:
 *   - This function configures and initiates the SysTick timer with the specified clock source.
 *   - The SysTick timer is often used for generating time delays.
 *****************************************************/
SYSTICK_ErrorStatus_t SYSTICK_start(uint32_t SYSTICK_Clk);

/*****************************************************
 * Function: SYSTICK_setTimeMS
 * Description: Sets the number of ticks for the SysTick timer.
 *
 * Parameters:
 *   - SYSTICK_ticks: Number of ticks to be set for the SysTick timer.
 *
 * Return:
 *   - SYSTICK_ErrorStatus_t: Status of the operation.
 *     - SYSTICK_OK: Operation successful.
 *     - SYSTICK_InvalidTicksValue: Returned if an invalid number of ticks is provided.
 *
 * Usage:
 *   SYSTICK_ErrorStatus_t status = SYSTICK_setTimeMS(1000);
 *   if(status == SYSTICK_OK) {
 *     // SysTick ticks set successfully
 *   }
 *
 * Notes:
 *   - This function sets the number of ticks for the SysTick timer, determining the period of interrupts.
 *   - The provided number of ticks should not exceed the maximum load value (SYSTICK_MAX_LOAD_VAL).
 *****************************************************/
SYSTICK_ErrorStatus_t SYSTICK_setTimeMS(uint32_t SYSTICK_ticks);

/*****************************************************
 * Function: SYSTICK_Stop
 * Description: Stops the SysTick timer.
 *
 * Parameters: None
 *
 * Return:
 *   - SYSTICK_ErrorStatus_t: Status of the operation.
 *     - SYSTICK_OK: Operation successful.
 *
 * Usage:
 *   SYSTICK_ErrorStatus_t status = SYSTICK_Stop();
 *   if(status == SYSTICK_OK) {
 *     // SysTick timer stopped successfully
 *   }
 *
 *****************************************************/
SYSTICK_ErrorStatus_t SYSTICK_Stop();

/*****************************************************
 * Function: SYSTICK_setCallBack
 * Description: Sets the callback function for a specific SysTick timer event.
 *
 * Parameters:
 *   - SYSTICK_CBF: Pointer to the callback function to be set.
 *   - req_Index: Index representing the SysTick timer event for which the callback is set.
 *
 * Return:
 *   - SYSTICK_ErrorStatus_t: Status of the operation.
 *     - SYSTICK_OK: Operation successful.
 *     - SYSTICK_InvalidCallBackIndex: Returned if an invalid callback index is provided.
 *     - SYSTICK_NullPtr: Returned if the provided callback function pointer is NULL.
 *
 * Usage:
 *   SYSTICK_ErrorStatus_t status = SYSTICK_setCallBack(MyCallbackFunction, 0);
 *   if(status == SYSTICK_OK) {
 *     // Callback function set successfully for SysTick event at index 0
 *   }
 *
 * Notes:
 *   - This function associates a callback function with a specific SysTick timer event, allowing custom actions
 *     to be taken when the corresponding event occurs.
 *   - The callback function pointer must be valid (not NULL), and the index should be within the valid range.
 *****************************************************/
SYSTICK_ErrorStatus_t SYSTICK_setCallBack(SYSTICKCallBackFn_t SYSTICK_CBF, uint8_t req_Index);


#endif /* SYSTICK_H_ */

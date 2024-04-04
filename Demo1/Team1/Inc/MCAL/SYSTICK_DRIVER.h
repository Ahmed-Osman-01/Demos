#ifndef SYSTICK_DRIVER_
#define SYSTICK_DRIVER_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "STD_Types.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define SYSTICK_IRQ_ENABLE 0x00000002
#define SYSTICK_IRQ_DISABLE 0x00000000
#define SYSTICK_CLK_AHB 0x00000004
#define SYSTICK_CLK_AHB_DIV_8 0x00000000

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
typedef void (*SYSTICK_CBF_t)(void);

/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

/*
 * use this function to start the operation of the Systick timer
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status SYSTICK_CTRL_StartTimer();

/*
 * use this function to control the Systick interrupt trigger after the requested time period
 * parameters:
 * SYSTICK_IRQ -> SYSTICK_IRQ_ENABLE, SYSTICK_IRQ_DISABLE
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status SYSTICK_CTRL_Interrupt(u32_t SYSTICK_IRQ);

/*
 * use this function to control choose the input clock to the Systick timer
 * parameters:
 * SYSTICK_CLK -> SYSTICK_CLK_AHB, SYSTICK_CLK_AHB_DIV_8
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status SYSTICK_CFG_CLKSource(u32_t SYSTICK_CLK);

/*
 * use this function to set the current value of the Systick timer VAL register
 * parameters:
 * Curr_Val -> the desired value
 * - must not exceed 0x00FF FFFF -
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status SYSTICK_SET_CurrentVal(u32_t Curr_Val);

/*
 * use this function to read the current value of the Systick timer VAL register
 * parameters:
 * Curr_Val -> the address of the variable to store the current value
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status SYSTICK_GET_CurrentVal(u32_t *Curr_Val);

/*
 * use this function to assign a function to be executed after the designated time period passes
 * parameters:
 * SYSTICK_CBF -> the address of the function to be executed
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status SYSTICK_SET_CallBack(SYSTICK_CBF_t SYSTICK_CBF);

/*
 * use this function to set the desired time period for the Systick timer
 * parameters:
 * TimeMs -> the desired time period in Ms
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status SYSTICK_SET_TimeTicksMs(u32_t TimeMs);

#endif // SYSTICK_DRIVER_
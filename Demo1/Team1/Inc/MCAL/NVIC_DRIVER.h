#ifndef NVIC_DRIVER_
#define NVIC_DRIVER_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "STD_Types.h"
#include "STM32f401CC_IRQ.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

#define NVIC_STATUS_NOT_ACTIVE 0x00000000
#define NVIC_STATUS_ACTIVE 0x00000001
#define NVIC_SUBGROUP_NUM 1
#define SUBGROUPBIT_NONE 0x00000000
#define SUBGROUPBIT_ONE 0x00000400
#define SUBGROUPBIT_TWO 0x00000500
#define SUBGROUPBIT_THREE 0x00000600
#define SUBGROUPBIT_FOUR 0x00000700

/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

/*
 * use this function to Enable an IRQ
 * parameters:
 * NVIC_IRQ -> the ID of the requested interrupt from the target IRQ_ID list
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status NVIC_CTRL_EnableIRQ(IRQ_ID_t NVIC_IRQ);

/*
 * use this function to Disable an IRQ
 * parameters:
 * NVIC_IRQ -> the ID of the requested interrupt from the target IRQ_ID list
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status NVIC_CTRL_DisableIRQ(IRQ_ID_t NVIC_IRQ);

/*
 * use this function to Set the pending bit corresponding to an IRQ
 * parameters:
 * NVIC_IRQ -> the ID of the requested interrupt from the target IRQ_ID list
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status NVIC_CTRL_SetIRQPending(IRQ_ID_t NVIC_IRQ);

/*
 * use this function to Clear the pending bit correspondin to an IRQ
 * parameters:
 * NVIC_IRQ -> the ID of the requested interrupt from the target IRQ_ID list
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status NVIC_CTRL_ClearIRQPending(IRQ_ID_t NVIC_IRQ);

/*
 * use this function to Generate a software interrupt corresponding to an IRQ
 * parameters:
 * NVIC_IRQ -> the ID of the requested interrupt from the target IRQ_ID list
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status NVIC_CTRL_GenerateSWI(IRQ_ID_t NVIC_IRQ);

/*
 * use this function to read the status of an IRQ
 * parameters:
 * NVIC_IRQ -> the ID of the requested interrupt from the target IRQ_ID list
 * Status -> address of the var to hold the status -> NVIC_STATUS_NOT_ACTIVE, NVIC_STATUS_ACTIVE
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status NVIC_Get_ActiveStatus(IRQ_ID_t NVIC_IRQ, u32_t *Status);

/*
 * use this function to Set the priority levels corresponding to an IRQ
 * parameters:
 * NVIC_IRQ -> the ID of the requested interrupt from the target IRQ_ID list
 * PreemptPri -> value of the Preemption priority level
 * SubGroupPri -> value of the Subgroup priority level
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status NVIC_CFG_SetPriority(IRQ_ID_t NVIC_IRQ, u8_t PreemptPri, u8_t SubGroupPri);

/*
 * use this function to read the priority of an IRQ
 * parameters:
 * NVIC_IRQ -> the ID of the requested interrupt from the target IRQ_ID list
 * Priority -> address of the var to hold the Priority
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status NVIC_Get_Priority(IRQ_ID_t NVIC_IRQ, u32_t *Priority);

/*
 * use this function to Set the priority bits to specify the subgroup and group priority levels
 * parameters:
 * SubGroupBit -> SUBGROUPBIT_NONE, SUBGROUPBIT_ONE, SUBGROUPBIT_TWO, SUBGROUPBIT_THREE, SUBGROUPBIT_FOUR
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status NVIC_CFG_SetSubGroupBits(u32_t SubGroupBit);

#endif // NVIC_DRIVER_
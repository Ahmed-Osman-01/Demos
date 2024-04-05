#ifndef DRIVER_INCLUDE_NVIC_H_
#define DRIVER_INCLUDE_NVIC_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include <stdint.h>
#include "STM32F401CC_NVIC_OPTIONS.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
/**
 * These are the expected return from function NVIC_GetPendingIRQ
*/
#define IS_NOT_PENDING 0
#define IS_PENDING     1

#define IS_NOT_ACTIVE 0
#define IS_ACTIVE     1

#define PRIORITY_16_LEVELS 0x00000300
#define PRIORITY_8_LEVELS  0x00000400
#define PRIORITY_4_LEVELS  0x00000500
#define PRIORITY_2_LEVELS  0x00000600
#define PRIORITY_NO_LEVELS 0x00000700

#define GROUP_0            0
#define GROUP_1            1
#define GROUP_2            2
#define GROUP_3            3
#define GROUP_4            4
#define GROUP_5            5
#define GROUP_6            6
#define GROUP_7            7
#define GROUP_8            8
#define GROUP_9            9
#define GROUP_10           10
#define GROUP_11           11
#define GROUP_12           12
#define GROUP_13           13
#define GROUP_14           14
#define GROUP_15           15

#define SUBGROUP_0         0
#define SUBGROUP_1         1
#define SUBGROUP_2         2
#define SUBGROUP_3         3
#define SUBGROUP_4         4
#define SUBGROUP_5         5
#define SUBGROUP_6         6
#define SUBGROUP_7         7
#define SUBGROUP_8         8
#define SUBGROUP_9         9
#define SUBGROUP_10        10
#define SUBGROUP_11        11
#define SUBGROUP_12        12
#define SUBGROUP_13        13
#define SUBGROUP_14        14
#define SUBGROUP_15        15
/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
typedef enum{

    NVIC_isOk,
    NVIC_isNotOk,
    NVIC_WrongPriority_Grouping,
    NVIC_WRONGIRQ,
    NVIC_WrongPriority,
    NVIC_NULLPTR,
    NVIC_WrongPriorityGrouping,
    NVIC_WrongGroup,
    NVIC_WrongSubGroup

} NVIC_errorStatus_t;


/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/



/**
 * @brief Enable IRQn
 * 
 */
NVIC_errorStatus_t NVIC_EnableIRQ(uint32_t IRQn); 

/**
 * @brief Disable IRQn
 * 
 * @param IRQn 
 */
NVIC_errorStatus_t NVIC_DisableIRQ(uint32_t IRQn); 



/**
 * @brief  Set IRQn pending
 * 
 * @param IRQn 
 */
NVIC_errorStatus_t NVIC_SetPendingIRQ (uint32_t IRQn);

/**
 * @brief Clear IRQn pending status
 * 
 * @param IRQn 
 */
NVIC_errorStatus_t NVIC_ClearPendingIRQ (uint32_t IRQn); 

/**
 * @brief Return true (IRQ-Number) if IRQn is pending
 * 
 * @param IRQn 
 * @param pendingState 
 */
NVIC_errorStatus_t NVIC_GetPendingIRQ (uint32_t IRQn, uint32_t* pendingState); 

/**
 * @brief Return the IRQ number of the active interrupt
 * 
 * @param IRQn 
 * @param activeState 
 * @return NVIC_errorStatus_t 
 */
NVIC_errorStatus_t NVIC_GetActive (uint32_t IRQn, uint32_t* activeState); 

/**
 * @brief Set priority for IRQn
 * 
 * @param IRQn 
 * @param priority 
 */
NVIC_errorStatus_t NVIC_SetPriority (uint32_t IRQn, uint32_t Group, uint32_t SubGroup); 

/**
 * @brief Read priority of IRQn
 * 
 * @param IRQn 
 * @param priority 
 * @return NVIC_errorStatus_t 
 */
NVIC_errorStatus_t NVIC_GetPriority (uint32_t IRQn, uint32_t* priority); 

/**
 * @brief  Set the priority grouping
 * 
 */
NVIC_errorStatus_t NVIC_SetPriorityGrouping(uint32_t priority_grouping);
/**
 * @brief Reset the system
 * 
 */
NVIC_errorStatus_t NVIC_SystemReset (void); 

/**
 * @brief 
 * 
 * @param IRQn 
 */
NVIC_errorStatus_t NVIC_GenerateSWINT(uint32_t IRQn);

#endif // DRIVER_INCLUDE_NVIC_H_
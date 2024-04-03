/*
 * SWITCH.h
 *
 *  Created on: Feb 18, 2024
 *      Author: Osman
 */


#ifndef HAL_SWITCH_SWITCH_H_
#define HAL_SWITCH_SWITCH_H_

/* ============================================================================ */
/*                                  INCLUDEDS                           	    */
/* ============================================================================ */

#include "HAL/SWITCH/SWITCH_config.h"


/* ============================================================================ */
/*                                   MACROS                                    	*/
/* ============================================================================ */

#define SWITCH_STATE_RELEASED       0x00000000
#define SWITCH_STATE_PRESSED        0x00000001
#define SWITCH_STATE_HOLD           0x00000002

#define SWITCH_CONNECTION_FLOATING  0x00000000
#define SWITCH_CONNECTION_PULLUP    0x00000008
#define SWITCH_CONNECTION_PULLDOWN  0x00000010


/* ============================================================================ */
/*                                  	TYPES                           	*/
/* ============================================================================ */
typedef struct
{
    void * Port;
    u32 Pin;
    u8 Connection;
}SWITCH_Config_t;

typedef enum
{
    SWITCH_OK,
    SWITCH_NOK,
    SWITCH_INVALID_CONNECTION,
    SWITCH_INVALID_SWITCH_ID,
    SWITCH_NULLPTR,
}SWITCH_ErrorStatus_t;



/* ============================================================================ */
/*                                  	PROTOTYPES				                */
/* ============================================================================ */


/**
 * @brief Initializes all the switches configured in configure files
 * 
 * @return SWITCH_ErrorStatus_t Switch module error status
 * 
 * @note Must configure the Switches in SWITCH_config.h and SWITCH_config.c firts
 */
SWITCH_ErrorStatus_t SWITCH_Init(void);


/**
 * @brief Returns the State of the switch (Pressed or Released)
 * 
 * @param Copy_Switch The switch ID as configured in SWITCH_config.h
 * @param Add_SwitchState Returned swtich state
 * @return SWITCH_ErrorStatus_t Switch module error status
 * @note Must Initialize the Switches first using SWITCH_Init() function
 */
SWITCH_ErrorStatus_t SWITCH_GetSwitchState(SWITCH_ID_t Copy_SwitchID, u8 * Add_SwitchState);

#endif /* HAL_SWITCH_SWITCH_H_ */

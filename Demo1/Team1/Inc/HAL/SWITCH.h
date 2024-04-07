/******************************************************************************
 *
 * Module: Switch
 *
 * File Name: SWITCH.h
 *
 * Description: Header file for the SWITCH driver for STM32F401xC
 *
 * Author: Momen Elsayed Shaban
 *
 *******************************************************************************/
#ifndef SWITCH_H_
#define SWITCH_H_

#include "SWITCH_Cfg.h"
#include "std_types.h"

/*******************************************************************************
 *                                Type Decelerations                           *
 *******************************************************************************/
#define SWITCH_PULL_UP          0x00000008      /*same values as GPIO Driver*/
#define SWITCH_PULL_DOWN        0x00000010
#define SWITCH_PRESSED          0x00000001
#define SWITCH_RELEASED         0x00000000

typedef enum{
    SWITCH_OK,
    SWITCH_InvalidDir,
    SWITCH_InvalidName,
    SWITCH_NullPtr
}SWITCH_ErrorStatus_t;


typedef struct{
    void* port;
    u32 pin;
    u32 dir;
}SWITCH_Cfg_t;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
/*****************************************************
 * Function: SWITCH_Init
 * Description: Initializes the specified switches based on the provided configuration.
 *
 * Return:
 *   - SWITCH_ErrorStatus_t: An error status indicating the success or failure of the switch initialization.
 *
 * Errors:
 *   - SWITCH_InvalidDir: Returned if an invalid switch direction is provided.
 *   - SWITCH_OK: Returned if the switch initialization is successful.
 *
 * Usage: SWITCH_ErrorStatus_t error = SWITCH_Init();
 * Example: Initialize switches according to the predefined configuration.
 *
 * Notes:
 *   - The SWITCH_Init function initializes switches based on the SWITCH_Cfg_t configuration.
 *****************************************************/
SWITCH_ErrorStatus_t SWITCH_Init();

/*****************************************************
 * Function: SWITCH_getState
 * Description: Reads the current state of the specified switch.
 *
 * Parameters:
 *   - Switch_Name: The index of the switch in the configuration array.
 *   - Switch_state: Pointer to store the read state of the switch.
 *
 * Return:
 *   - SWITCH_ErrorStatus_t: An error status indicating the success or failure of the switch state read.
 *
 * Errors:
 *   - SWITCH_InvalidName: Returned if an invalid switch index is provided.
 *   - SWITCH_NullPtr: Returned if the Switch_state pointer is NULL.
 *   - SWITCH_OK: Returned if the switch state read is successful.
 *
 * Usage: SWITCH_ErrorStatus_t error; u8 state; error = SWITCH_getState(SwitchName, &state);
 * Example: Read the state of the first switch and store it in the 'state' variable.
 *
 * Notes:
 *   - The function reads the state using GPIO_getPinValue and adjusts it based on the switch direction.
 *****************************************************/
SWITCH_ErrorStatus_t SWITCH_getState(u32 Switch_Name,u8* Switch_state);

#endif /*SWITCH_H_*/

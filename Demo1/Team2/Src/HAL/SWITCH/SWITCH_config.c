/*
 * SWITCH_config.c
 *
 *  Created on: Feb 18, 2024
 *      Author: Osman
 */

#include "LIB/STD_TYPES.h"
#include "HAL/SWITCH/SWITCH.h"
#include "MCAL/GPIO/GPIO.h"

const SWITCH_Config_t Switches[_SWITCH_NUM] = {
    [SWITCH_ONE] = {
        .Port = GPIO_PORT_A,
        .Pin = GPIO_PIN_3,
        .Connection = SWITCH_CONNECTION_PULLUP
    },

    [SWITCH_TWO] = {
        .Port = GPIO_PORT_A,
        .Pin = GPIO_PIN_7,
        .Connection = SWITCH_CONNECTION_PULLDOWN
    },

};

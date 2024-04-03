/*
 * SWITCH_config.h
 *
 *  Created on: Feb 18, 2024
 *      Author: Osman
 */

#ifndef HAL_SWITCH_SWITCH_CONFIG_H_
#define HAL_SWITCH_SWITCH_CONFIG_H_


/* Add Or Remove Switches as desired here to be configured in SWITCH_config.c file */
typedef enum
{
    SWITCH_ONE,
    SWITCH_TWO,


    
/* DON'T REMOVE _SWITCH_NUM OR ADD ANY SWITCHES AFTER IT */
    _SWITCH_NUM,
}SWITCH_ID_t;

#endif /* HAL_SWITCH_SWITCH_CONFIG_H_ */

/*
 * LCD_config.c
 *
 *  Created on: Mar 22, 2024
 *      Author: Osman
 */


/* ============================================================================ */
/*                                  INCLUDEDS                           	    */
/* ============================================================================ */

#include "LIB/STD_TYPES.h"
#include "MCAL/GPIO/GPIO.h"
#include "HAL/LCD/LCD_priv.h"
#include "HAL/LCD/LCD_CFG.h"
#include "HAL/LCD/LCD_HAL.h"



/**
 * The configuration of the following array is reflected from the configuration
 * in the LCD_config.h file.
 * In order to configure the Ports and Pins of the LCD pins, you should configure
 * them in LCD_config.h file and leave the following array as it is.
 * 
 */

#if LCD_MODE == LCD_8_BIT_MODE

const LCD_Pin_t LCD_Pins[8] = 
{
    [0] = {.Port = LCD_PORT_D0, .Pin = LCD_PIN_D0},     /* LCD_D0 */
    [1] = {.Port = LCD_PORT_D1, .Pin = LCD_PIN_D1},     /* LCD_D1 */
    [2] = {.Port = LCD_PORT_D2, .Pin = LCD_PIN_D2},     /* LCD_D2 */
    [3] = {.Port = LCD_PORT_D3, .Pin = LCD_PIN_D3},     /* LCD_D3 */
    [4] = {.Port = LCD_PORT_D4, .Pin = LCD_PIN_D4},     /* LCD_D4 */
    [5] = {.Port = LCD_PORT_D5, .Pin = LCD_PIN_D5},     /* LCD_D5 */
    [6] = {.Port = LCD_PORT_D6, .Pin = LCD_PIN_D6},     /* LCD_D6 */
    [7] = {.Port = LCD_PORT_D7, .Pin = LCD_PIN_D7},     /* LCD_D7 */
};

#elif LCD_MODE == LCD_4_BIT_MODE

const LCD_Pin_t LCD_Pins[4] = 
{
    [0] = {.Port = LCD_PORT_D4, .Pin = LCD_PIN_D4},     /* LCD_D4 */
    [1] = {.Port = LCD_PORT_D5, .Pin = LCD_PIN_D5},     /* LCD_D5 */
    [2] = {.Port = LCD_PORT_D6, .Pin = LCD_PIN_D6},     /* LCD_D6 */
    [3] = {.Port = LCD_PORT_D7, .Pin = LCD_PIN_D7},     /* LCD_D7 */

};
#endif


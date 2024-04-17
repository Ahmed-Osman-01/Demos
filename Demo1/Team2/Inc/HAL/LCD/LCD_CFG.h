/*
 * LCD_config.h
 *
 *  Created on: Mar 22, 2024
 *      Author: Osman
 */

#ifndef HAL_LCD_LCD_CONFIG_H_
#define HAL_LCD_LCD_CONFIG_H_



/**
 * Options:
 *      - LCD_8_BIT_MODE 
 *      - LCD_4_BIT_MODE 
 * 
 */
#define LCD_MODE    LCD_4_BIT_MODE


/* Max size of LCD requests to be buffered and executed at once */
#define REQUEST_BUFFER_SIZE     150



/**
 * @brief Set the Pin and Port for each of the LCD pins
 * 
 */
#if LCD_MODE == LCD_8_BIT_MODE

#define LCD_PIN_D0      GPIO_PIN_0
#define LCD_PORT_D0     GPIO_PORT_A

#define LCD_PIN_D1      GPIO_PIN_1
#define LCD_PORT_D1     GPIO_PORT_A

#define LCD_PIN_D2      GPIO_PIN_2
#define LCD_PORT_D2     GPIO_PORT_A

#define LCD_PIN_D3      GPIO_PIN_3
#define LCD_PORT_D3     GPIO_PORT_A

#endif

#define LCD_PIN_D4      GPIO_PIN_3
#define LCD_PORT_D4     GPIO_PORT_A

#define LCD_PIN_D5      GPIO_PIN_4
#define LCD_PORT_D5     GPIO_PORT_A

#define LCD_PIN_D6      GPIO_PIN_5
#define LCD_PORT_D6     GPIO_PORT_A

#define LCD_PIN_D7      GPIO_PIN_6
#define LCD_PORT_D7     GPIO_PORT_A

#define LCD_PIN_RS      GPIO_PIN_0
#define LCD_PORT_RS     GPIO_PORT_A

#define LCD_PIN_RW      GPIO_PIN_1
#define LCD_PORT_RW     GPIO_PORT_A

#define LCD_PIN_E       GPIO_PIN_2
#define LCD_PORT_E      GPIO_PORT_A


/**
 * @brief Periodicity of the LCD Update task in milli seconds
 * 
 */
#define LCD_UPDATE_PERIODICITY_MS       2U


/*==================================================================================*/
/*           Initialization Options for the LCD                                     */
/*==================================================================================*/


/* Lines mode for the LCD                    */
/* Options:                                  */  
/*      - LCD_ONE_LINE                       */
/*      - LCD_TWO_LINES                      */
#define LCD_LINES_NUMBER       LCD_TWO_LINES


/* Character formats                         */
/* Options:                                  */  
/*      - LCD_5X8_FORMAT                     */
/*      - LCD_5X11_FORMAT                    */
#define LCD_FORMAT       LCD_5X8_FORMAT


/* Increment and Decrement the Register Counter after write operation                       */
/* Options:                                                                                 */  
/*      - LCD_DECREMENT    =>  The Pointer counter decrements (Moves from right to left)    */
/*      - LCD_INCREMENT    =>  The Pointer counter increments (Moves from left to right)    */
#define LCD_INCREMENT_DECREMENT    LCD_INCREMENT


/* Shifts the entire display when enabled, shifts to the Left when Pointer Counter Decrements   */
/* and to the right when it Increments                                                          */
#define LCD_SHIFTING               LCD_DISABLE


/* Display Options for the LCD       */
/* Options:                          */  
/*      - LCD_DISPLAY_OFF            */
/*      - LCD_DISPLAY_ON             */
#define LCD_DISPLAY                LCD_DISPLAY_ON


/* Enable or Disable the LCD curser                */
#define LCD_CURSER                 LCD_ENABLE


/* Enable or Disable the blinking of the curser    */
#define LCD_BLINKING               LCD_DISABLE


#endif /* HAL_LCD_LCD_CONFIG_H_ */

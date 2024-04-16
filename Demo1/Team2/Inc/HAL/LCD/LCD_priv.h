/*
 * LCD_priv.h
 *
 *  Created on: Mar 22, 2024
 *      Author: Osman
 */

#ifndef HAL_LCD_LCD_PRIV_H_
#define HAL_LCD_LCD_PRIV_H_

/* ======================================================================== */
/* Operation modes available for the LCD                                    */
/* ======================================================================== */
#define LCD_4_BIT_MODE     4
#define LCD_8_BIT_MODE     8


/* ======================================================================== */
/* LCD Options states                                                       */
/* ======================================================================== */
#define LCD_DISABLE     0
#define LCD_ENABLE      1

/* ======================================================================== */
/* Increment and Decrement the Register Counter after write operation       */
/* ======================================================================== */
#define LCD_DECREMENT      0
#define LCD_INCREMENT      1

/* ======================================================================== */
/* Display Options for the LCD                                              */
/* ======================================================================== */
#define LCD_DISPLAY_OFF             0
#define LCD_DISPLAY_ON              1

/* ======================================================================== */
/* Line Modes                                                               */
/* ======================================================================== */
#define LCD_ONE_LINE       0
#define LCD_TWO_LINES      1


/* ======================================================================== */
/* Character formats                                                                                                                            */
/* ======================================================================== */
#define LCD_5X8_FORMAT     0
#define LCD_5X11_FORMAT    1

#endif /* HAL_LCD_LCD_PRIV_H_ */

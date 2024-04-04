#ifndef LCD_DRIVER_
#define LCD_DRIVER_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "STD_Types.h"
#include "LCD_cfg.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define LCD_STATE_OFF 0
#define LCD_STATE_INIT 1
#define LCD_STATE_OPER 2

/*Options to select the number of data pins of the LCD*/
#define DATA_8_PINS 8
#define DATA_4_PINS 4

/*Options to select the number of lines of the LCD*/
#define ONE_LINE 0
#define TWO_LINES 1

/*Options to select the font size of the LCD*/
#define FONT_5_8 0
#define FONT_11_8 1

/*Options to select the default state of the display of the LCD*/
#define DISPLAY_OFF 0
#define DISPLAY_ON 1

/*Options to select the default state of the cursor of the LCD*/
#define HIDE_CURSOR 0
#define SHOW_CURSOR 1

/*Options to select the default state of the blinking of the LCD*/
#define BLINK_OFF 0
#define BLINK_ON 1

/*Options to select the default state of the incrementation of the LCD*/
#define NO_INCREMENT 0
#define INCREMENT 1

/*Options to select the default state of the shifting of the LCD*/
#define SHIFT_OFF 0
#define SHIFT_ON 1

/*Options to select the number of the physical digits of the LCD*/
#define DIGIT_16 15
#define DIGIT_24 23
#define DIGIT_40 39
/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
typedef void (*CallBack_t)(void);

typedef struct
{
    void *Port;
    u32_t Pin;
} LCD_Pin_cfg_t;

typedef struct
{
    LCD_Pin_cfg_t LCD_data_pins[NUMBER_OF_DATA_LINES];
    LCD_Pin_cfg_t R_S_pin;
    LCD_Pin_cfg_t R_W_pin;
    LCD_Pin_cfg_t E_pin;
} LCD_cfg_t;

/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

/*
 * use this function to initialize the lcd pins
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status LCD_Init();

/*
 * use this function to write a string on the lcd
 * Parameters:
 * string -> address of the string to be written
 * length -> the length of the string
 * CB -> address of the function to be called after finishing - use NULL if no function is needed -
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input, Status_LCD_Full_Buffer
 */
Error_Status LCD_WriteStringAsync(u8_t *string, u8_t length, CallBack_t CB);

/*
 * use this function to set the position of the lcd cursor
 * Parameters:
 * X_pos -> the desired x position
 * Y_pos -> the desired y position
 * CB -> address of the function to be called after finishing - use NULL if no function is needed -
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input, Status_LCD_Full_Buffer
 */
Error_Status LCD_SetCursorPositionAsync(u8_t X_pos, u8_t Y_pos, CallBack_t CB);

/*
 * use this function to clear the lcd display
 * Parameters:
 * CB -> address of the function to be called after finishing - use NULL if no function is needed -
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input, Status_LCD_Full_Buffer
 */
Error_Status LCD_ClearScreenAsync(CallBack_t CB);

/*
 * use this function to get the satsus of the lcd
 * Parameters:
 * Status -> address of the var to hold the state ->LCD_STATE_OFF, LCD_STATE_INIT, LCD_STATE_OPER
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status LCD_GetStatus(u8_t *Status);

#endif // LCD_DRIVER_
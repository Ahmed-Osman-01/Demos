#ifndef HAL_INCLUDE_LCD_CFG_H_
#define HAL_INCLUDE_LCD_CFG_H_


// please choose FOUR_BIT_MODE --> 7 or EIGHT_BIT_MODE ---> 11
#define LCD_MODE 7

#define LCD_XDIMSION 2
#define LCD_YDIMSION 16 

#if LCD_MODE == 11

    #define FUNCTION_SET 0x38

    /**
     * @brief choose between following configuations
     * 
     * DISPLAY_ON_OFF_CONTROL 0x0f for display on and Cursor display on and blinkin is active
     * DISPLAY_ON_OFF_CONTROL 0x0e for display on and Cursor display on and blinkin is inactive
     * DISPLAY_ON_OFF_CONTROL 0x0c for display on and Cursor display off
     * DISPLAY_ON_OFF_CONTROL 0x08 for display off
     * 
     */
    #define DISPLAY_ON_OFF_CONTROL 0x0e

    #define DISPLAY_CLEAR 0x01

    /**
     * @brief choose between following configuations 
     * 
     * ENTRY_MODE_SET 0x06 for increasing DDRAM address by 1 or decrease by 1 when code is written or read from DDRAM
     * and the display does not shift
     * 
     * ENTRY_MODE_SET 0x07 for increasing DDRAM address by 1 or decrease by 1 when code is written or read from DDRAM 
     * and the dispaly shifts to left
     * 
     * ENTRY_MODE_SET 0x05 for shifting the display to right
     * 
     */
    #define ENTRY_MODE_SET 0x06

#elif LCD_MODE == 7

    #define FUNCTION_SET_1 0x20
    #define FUNCTION_SET_2 0x20
    #define FUNCTION_SET_3 0x80

    #define DISPLAY_ON_OFF_CONTROL_1 0x00
    /**
     * @brief choose between following configuations
     * 
     * DISPLAY_ON_OFF_CONTROL_2 0xf0 for display on and Cursor display on and blinkin is active
     * DISPLAY_ON_OFF_CONTROL_2 0xe0 for display on and Cursor display on and blinkin is inactive
     * DISPLAY_ON_OFF_CONTROL_2 0xc0 for display on and Cursor display off
     * DISPLAY_ON_OFF_CONTROL_2 0x80 for display off
     * 
     */
    #define DISPLAY_ON_OFF_CONTROL_2 0xf0

    #define DISPLAY_CLEAR_1 0x00
    #define DISPLAY_CLEAR_2 0x10

    #define ENTRY_MODE_SET_1 0x00
    /**
     * @brief choose between the following configuations
     * 
     * ENTRY_MODE_SET_2 0x60 for increasing or decreasing the address by one when writing to or reading from the DDRAM 
     * and the display do not shift 
     * 
     * ENTRY_MODE_SET_2 0x70 for increasing or decreasing the address by one when writing to or reading from the DDRAM 
     * and display shifts to left
     * 
     * ENTRY_MODE_SET_2 0x50 for shifting the display to right 
     * 
     */
    #define ENTRY_MODE_SET_2 0x60

	#define SHIFT_LEFT 0x18
	
	#define SHIFT_RIGHT 0x1c
	
#else

    #error "Wrong LCD mode selection"

#endif

#endif // HAL_INCLUDE_LCD_CFG_H_
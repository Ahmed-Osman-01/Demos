#ifndef LCD_CFG_
#define LCD_CFG_

/*********** configure the operation parameters of the LCD ***********/

/*Options are:
 *DATA_8_PINS -> operate the LCD with 8 wires
 *DATA_4_PINS -> operate the LCD with 4 wires
 */
#define NUMBER_OF_DATA_LINES DATA_8_PINS

/*Options are:
 *ONE_LINE  -> operate the LCD with 1 * 16 character blocks
 *TWO_LINES -> operate the LCD with 2 * 16 character blocks
 */
#define NUMBER_OF_LINES TWO_LINES

/*Options are:
 *DIGIT_16  -> operate the LCD with 16 character blocks
 *DIGIT_24  -> operate the LCD with 24 character blocks
 *DIGIT_40  -> operate the LCD with 40 character blocks
 */
#define NUM_OF_DIGITS DIGIT_16

/*Options are:
 *FONT_5_8  -> operate the LCD with 5 * 8 character block size
 *FONT_11_8 -> operate the LCD with 11 * 8 character block size
 */
#define FONT_SIZE FONT_5_8

/*********** configure the display parameters of the LCD ***********/

/*Options are:
 *DISPLAY_OFF -> turn on the display at the beginnig of the LCD operation
 *DISPLAY_ON  -> turn off the display at the beginnig of the LCD operation
 */
#define DISPLAY_STATE DISPLAY_ON

/*Options are:
 *HIDE_CURSOR -> turn on the cursor at the beginnig of the LCD operation
 *SHOW_CURSOR -> turn off the cursor at the beginnig of the LCD operation
 */
#define CUSOR_STATE HIDE_CURSOR

/*Options are:
 *BLINK_OFF -> turn on the blinking at the beginnig of the LCD operation
 *BLINK_ON  -> turn off the blinking at the beginnig of the LCD operation
 */
#define BLINK_STATE BLINK_OFF

/*Options are:
 *INCREMENT -> turn on the incrementation at the beginnig of the LCD operation
 *NO_INCREMENT  -> turn off the incrementation at the beginnig of the LCD operation
 */
#define INCREMENT_STATE INCREMENT

/*Options are:
 *SHIFT_OFF -> turn on the shifting at the beginnig of the LCD operation
 *SHIFT_ON  -> turn off the shifting at the beginnig of the LCD operation
 */
#define SHIFT_STATE SHIFT_OFF

#endif // LCD_CFG_
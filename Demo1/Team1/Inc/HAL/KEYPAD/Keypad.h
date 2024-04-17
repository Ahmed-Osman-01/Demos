/******************************************************************************
*
* Module: Keypad
*
* File Name: Keypad.c
*
* Description: Header file for Keypad.c module
*
* Author: Momen Elsayed Shaban
*
*******************************************************************************/

#ifndef D__ITI_STM32F401CC_DRIVERS_INC_HAL_KEYPAD_KEYPAD_H_
#define D__ITI_STM32F401CC_DRIVERS_INC_HAL_KEYPAD_KEYPAD_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "HAL/KEYPAD/Keypad_Cfg.h"
#include "LIB/std_types.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
/* Type Declarations Used in KEYPAD structure Configurations */
#define KEYPAD_ROWS 0
#define KEYPAD_COLS 1

/* Type Deceleration For readability in the configuration Structure*/
#define ROW0 0
#define ROW1 1
#define ROW2 2
#if (KEYPAD_NUMBER_OF_ROWS == 4)
#define ROW3 3
#endif /* KEYPAD_NUMBER_OF_ROWS == 4 */

#define COL0 0
#define COL1 1
#define COL2 2
#if (KEYPAD_NUMBER_OF_COLS == 4)
#define COL3 3
#endif /* KEYPAD_NUMBER_OF_COLS == 4 */


/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
typedef enum {
    KEYPAD_enumPin0 = 0,
    KEYPAD_enumPin1,
    KEYPAD_enumPin2,
    KEYPAD_enumPin3,
    KEYPAD_enumPin4,
    KEYPAD_enumPin5,
    KEYPAD_enumPin6,
    KEYPAD_enumPin7,
    KEYPAD_enumPin8,
    KEYPAD_enumPin9,
    KEYPAD_enumPin10,
    KEYPAD_enumPin11,
    KEYPAD_enumPin12,
    KEYPAD_enumPin13,
    KEYPAD_enumPin14,
    KEYPAD_enumPin15
} KEYPAD_enumPins_t;     /* Enum for keypad pins */

typedef enum {
    KEYPAD_enumSwitchPressed = 0,
    KEYPAD_enumSwitchReleased
} KEYPAD_enumSwitchState_t;     /* Enum for switch states */

typedef enum {
    KEYPAD_enumLogicLow,
    KEYPAD_enumLogicHigh
} KEYPAD_enumLogicType_t;     /* Enum for keypad logic type */

typedef enum {
    KEYPAD_enumOK,
    KEYPAD_enumInvalidRowsNumber,
    KEYPAD_enumInvalidColsNumber,
    KEYPAD_enumNullPtr,
    KEYPAD_enumInvalidConnectionType,
    KEYPAD_enumInvalidLogicType
} KEYPAD_enumError_t;     /* Enum for keypad error types */

typedef enum {
    KEYPAD_enumPullUp = 1,			/* Equal to 1 to be equivalent to the value of Pull-up resistor in Port Driver */
    KEYPAD_enumPullDown
} KEYPAD_enumConnectionType_t;      /* Enum for keypad connection types */

typedef struct {
    void* KEYPAD_enumPORT;     /* Enum for keypad port */
    KEYPAD_enumPins_t KEYPAD_enumPin;       /* Enum for keypad pin */
} KEYPAD_strRowColConfig_t;					/* Structure for keypad row and column configuration */

typedef struct {
    KEYPAD_strRowColConfig_t KEYPAD_strROWS[KEYPAD_NUMBER_OF_ROWS];     /* Array of row configurations */
    KEYPAD_strRowColConfig_t KEYPAD_strCOLS[KEYPAD_NUMBER_OF_COLS];     /* Array of column configurations */
    KEYPAD_enumConnectionType_t KEYPAD_enumConnection;					/* Enum for keypad connection type */
    KEYPAD_enumLogicType_t KEYPAD_enumLogicType;						/* Enum for keypad logic type */
} KEYPAD_strConfig_t;													/* Structure for overall keypad configuration */

/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/
/*****************************************************************************************
 * @brief   Initialize the keypad configuration and ports.
 *
 * @details This function initializes the keypad configuration and ports based on
 *          the user-configured settings in the KEYPAD module.
 *
 * @return  KEYPAD_enumError_t Error status from the function.
 *             - KEYPAD_enumOK: Function executed successfully.
 *             - KEYPAD_enumInvalidRowsNumber: Invalid number of rows specified.
 *             - KEYPAD_enumInvalidColsNumber: Invalid number of columns specified.
 *             - KEYPAD_enumInvalidConnectionType: Invalid keypad connection type specified.
 *             - KEYPAD_enumInvalidLogicType: Invalid keypad logic type specified.
 *
 * @note    It relies on the user-configured settings in the KEYPAD module, 
 *			including the number of rows and columns,
 *          keypad connection type, and keypad logic type.
 ************************************************************************************************/
KEYPAD_enumError_t KEYPAD_Init();


/***************************************************************************************************
 * @brief   Get the pressed key from a keypad matrix.
 *
 * @details This function iterates through the keypad matrix by setting each column
 *          with logic high and checking each row. If a switch is pressed, the
 *          corresponding row and column are returned in the user-configured characters.
 *
 * @param   Add_KEYPAD_enumSwitch Pointer to the variable where the pressed key will be stored.
 *
 * @return  KEYPAD_enumError_t Error status from the function.
 *             - KEYPAD_enumOK: Function executed successfully.
 *             - KEYPAD_enumNullPtr: Null pointer detected in the argument.
 *
 * @note    - It relies on the KEYPAD_Charchters array, which should be
 *          user-configured to match the keypad matrix layout.
 *          - The function considers the keypad connection type (Pull-up or Pull-down)
 *          and logic type (Active High or Active Low) for handling the state of pressed buttons.
 ********************************************************************************************************/
KEYPAD_enumError_t KEYPAD_getPressedKey(char* Add_KEYPAD_enumSwitchState);




#endif // D__ITI_STM32F401CC_DRIVERS_INC_HAL_KEYPAD_KEYPAD_H_
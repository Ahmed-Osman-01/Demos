 /******************************************************************************
 *
 * Module: KEYPAD_Cfg
 *
 * File Name: KEYPAD_Cfg.c
 *
 * Description: Source file for the KEYPAD driver Configurations
 *
 * Author: Momen Elsayed Shaban
 *
 *******************************************************************************/

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "MCAL/GPIO/GPIO_DRIVER.h"
#include "HAL/KEYPAD/Keypad.h"

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
/*Initialization of the KEYPAD configuration structure*/
#if 0
KEYPAD_strConfig_t KEYPAD_strConfig = {
	.KEYPAD_strROWS = {
		[ROW0] = {
			.KEYPAD_enumPORT = GPIO_PORT_A,
			.KEYPAD_enumPin = KEYPAD_enumPin10,
		},
		[ROW1] = {
			.KEYPAD_enumPORT = GPIO_PORT_A,
			.KEYPAD_enumPin = KEYPAD_enumPin9,
		},
		[ROW2] = {
			.KEYPAD_enumPORT = GPIO_PORT_A,
			.KEYPAD_enumPin = KEYPAD_enumPin8,
		},
	},
	.KEYPAD_strCOLS = {
		[COL0] = {
			.KEYPAD_enumPORT = GPIO_PORT_A,
			.KEYPAD_enumPin = KEYPAD_enumPin11,
		},
		[COL1] = {
			.KEYPAD_enumPORT = GPIO_PORT_A,
			.KEYPAD_enumPin = KEYPAD_enumPin12,
		},
		[COL2] = {
			.KEYPAD_enumPORT = GPIO_PORT_B,
			.KEYPAD_enumPin = KEYPAD_enumPin14,
		},
	},
	.KEYPAD_enumConnection = KEYPAD_enumPullUp,    /*Connection type (Pull-Up or Pull-Down)*/
	.KEYPAD_enumLogicType = KEYPAD_enumLogicHigh   /*Logic type (High or Low when pressed)*/
};
#elif 1
KEYPAD_strConfig_t KEYPAD_strConfig = {
	.KEYPAD_strROWS = {
		[ROW0] = {
			.KEYPAD_enumPORT = GPIO_PORT_B,
			.KEYPAD_enumPin = KEYPAD_enumPin8,
		},
		[ROW1] = {
			.KEYPAD_enumPORT = GPIO_PORT_B,
			.KEYPAD_enumPin = KEYPAD_enumPin9,
		},
		[ROW2] = {
			.KEYPAD_enumPORT = GPIO_PORT_B,
			.KEYPAD_enumPin = KEYPAD_enumPin10,
		},
	},
	.KEYPAD_strCOLS = {
		[COL0] = {
			.KEYPAD_enumPORT = GPIO_PORT_B,
			.KEYPAD_enumPin = KEYPAD_enumPin5,
		},
		[COL1] = {
			.KEYPAD_enumPORT = GPIO_PORT_B,
			.KEYPAD_enumPin = KEYPAD_enumPin6,
		},
		[COL2] = {
			.KEYPAD_enumPORT = GPIO_PORT_B,
			.KEYPAD_enumPin = KEYPAD_enumPin7,
		},
	},
	.KEYPAD_enumConnection = KEYPAD_enumPullUp,    /*Connection type (Pull-Up or Pull-Down)*/
	.KEYPAD_enumLogicType = KEYPAD_enumLogicHigh   /*Logic type (High or Low when pressed)*/
};




#endif
/*Characters assigned to each key on the keypad*/
/*S = Start ,U = Up, P = Stop, L = Left, K = Ok, R = Right, E = Edit, M = Mode "Stopwatch, Display Date and Time"  */
char KEYPAD_Charchters[KEYPAD_NUMBER_OF_ROWS][KEYPAD_NUMBER_OF_COLS] =
{
	{'S','U','P'},
	{'L','E','R'},
	{'M','D','K'}
};



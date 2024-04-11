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
#include "MCAL/GPIO/GPIO.h"
#include "HAL/KEYPAD/Keypad.h"

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
/*Initialization of the KEYPAD configuration structure*/
KEYPAD_strConfig_t KEYPAD_strConfig = {
	.KEYPAD_strROWS = {
		[ROW0] = {
			.KEYPAD_enumPORT = GPIO_PORT_B,
			.KEYPAD_enumPin = KEYPAD_enumPin13,
		},
		[ROW1] = {
			.KEYPAD_enumPORT = GPIO_PORT_B,
			.KEYPAD_enumPin = KEYPAD_enumPin14,
		},
		[ROW2] = {
			.KEYPAD_enumPORT = GPIO_PORT_B,
			.KEYPAD_enumPin = KEYPAD_enumPin15,
		},
	},
	.KEYPAD_strCOLS = {
		[COL0] = {
			.KEYPAD_enumPORT = GPIO_PORT_B,
			.KEYPAD_enumPin = KEYPAD_enumPin12,
		},
		[COL1] = {
			.KEYPAD_enumPORT = GPIO_PORT_B,
			.KEYPAD_enumPin = KEYPAD_enumPin10,
		},
		[COL2] = {
			.KEYPAD_enumPORT = GPIO_PORT_B,
			.KEYPAD_enumPin = KEYPAD_enumPin9,
		},
	},
	.KEYPAD_enumConnection = KEYPAD_enumPullUp,    /*Connection type (Pull-Up or Pull-Down)*/
	.KEYPAD_enumLogicType = KEYPAD_enumLogicHigh   /*Logic type (High or Low when pressed)*/
};

/*Characters assigned to each key on the keypad*/
/*S = Start ,U = Up, P = Stop, L = Left, K = Ok, R = Right, E = Edit, M = Mode "Stopwatch, Display Date and Time"  */
char KEYPAD_Charchters[KEYPAD_NUMBER_OF_ROWS][KEYPAD_NUMBER_OF_COLS] =
{
	{'S','U','P'},
	{'L','K','R'},
	{'E','D','M'}
};



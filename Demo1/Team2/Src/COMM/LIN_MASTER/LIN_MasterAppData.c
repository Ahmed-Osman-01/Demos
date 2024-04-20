/*
 * LIN_Data.c
 *
 *  Created on: Apr 7, 2024
 *      Author: Osman
 */

#include <stdint.h>

#include "COMM/LIN_MASTER/LIN_Types.h"

#define SEND_CONTROLS_MESSAGE   0
#define RECIEVE_CONTROLS_MESSAGE   1


extern const LIN_Message_t MasterMessages[2];

void MasterSendControls(uint16_t controls)
{
	MasterMessages[1].data[0] = (uint8_t)controls;
	MasterMessages[1].data[1] = (uint8_t)(controls>>8);
}


void MasterGetControls(uint16_t * controls)
{
	*controls = ((uint16_t)MasterMessages[0].data[0]) | ((uint16_t)MasterMessages[0].data[1] << 4);
}

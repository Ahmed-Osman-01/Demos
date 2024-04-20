/*
 * LIN_SlaveAppData.c
 *
 *  Created on: Apr 17, 2024
 *      Author: Osman
 */

#include "LIB/STD_TYPES.h"
#include "COMM/LIN_SLAVE/LIN_Types.h"

extern const LIN_Message_t slave1Messages[2];

void SlaveSendControls(uint16_t controls)
{
	slave1Messages[0].data[0] = ((uint8_t)controls) & 0x0f;
	// slave1Messages[0].data[1] = ((uint8_t)(controls>>8)) & 0x1f; // 0000 0000 0000 0000
	slave1Messages[0].data[1] = ((uint8_t)(controls>>4)); // 0000 0000 0000 0000
}


void SlaveGetControls(uint16_t * controls)  
{
	// *controls = ((uint16_t)slave1Messages[1].data[0]) | ((uint16_t)slave1Messages[1].data[1] << 8);
	*controls = ((uint16_t)slave1Messages[1].data[0]) | (((uint16_t)slave1Messages[1].data[1]) << 4);
}

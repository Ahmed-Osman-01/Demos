/*
 * LIN_SlaveAppData.c
 *
 *  Created on: Apr 17, 2024
 *      Author: Osman
 */

#include "LIB/STD_TYPES.h"
#include "COMM/LIN_SLAVE/LIN_Types.h"

extern const LIN_Message_t slave1Messages[2];

void SlaveSendControls(u16 controls)
{
	slave1Messages[0].data[0] = (u8)controls;
	slave1Messages[0].data[1] = (u8)(controls>>8);
}


void SlaveGetControls(u16 * controls)  
{
	*controls = ((u16)slave1Messages[1].data[0]) | ((u16)slave1Messages[1].data[1] << 8);
}

/*
 * LIN_Data.c
 *
 *  Created on: Apr 7, 2024
 *      Author: Osman
 */


#include "LIB/STD_TYPES.h"
#include "COMM/LIN_MASTER/LIN_Types.h"

#define SEND_CONTROLS_MESSAGE   0
#define RECIEVE_CONTROLS_MESSAGE   1


extern const LIN_Message_t MasterMessages[2];

void MasterSendControls(u16 controls)
{
	MasterMessages[1].data[0] = (u8)controls;
	MasterMessages[1].data[1] = (u8)(controls>>8);
}


void MasterGetControls(u16 * controls)
{
	*controls = ((u16)MasterMessages[0].data[0]) | ((u16)MasterMessages[0].data[1] << 8);
}

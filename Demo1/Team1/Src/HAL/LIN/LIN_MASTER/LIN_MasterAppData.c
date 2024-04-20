
#include "HAL/LIN/LIN_MASTER/LIN_MasterAppData.h"

extern const LIN_Message_t Master_Messages[2];

void MasterSendControls(uint16_t control)
{
	
	Master_Messages[1].Data[0] = ((uint8_t)control & 0x0F);
	Master_Messages[1].Data[1] = ((uint8_t)(control >> 4));
}

void MasterGetControls(uint16_t *control)
{
	*control = ((uint16_t)Master_Messages[0].Data[0]) | (((uint16_t)Master_Messages[0].Data[1]) << 4);
}

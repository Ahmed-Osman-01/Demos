

#include "HAL/LIN/LIN_SLAVE/LIN_SlaveAppData.h"

void SlaveSendControls(uint8_t control1, uint8_t control2)
{
	Slave1_Messages[0].Data[0] = control1;
	Slave1_Messages[0].Data[1] = control2;
}

void SlaveGetControls(uint8_t *control1, uint8_t *control2)
{
	*control1 = Slave1_Messages[1].Data[0];
	*control2 = Slave1_Messages[1].Data[1];
}

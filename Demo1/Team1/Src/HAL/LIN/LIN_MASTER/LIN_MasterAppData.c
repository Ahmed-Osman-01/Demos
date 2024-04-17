
#include "HAL/LIN_MASTER/LIN_MasterAppData.h"

void MasterSendControls(u8_t control1, u8_t control2)
{
	Master_Messages[1].Data[0] = control1;
	Master_Messages[1].Data[1] = control2;
}

void MasterGetControls(u8_t *control1, u8_t *control2)
{
	*control1 = Master_Messages[0].Data[0];
	*control2 = Master_Messages[0].Data[1];
}

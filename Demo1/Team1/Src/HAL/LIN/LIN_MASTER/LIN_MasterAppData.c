
#include "HAL/LIN/LIN_MASTER/LIN_MasterAppData.h"

void MasterSendControls(uint8_t control1, uint8_t control2)
{
	
	control1 |= ((control2 & 0x0F) << 4 );
	control2 = ((control2 & 0x10) >> 4 );
	Master_Messages[1].Data[0] = control1;
	Master_Messages[1].Data[1] = control2;
}

void MasterGetControls(uint8_t *control1, uint8_t *control2)
{
	*control1 = Master_Messages[0].Data[0];
	*control2 = Master_Messages[0].Data[1];
}

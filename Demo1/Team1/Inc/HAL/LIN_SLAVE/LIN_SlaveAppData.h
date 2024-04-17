
#ifndef LIN_SLAVE_LIN_SLAVEAPPDATA_H_
#define LIN_SLAVE_LIN_SLAVEAPPDATA_H_

#include "STD_Types.h"
#include "HAL/LIN_Types.h"

extern const LIN_Message_t Slave1_Messages[2];

void SlaveSendControls(u8_t control1, u8_t control2);

void SlaveGetControls(u8_t *control1, u8_t *control2);

#endif

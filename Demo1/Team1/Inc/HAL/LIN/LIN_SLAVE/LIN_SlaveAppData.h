
#ifndef LIN_SLAVE_LIN_SLAVEAPPDATA_H_
#define LIN_SLAVE_LIN_SLAVEAPPDATA_H_

#include "LIB/std_types.h"
#include "HAL/LIN/LIN_Types.h"

extern const LIN_Message_t Slave1_Messages[2];

void SlaveSendControls(uint8_t control1, uint8_t control2);

void SlaveGetControls(uint8_t *control1, uint8_t *control2);

#endif

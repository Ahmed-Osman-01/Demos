
#ifndef MASTER_LIN_MASTERAPPDATA_H_
#define MASTER_LIN_MASTERAPPDATA_H_

#include "LIB/std_types.h"
#include "HAL/LIN/LIN_Types.h"

#define SEND_CONTROLS_MESSAGE 0
#define RECIEVE_CONTROLS_MESSAGE 1

extern const LIN_Message_t Master_Messages[2];

void MasterSendControls(uint8_t control1, uint8_t control2);

void MasterGetControls(uint8_t *control1, uint8_t *control2);

#endif

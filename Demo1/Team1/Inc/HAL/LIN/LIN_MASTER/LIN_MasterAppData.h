
#ifndef MASTER_LIN_MASTERAPPDATA_H_
#define MASTER_LIN_MASTERAPPDATA_H_

#include "LIB/std_types.h"
#include "HAL/LIN/LIN_Types.h"

#define SEND_CONTROLS_MESSAGE 0
#define RECIEVE_CONTROLS_MESSAGE 1


void MasterSendControls(uint16_t control);

void MasterGetControls(uint16_t *control);

#endif

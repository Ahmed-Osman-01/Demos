

#ifndef LIN_SLAVE_H_
#define LIN_SLAVE_H_

#include "LIB/std_types.h"
#include "HAL/LIN/LIN_Types.h"

Error_Status LIN_SlaveInit(LIN_cfg_t LIN_CfgArr);

Error_Status LIN_Assign_DatatoMSGSignal(LIN_Message_t *MSG, uint8_t *Values, uint8_t Values_Num);

#endif
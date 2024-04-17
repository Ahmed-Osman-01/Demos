

#ifndef LIN_SLAVE_H_
#define LIN_SLAVE_H_

Error_Status LIN_SlaveInit(LIN_cfg_t LIN_CfgArr);

Error_Status LIN_Assign_DatatoMSGSignal(LIN_Message_t *MSG, u8_t *Values, u8_t Values_Num);

#endif
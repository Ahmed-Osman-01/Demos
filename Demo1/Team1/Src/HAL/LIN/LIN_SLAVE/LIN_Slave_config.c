
#include "STD_Types.h"
#include "HAL/LIN_Types.h"
#include "HAL/LIN_SLAVE/LIN_Slave_config.h"

static Message_5_Byte_Data_t MSG0Data;
static Message_5_Byte_Data_t MSG1Data;

static LIN_Signal_t Signals[_SLAVE1_MSG_NUM] =
    {
        [0] = {.Start_Index = 0, .Length = 8},
        [1] = {.Start_Index = 8, .Length = 8}};

LIN_Message_t Slave1_Messages[_SLAVE1_MSG_NUM] =
    {
        [First_SMSG] =
            {
                .ID = 0x7,
                .PID = 0x47,
                .Data = MSG0Data,
                .Data_Length = 2,
                .Relation = Sender,
                .Signals_Num = 2,
                .Signals[0] = &Signals[0],
                .Signals[1] = &Signals[1]

            },
        [Second_SMSG] =
            {
                .ID = 0x5,
                .PID = 0x85,
                .Data = MSG1Data,
                .Data_Length = 2,
                .Relation = Receiver,
                .Signals_Num = 2,
                .Signals[0] = &Signals[0],
                .Signals[1] = &Signals[1]}};

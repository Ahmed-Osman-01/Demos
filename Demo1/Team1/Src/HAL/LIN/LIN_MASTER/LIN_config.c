
#include "LIB/std_types.h"
#include "HAL/LIN/LIN_Types.h"
#include "HAL/LIN/LIN_cfg.h"
#include "HAL/LIN/LIN_MASTER/LIN_Master_config.h"
#include "HAL/LIN/LIN_SLAVE/LIN_Slave_config.h"

static Message_8_Byte_Data_t MSG0Data;
static Message_8_Byte_Data_t MSG1Data;

static LIN_Signal_t Signals[_Signal_NUM] =
    {
        [First_signal] = {.Start_Index = 0, .Length = 8},
        [Second_signal] = {.Start_Index = 8, .Length = 8}};

/*
static const LIN_Message_t Slave1_Messages[_SLAVE1_MSG_NUM] =
    {
        [First_MSG] =
            {
                .ID = 0x7,
                .PID = 0x47,
                .Data = MSG0Data,
                .Data_Length = 2,
                .Relation = Sender,
                .Signals_Num = 2,
                .Signals[0] = &Signals[0],
                .Signals[1] = &Signals[2]

            },
        [Second_MSG] =
            {
                .ID = 0x5,
                .PID = 0x85,
                .Data = MSG1Data,
                .Data_Length = 2,
                .Relation = Receiver,
                .Signals_Num = 1,
                .Signals[0] = &Signals[0],
            }};
*/
const LIN_Message_t Master_Messages[_MASTER_MSG_NUM] =
    {
        [First_MSG] =
            {
                .ID = 0x7,
                .PID = 0x47,
                .Data = MSG0Data,
                .Data_Length = 2,
                .Relation = Receiver,
                .Signals_Num = 2,
                .Signals[0] = &Signals[0],
                .Signals[1] = &Signals[1]},
        [Second_MSG] =
            {
                .ID = 0x5,
                .PID = 0x85,
                .Data = MSG1Data,
                .Data_Length = 2,
                .Relation = Sender,
                .Signals_Num = 2,
                .Signals[0] = &Signals[0],
                .Signals[1] = &Signals[1]}};

const LIN_SchedTableEntry_t SchedTable[SCHED_TABLE_MESSAGES_NUM] =
    {
        [0] = {.Message = &Master_Messages[0], .TimeSlotMs = 10},
        [1] = {.Message = &Master_Messages[1], .TimeSlotMs = 10}};

/*
 * LIN_Slave_config.c
 *
 *  Created on: Apr 7, 2024
 *      Author: Osman
 */

#include "LIB/STD_TYPES.h"
#include "COMM/LIN_SLAVE/LIN_Types.h"
#include "COMM/LIN_SLAVE/LIN_Slave_config.h"


static Message0_Data_t msg0DataSlave;
static Message1_Data_t msg1DataSlave;


static const LIN_Signal_t signals[3] =
{
    [0] = {.start = 0, .length = 3},
    [1] = {.start = 4, .length = 2},
    [2] = {.start = 7, .length = 5}
};


const LIN_Message_t slave1Messages[SLAVE1_MESSAGES_COUNT] = 
{
    [0] = 
    {
        .ID = 0x7,
        .PID = 0x47,
        .data = msg0DataSlave,
        .dataLength = 2,
        .relation = Sender,
        .signalsNum = 2,
        .signals[0] = &signals[0],
        .signals[1] = &signals[2]

    },
    [1] = 
    {
        .ID = 0x5,
        .PID = 0x85,
        .data = msg1DataSlave,
        .dataLength = 2,
        .relation = Receiver,
        .signalsNum = 1,
        .signals[0] = &signals[0],
    }
};


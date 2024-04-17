/*
 * LIN_config.c
 *
 *  Created on: Apr 5, 2024
 *      Author: Osman
 */

#include "LIB/STD_TYPES.h"
#include "COMM/LIN_MASTER/LIN_Types.h"
#include "COMM/LIN_MASTER/LIN_Master_config.h"



static Message0_Data_t msg0DataMaster;
static Message1_Data_t msg1DataMaster;


static const LIN_Signal_t signals[3] =
{
    [0] = {.start = 0, .length = 3},
    [1] = {.start = 4, .length = 2},
    [2] = {.start = 7, .length = 5}
};


/* STATIC BECAUSE IT MAKES A MULTIPLE DEFIITION PROBLEM BECUASE THE SLAVE AND MASTER NODES ARE COMPILED
 * ON THE SAME MCU WHICH IS NOT THE CASE IN REAL SITUATION WHERE ONLY THE SLAVE OR THE MASTER EXIST */

static const LIN_Message_t slave1Messages[2] =
{
    [0] =
    {
        .ID = 0x7,
        .PID = 0x47,
        .data = msg0DataMaster,
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
        .data = msg1DataMaster,
        .dataLength = 2,
        .relation = Receiver,
        .signalsNum = 1,
        .signals[0] = &signals[0],
    }
};


const LIN_Message_t MasterMessages[LIN_MASTER_MESSAGES_NUM] =
{
    [0] = 
    {
        .ID = 0x7,
        .PID = 0x47,
        .data = msg0DataMaster,
        .dataLength = 2,
        .relation = Receiver,
        .signalsNum = 2,
        .signals[0] = &signals[0],
        .signals[1] = &signals[2]

    },
    [1] = 
    {
        .ID = 0x5,
        .PID = 0x85,
        .data = msg1DataMaster,
        .dataLength = 2,
        .relation = Sender,
        .signalsNum = 1,
        .signals[0] = &signals[0],
    }
};


const LIN_SchedTableEntry_t schedTable[SCHED_ENTRY_NUM] = 
{
    [0] = {.message = &slave1Messages[0], .timeSlotMs = 100},
    [1] = {.message = &slave1Messages[1], .timeSlotMs = 100}
};

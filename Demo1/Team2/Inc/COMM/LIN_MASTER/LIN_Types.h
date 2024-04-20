/*
 * LIN_StdTypes.h
 *
 *  Created on: Apr 5, 2024
 *      Author: Osman
 */

#ifndef COMM_LIN_MASTER_LIN_TYPES_H_
#define COMM_LIN_MASTER_LIN_TYPES_H_

typedef struct LIN_Message_t LIN_Message_t;

typedef struct
{
    uint8_t start;
    uint8_t length;
    LIN_Message_t * messgae;
}LIN_Signal_t;


typedef enum
{
    Sender,
    Receiver,
    None,
}LIN_MessageRelation_t;

typedef struct LIN_Message_t
{
    uint8_t ID;
    uint8_t PID;
    uint8_t dataLength;
    uint8_t * data;
    uint8_t checksum;
    LIN_MessageRelation_t relation;
    const LIN_Signal_t *signals[64];
    uint8_t signalsNum;
}LIN_Message_t;


typedef struct
{
    LIN_Message_t * message;
    uint8_t timeSlotMs;
}LIN_SchedTableEntry_t;


typedef uint8_t Message0_Data_t[2];
typedef uint8_t Message1_Data_t[2];

#endif /* COMM_LIN_MASTER_LIN_TYPES_H_ */

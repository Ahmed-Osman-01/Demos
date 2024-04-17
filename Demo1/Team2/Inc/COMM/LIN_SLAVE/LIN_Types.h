/*
 * LIN_Types.h
 *
 *  Created on: Apr 7, 2024
 *      Author: Osman
 */

#ifndef COMM_LIN_SLAVE_LIN_TYPES_H_
#define COMM_LIN_SLAVE_LIN_TYPES_H_


typedef struct LIN_Message_t LIN_Message_t;

typedef struct
{
    u8 start;
    u8 length;
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
    u8 ID;
    u8 PID;
    u8 dataLength;
    u8 * data;
    u8 checksum;
    LIN_MessageRelation_t relation;
    const LIN_Signal_t *signals[64];
    u8 signalsNum;
}LIN_Message_t;


typedef struct
{
    LIN_Message_t * message;
    u8 timeSlotMs;
}LIN_SchedTableEntry_t;


typedef u8 Message0_Data_t[2];
typedef u8 Message1_Data_t[2];



#endif /* COMM_LIN_SLAVE_LIN_TYPES_H_ */

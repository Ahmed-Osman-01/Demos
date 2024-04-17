

#ifndef LIN_TYPES_H_
#define LIN_TYPES_H_

#include "LIB/std_types.h"

#define LIN_SIGNAL_MAX_NUM 64

typedef struct
{
    void *Port;
    uint32_t Pin;
} LIN_Pin_cfg_t;

typedef struct
{
    LIN_Pin_cfg_t TX_Pin;
    LIN_Pin_cfg_t RX_Pin;
    uint32_t address;
    uint32_t BaudRate;
    uint32_t WordLength;
    uint32_t ParityControl;
    uint32_t ParitySelect;
    uint32_t StopBits;
    uint32_t OverSampling;
    uint32_t LIN_Mode;
    uint32_t LIN_IRQ;
    uint32_t LIN_BreakLength;
} LIN_cfg_t;

typedef struct LIN_Message_t LIN_Message_t;

typedef struct
{
    uint8_t Start_Index;
    uint8_t Length;
    LIN_Message_t *Message;
    uint8_t *Value;
} LIN_Signal_t;

typedef enum
{
    Sender,
    Receiver,
    None,
} LIN_Relation_t;

typedef struct LIN_Message_t
{
    uint8_t ID;
    uint8_t PID;
    uint8_t Data_Length;
    uint8_t *Data;
    uint8_t CheckSum;
    LIN_Relation_t Relation;
    LIN_Signal_t *Signals[LIN_SIGNAL_MAX_NUM];
    uint8_t Signals_Num;
} LIN_Message_t;

typedef struct
{
    LIN_Message_t *Message;
    uint16_t TimeSlotMs;
} LIN_SchedTableEntry_t;

typedef uint8_t Message_8_Byte_Data_t[8];
typedef uint8_t Message_5_Byte_Data_t[5];

#endif

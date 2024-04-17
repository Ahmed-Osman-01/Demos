

#ifndef LIN_TYPES_H_
#define LIN_TYPES_H_

#include "STD_Types.h"

#define LIN_SIGNAL_MAX_NUM 64

typedef struct
{
    void *Port;
    u32_t Pin;
} LIN_Pin_cfg_t;

typedef struct
{
    LIN_Pin_cfg_t TX_Pin;
    LIN_Pin_cfg_t RX_Pin;
    u32_t address;
    u32_t BaudRate;
    u32_t WordLength;
    u32_t ParityControl;
    u32_t ParitySelect;
    u32_t StopBits;
    u32_t OverSampling;
    u32_t LIN_Mode;
    u32_t LIN_IRQ;
    u32_t LIN_BreakLength;
} LIN_cfg_t;

typedef struct LIN_Message_t LIN_Message_t;

typedef struct
{
    u8_t Start_Index;
    u8_t Length;
    LIN_Message_t *Message;
    u8_t *Value;
} LIN_Signal_t;

typedef enum
{
    Sender,
    Receiver,
    None,
} LIN_Relation_t;

typedef struct LIN_Message_t
{
    u8_t ID;
    u8_t PID;
    u8_t Data_Length;
    u8_t *Data;
    u8_t CheckSum;
    LIN_Relation_t Relation;
    LIN_Signal_t *Signals[LIN_SIGNAL_MAX_NUM];
    u8_t Signals_Num;
} LIN_Message_t;

typedef struct
{
    LIN_Message_t *Message;
    u16_t TimeSlotMs;
} LIN_SchedTableEntry_t;

typedef u8_t Message_8_Byte_Data_t[8];
typedef u8_t Message_5_Byte_Data_t[5];

#endif

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "HAL/LIN_Types.h"
#include "MCAL/GPIO_DRIVER.h"
#include "MCAL/USART_DRIVER.h"
#include "MCAL/STM32f401CC_IRQ.h"
#include "MCAL/NVIC_DRIVER.h"
#include "HAL/LIN_SLAVE/LIN_Slave.h"
#include "HAL/LIN_SLAVE/LIN_Slave_config.h"
/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define LIN_SYNC_BYTE 0x55
#define LIN_GET_ID_MASK 0x3F
#define LIN_GET_1ST_BIT_MASK 0x01
#define LIN_CHK_SUM_INVERSION_MASK 0xFF
#define LIN_MAX_ID 0x3F
#define LIN_TEMP_DATA_SIZE 9

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
typedef struct
{
    u8_t Sync_Byte;
    u8_t PID;
} Header_t;

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/

// static Header_t LIN_Header;
static u8_t LIN_Header[2];
static USART_Req_t Header_Buffer;
static USART_Req_t Send_Buffer;
static USART_Req_t Recieve_Buffer;
static u8_t USART_Peri;
static u8_t Temp_DataReceive[LIN_TEMP_DATA_SIZE];
static u8_t Temp_DataSend[LIN_TEMP_DATA_SIZE];
static LIN_Message_t *CurrMSG;
extern const LIN_Message_t Slave1_Messages[_SLAVE1_MSG_NUM];

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/
static Error_Status LIN_SlaveTask();
static void Receive_Header(void);
static void RecieveTempData(void);
static Error_Status ReceiveData(void);
static void SendData(void);
static u8_t Calculate_Checksum(u8_t PID, u8_t *Data, u8_t size);
static u8_t CalculatePid(u8_t ID);
Error_Status LIN_Collect_DatafromMSG(LIN_Message_t *MSG);
Error_Status LIN_Publish_DataToMSG(LIN_Message_t *MSG);

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

Error_Status LIN_SlaveInit(LIN_cfg_t LIN_CfgArr)
{
    Error_Status LOC_Status = Status_NOK;
    GPIO_Pin_t LIN_Pins[2] = {[0] = {.Pin = LIN_CfgArr.TX_Pin.Pin, .Port = LIN_CfgArr.TX_Pin.Port, .Mode = GPIO_MODE_AF_PP, .Speed = GPIO_SPEED_VHIGH},
                              [1] = {.Pin = LIN_CfgArr.RX_Pin.Pin, .Port = LIN_CfgArr.RX_Pin.Port, .Mode = GPIO_MODE_AF_PP, .Speed = GPIO_SPEED_VHIGH}};
    u8_t LOC_Alternatefunc = 0;
    u8_t LOC_IRQ_Enable = 0;

    switch (LIN_CfgArr.address)
    {
    case USART_Peri_1:
        LOC_IRQ_Enable = NVIC_IRQ_USART1;
        LOC_Alternatefunc = GPIO_FUNC_AF7;
        break;
    case USART_Peri_2:
        LOC_IRQ_Enable = NVIC_IRQ_USART2;
        LOC_Alternatefunc = GPIO_FUNC_AF7;
        break;
    case USART_Peri_6:
        LOC_IRQ_Enable = NVIC_IRQ_USART6;
        LOC_Alternatefunc = GPIO_FUNC_AF8;
        break;
    default:
        break;
    }

    USART_cfg_t USART_LIN_CFG =
        {
            .address = LIN_CfgArr.address,
            .BaudRate = LIN_CfgArr.BaudRate,
            .WordLength = LIN_CfgArr.WordLength,
            .ParityControl = LIN_CfgArr.ParityControl,
            .ParitySelect = LIN_CfgArr.ParityControl,
            .StopBits = LIN_CfgArr.StopBits,
            .OverSampling = LIN_CfgArr.OverSampling};

    USART_LIN_cfg_t LIN_CFG = {
        .USART_Peri = LIN_CfgArr.address,
        .LIN_Mode = LIN_CfgArr.LIN_Mode,
        .LIN_IRQ = LIN_CfgArr.LIN_IRQ,
        .LIN_BreakLength = LIN_CfgArr.LIN_BreakLength};

    USART_Peri = LIN_CfgArr.address;
    Header_Buffer.USART_Peri = USART_LIN_CFG.address;
    Header_Buffer.buffer = LIN_Header;
    Header_Buffer.length = 2;
    Header_Buffer.CB = LIN_SlaveTask;

    LOC_Status = GPIO_Init(LIN_Pins, 2);
    LOC_Status = GPIO_CFG_AlternateFunction(LIN_Pins[0].Port, LIN_Pins[0].Pin, LOC_Alternatefunc);
    LOC_Status = GPIO_CFG_AlternateFunction(LIN_Pins[1].Port, LIN_Pins[1].Pin, LOC_Alternatefunc);
    LOC_Status = NVIC_CTRL_EnableIRQ(LOC_IRQ_Enable);

    LOC_Status = USART_Init(USART_LIN_CFG);
    LOC_Status = USART_LIN_Init(LIN_CFG);
    LOC_Status = USART_Set_BreakCallBack(USART_LIN_CFG.address, Receive_Header);

    return LOC_Status;
}

void Receive_Header(void)
{
    USART_RXBufferAsyncZC(Header_Buffer);
}

Error_Status LIN_SlaveTask()
{
    Error_Status LOC_Status = Status_NOK;
    u8_t Index;
    u8_t PID;
    CurrMSG = NULL;

    //  if (LIN_Header.Sync_Byte != LIN_SYNC_BYTE)
    if (LIN_Header[0] != LIN_SYNC_BYTE)
    {
        LOC_Status = Status_LIN_OUT_SYNC;
    }
    else
    {
        LOC_Status = Status_OK;
        /* PID = CalculatePid(LIN_Header.PID & LIN_GET_ID_MASK);
        if (LIN_Header.PID == PID) */
        PID = CalculatePid(LIN_Header[1] & LIN_GET_ID_MASK);
        if (LIN_Header[1] == PID)
        {
            for (Index = 0; Index < _SLAVE1_MSG_NUM; Index++)
            {
                if (PID == Slave1_Messages[Index].PID)
                {
                    CurrMSG = &Slave1_Messages[Index];
                }
            }
            if (CurrMSG)
            {
                switch (CurrMSG->Relation)
                {
                case Sender:
                    SendData();
                    break;

                case Receiver:
                    RecieveTempData();
                    break;

                default:
                    break;
                }
            }
        }
    }

    return LOC_Status;
}

static void RecieveTempData(void)
{
    Recieve_Buffer.USART_Peri = USART_Peri;
    Recieve_Buffer.buffer = Temp_DataReceive; // ####
    Recieve_Buffer.length = CurrMSG->Data_Length + 1;
    Recieve_Buffer.CB = ReceiveData;
    USART_RXBufferAsyncZC(Recieve_Buffer);
}

static void SendData(void)
{
    u8_t CheckSum;
    u8_t Index;

    CheckSum = Calculate_Checksum(CurrMSG->PID, CurrMSG->Data, CurrMSG->Data_Length);
    Send_Buffer.USART_Peri = USART_Peri;
  //  LIN_Publish_DataToMSG(CurrMSG);

    for (Index = 0; Index < CurrMSG->Data_Length; Index++)
    {
        Temp_DataSend[Index] = CurrMSG->Data[Index];
    }

    Temp_DataSend[CurrMSG->Data_Length] = CheckSum;
    Send_Buffer.buffer = Temp_DataSend;
    Send_Buffer.length = CurrMSG->Data_Length + 1;

    USART_TXBufferAsyncZC(Send_Buffer);
}

static Error_Status ReceiveData(void)
{
    u8_t CheckSum;
    u8_t Index;
    Error_Status LOC_Status = Status_NOK;

    CheckSum = Calculate_Checksum(CurrMSG->PID, Temp_DataReceive, CurrMSG->Data_Length);

    if (CheckSum == Temp_DataReceive[CurrMSG->Data_Length])
    {
        LOC_Status = Status_OK;
        for (Index = 0; Index < CurrMSG->Data_Length; Index++)
        {
            CurrMSG->Data[Index] = Temp_DataReceive[Index];
        }
      //  LIN_Collect_DatafromMSG(CurrMSG);
    }
    else
    {
        LOC_Status = Status_LIN_WRONG_CHK_SUM;
    }

    return LOC_Status;
}

static u8_t CalculatePid(u8_t ID)
{
    u8_t Index;
    u8_t PID;

    if (ID > LIN_MAX_ID)
    {
        /* invalid ID */
    }
    else
    {
        u8_t ID_Bits[6];
        for (Index = 0; Index < 6; Index++)
        {
            ID_Bits[Index] = (ID >> Index) & 0x01;
        }

        u8_t P0 = (ID_Bits[0] ^ ID_Bits[1] ^ ID_Bits[2] ^ ID_Bits[4]) & LIN_GET_1ST_BIT_MASK;
        u8_t P1 = ~((ID_Bits[1] ^ ID_Bits[3] ^ ID_Bits[4] ^ ID_Bits[5]) & LIN_GET_1ST_BIT_MASK);

        PID = ID | (P0 << 6) | (P1 << 7);
    }
    return PID;
}

static u8_t Calculate_Checksum(u8_t PID, u8_t *Data, u8_t Size)
{
    u8_t CheckSum = PID;
    u16_t Index;

    for (Index = 0; Index < Size; Index++)
    {
        CheckSum += Data[Index];
    }

    CheckSum = LIN_CHK_SUM_INVERSION_MASK - CheckSum;
    return CheckSum;
}

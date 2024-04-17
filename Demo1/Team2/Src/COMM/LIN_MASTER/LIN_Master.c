/*
 * LIN_Master.c
 *
 *  Created on: Apr 2, 2024
 *      Author: Osman
 */


#include "LIB/STD_TYPES.h"
#include "MCAL/GPIO/GPIO.h"

#include "MCAL/UART/UART.h"


#include "COMM/LIN_MASTER/LIN_Master.h"
#include "COMM/LIN_MASTER/LIN_Master_config.h"
#include "COMM/LIN_MASTER/LIN_Types.h"

#define LIN_SYNC_BYTE    0x55


extern const LIN_SchedTableEntry_t schedTable[2];
extern const LIN_Message_t MasterMessages[2];

static const LIN_Message_t * currMsgMaster = NULL;
static u8 headerBuffer[2];
static u8 tempRecieveBuffer[9];
static u8 tempSendBuffer[9];


static void LIN_SlaveTask(void);
static void SendData(void);
static void ReceiveData(void);
static u8 Calculate_Checksum(u8 PID, u8 *data, u8 size);


void LIN_MasterGetData(u8 * data)
{
    u8 iter;

    for(iter = 0; iter < MasterMessages[0].dataLength; iter++)
    {
        data[iter] = MasterMessages[0].data[iter];
    }
}

void LIN_MasterSendData(u8 * data)
{
    u8 iter;

    for(iter = 0; iter < MasterMessages[1].dataLength; iter++)
    {
        MasterMessages[1].data[iter] = data[iter];
    }
}

static void sendHeader(LIN_Message_t * msg)
{

    headerBuffer[0] = LIN_SYNC_BYTE;
    headerBuffer[1] = msg->PID;

    UART_SendBreak(LIN_MASTER_SCI);


    /* After sending the header, call the slave_task_master */
    UART_TXBufferAsyncZC(LIN_MASTER_SCI, &headerBuffer, 2, LIN_SlaveTask);

}

void SendData(void)
{
    u8 checksum;

    u8 iter;
    checksum = Calculate_Checksum(currMsgMaster->PID, currMsgMaster->data, currMsgMaster->dataLength);


    for(iter = 0; iter < currMsgMaster->dataLength; iter++)   // ------->> is there a better way??
    {
        tempSendBuffer[iter] = currMsgMaster->data[iter];
    }

    tempSendBuffer[currMsgMaster->dataLength] = checksum;

    UART_TXBufferAsyncZC(LIN_MASTER_SCI, tempSendBuffer, currMsgMaster->dataLength +1, NULL);

}


static void ReceiveData(void)
{
    u8 checksum;
    u8 iter;

    checksum = Calculate_Checksum(currMsgMaster->PID, tempRecieveBuffer, currMsgMaster->dataLength);

    if(checksum == tempRecieveBuffer[currMsgMaster->dataLength])
    {
        /* correct datat */
        /* put it in the data so upper layer can extract it */
        for(iter = 0; iter < currMsgMaster->dataLength; iter++)
        {
            currMsgMaster->data[iter] = tempRecieveBuffer[iter];
        }
    }
    else
    {
        /* bad data */
    }

}


void LIN_MasterInit(void)
{

    GPIO_Pin_t uartTX;
    GPIO_Pin_t uartRX;

    uartTX.Mode = GPIO_MODE_AF_PP;
    uartTX.Pin = LIN_MASTER_TX_PIN;
    uartTX.Port = LIN_MASTER_TX_PORT;
    uartTX.Speed = GPIO_SPEED_HIGH;

    uartRX.Mode = GPIO_MODE_AF_PP;
    uartRX.Pin = LIN_MASTER_RX_PIN;
    uartRX.Port = LIN_MASTER_RX_PORT;
    uartRX.Speed = GPIO_SPEED_HIGH;

    #if (LIN_MASTER_UART == UART1) || (LIN_MASTER_UART == LIN_MASTER_SCI)
    uartRX.AF = GPIO_AF_USART1_2;
    uartTX.AF = GPIO_AF_USART1_2;

    #elif LIN_MASTER_UART == UART6
    uartRX.AF = GPIO_AF_USART6;
    uartTX.AF = GPIO_AF_USART6;
    #endif

    GPIO_Init(&uartTX);
    GPIO_Init(&uartRX);

    UART_Config_t uart1Config;
    uart1Config.BaudRate = LIN_MASTER_SPEED;
    uart1Config.Mode = UART_MODE_TX_RX;
    uart1Config.Parity = UART_PARITY_DISABLE;
    uart1Config.Sampling = UART_SAMPLING_16;
    uart1Config.WordLength = UART_WORDLENGTH_8;

    UART_SetConfig(LIN_MASTER_SCI, &uart1Config);

    UART_LIN_Config_t linConfig;
    linConfig.State = UART_LIN_ENABLED;
    linConfig.BreakDetectInterrupt = UART_LIN_BREAK_INTERRUPT_ENABLED;
    linConfig.BreakDetectLength = UART_LIN_BREAK_DETECTION_11;
    
    /* callback to call when header is sent */
    UART_ConfigLIN(LIN_MASTER_SCI, &linConfig);
}

/* Peiodic task */
void LIN_MasterTask(void)
{
    static u8 timeMs = 0;
    static u8 idx = 0;

    if(timeMs == 0)
    {
        sendHeader(schedTable[idx].message);    
    }

    timeMs += MASTER_TASK_PERIODICITY;

    if(timeMs == schedTable[idx].timeSlotMs)
    {
        timeMs = 0;

        idx++;
        if(idx == SCHED_ENTRY_NUM)
        {
            idx = 0;
        }
    }
}


static void LIN_SlaveTask(void)
{
    u8 iter;
    currMsgMaster = NULL;

    for(iter = 0; iter < LIN_MASTER_MESSAGES_NUM; iter++)
    {
        if(MasterMessages[iter].PID == headerBuffer[1])
        {
            currMsgMaster = &MasterMessages[iter];
        }
    }

	if(currMsgMaster)
	{
		switch(currMsgMaster->relation)
		{
			case Sender:
				SendData();
				break;

			case Receiver:
				UART_RXBufferAsyncZC(LIN_MASTER_SCI, tempRecieveBuffer, currMsgMaster->dataLength+1, ReceiveData);
				break;

			default:
				break;
		}
	}

}


static u8 Calculate_Checksum(u8 PID, u8 *data, u8 size)
{
    u8 sum = PID;
    u16 i;

    for (i = 0; i < size; i++)
    {
        sum += data[i];
    }

    sum = 0xFF - sum;
    return sum;
}

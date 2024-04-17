/*
 * LIN_Slave.c
 *
 *  Created on: Apr 7, 2024
 *      Author: Osman
 */


#include "LIB/STD_TYPES.h"
#include "MCAL/GPIO/GPIO.h"

#include "MCAL/UART/UART.h"


#include "COMM/LIN_SLAVE/LIN_Slave.h"
#include "COMM/LIN_SLAVE/LIN_Slave_config.h"
#include "COMM/LIN_SLAVE/LIN_Types.h"


#define PARITY_MASK     ((u32)(3 << 6))
#define ASYNC_BYTE      0x55


static u8 HeaderBuffer[2];
static u8 tempSendBuffer[9];
static u8 tempReceiveBuffer[9];
static const LIN_Message_t * currMsgSlave = NULL;

extern const LIN_Message_t slave1Messages[2];


static void SlaveTask(void);
static void BreakCallback(void);
static void SendData(void);
static void ReceiveData(void);
static u8 CalculatePid(u8 ID);
static u8 Calculate_Checksum(u8 PID, u8 *data, u8 size);



void LIN_SlaveInit(void)
{
    GPIO_Pin_t uartTX;
    GPIO_Pin_t uartRX;

    uartTX.Mode = GPIO_MODE_AF_PP;
    uartTX.Pin = LIN_SLAVE_TX_PIN;
    uartTX.Port = LIN_SLAVE_TX_PORT;
    uartTX.Speed = GPIO_SPEED_HIGH;

    uartRX.Mode = GPIO_MODE_AF_PP;
    uartRX.Pin = LIN_SLAVE_RX_PIN;
    uartRX.Port = LIN_SLAVE_RX_PORT;
    uartRX.Speed = GPIO_SPEED_HIGH;

    #if (LIN_SLAVE_SCI == UART1) || (LIN_SLAVE_SCI == UART2)
    uartRX.AF = GPIO_AF_USART1_2;
    uartTX.AF = GPIO_AF_USART1_2;

    #elif LIN_SLAVE_SCI == UART6
    uartRX.AF = GPIO_AF_USART6;
    uartTX.AF = GPIO_AF_USART6;
    #endif

    GPIO_Init(&uartTX);
    GPIO_Init(&uartRX);

    UART_Config_t uart1Config;
    uart1Config.BaudRate = LIN_SLAVE_SPEED;
    uart1Config.Mode = UART_MODE_TX_RX;
    uart1Config.Parity = UART_PARITY_DISABLE;
    uart1Config.Sampling = UART_SAMPLING_16;
    uart1Config.WordLength = UART_WORDLENGTH_8;

    UART_SetConfig(LIN_SLAVE_SCI, &uart1Config);

    UART_LIN_Config_t linConfig;
    linConfig.State = UART_LIN_ENABLED;
    linConfig.BreakDetectInterrupt = UART_LIN_BREAK_INTERRUPT_ENABLED;
    linConfig.BreakDetectLength = UART_LIN_BREAK_DETECTION_11;
    
    /* callback to call when header is sent */
    UART_SetLBDCallback(LIN_SLAVE_SCI, BreakCallback);
    UART_ConfigLIN(LIN_SLAVE_SCI, &linConfig);
}

static void BreakCallback(void)
{
    UART_RXBufferAsyncZC(LIN_SLAVE_SCI, HeaderBuffer, 2, SlaveTask);
}

static void SlaveTask(void)
{
    u8 iter;
    currMsgSlave = NULL;
    u8 PID;
 

    if(HeaderBuffer[0] == ASYNC_BYTE)
    {
        PID = CalculatePid(HeaderBuffer[1] & (~PARITY_MASK));

        if(HeaderBuffer[1] == PID)
        {
            /* good PID */

            for(iter = 0; iter < SLAVE1_MESSAGES_COUNT; iter++)
            {
                if(PID == slave1Messages[iter].PID)
                {
                    currMsgSlave = &slave1Messages[iter];
                }
            }

            if(currMsgSlave) /* if a message with the same ID as received found */
            {
                switch(currMsgSlave->relation)
                {
                    case Sender:
                        SendData();
                        break;

                    case Receiver:
                        UART_RXBufferAsyncZC(LIN_SLAVE_SCI, tempReceiveBuffer, currMsgSlave->dataLength+1, ReceiveData);
                        break;

                    default:
                        break;
                }
            }
        }
        else
        {
            /* bad PID */
        }     
    }
    else
    {
        /* bad sync */
    }
}


static void SendData(void)
{
    u8 checksum;
    u8 iter;

    checksum = Calculate_Checksum(currMsgSlave->PID, currMsgSlave->data, currMsgSlave->dataLength);

    for(iter = 0; iter < currMsgSlave->dataLength; iter++)   // ------->> is there a better way??
    {
        tempSendBuffer[iter] = currMsgSlave->data[iter];
    }

    tempSendBuffer[currMsgSlave->dataLength] = checksum;

    UART_TXBufferAsyncZC(LIN_SLAVE_SCI, tempSendBuffer, currMsgSlave->dataLength +1, NULL);

}

static void ReceiveData(void)
{
    u8 checksum;
    u8 iter;

    checksum = Calculate_Checksum(currMsgSlave->PID, tempReceiveBuffer, currMsgSlave->dataLength);

    if(checksum == tempReceiveBuffer[currMsgSlave->dataLength])
    {
        /* correct data */
        /* put it in the data so upper layer can extract it */
        for(iter = 0; iter < currMsgSlave->dataLength; iter++)
        {
            currMsgSlave->data[iter] = tempReceiveBuffer[iter];
        }
    }
    else
    {
        /* bad data */
    }
}


u8 CalculatePid(u8 ID)
{
    u8 iter;
    u8 PID;

    if (ID > 0x3F)
    {
        /* invalid ID */
    }
    else
    {
        u8 IDBuf[6];
        for (iter = 0; iter < 6; iter++)
        {
            IDBuf[iter] = (ID >> iter) & 0x01;
        }

        u8 P0 = (IDBuf[0] ^ IDBuf[1] ^ IDBuf[2] ^ IDBuf[4]) & 0x01;
        u8 P1 = (~(IDBuf[1] ^ IDBuf[3] ^ IDBuf[4] ^ IDBuf[5])) & 0x01;

        PID = ID | (P0 << 6) | (P1 << 7);
    }
    return PID;
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

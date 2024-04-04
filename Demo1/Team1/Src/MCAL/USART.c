/******************************************************************************
*
* Module: USART
*
* File Name: USART.c
*
* Description: Source file for the USART driver for STM32F401xC
*
* Author: Momen Elsayed Shaban
*
*******************************************************************************/

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "MCAL/USART/USART.h"
#include "MCAL/USART/USART_Cfg.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define NUMBER_OF_USART_INSTANCE        3
#define USART1_BASE_ADDR                0x40011000
#define USART2_BASE_ADDR                0x40004400
#define USART6_BASE_ADDR                0x40011400      
#define USART_OVERSAMPLING_BIT          15
#define USART_FRACTION_OVERFLOW_LIMIT   0x10
#define USART_ENABLE                    0x00002000
#define USART_MAX_BAUDRATE              10500000
#define USART_TX_ENABLE                 0x00000008
#define USART_RX_ENABLE                 0x00000004
#define USART_SR_TXE_MASK               0x00000080
#define USART_SR_RXNE_MASK              0x00000020
#define USART_TXEIE_ENABLE              0x00000080
#define USART_RXNEIE_ENABLE             0x00000004
#define USART_TX_DONE_IRQ               0x00000080
#define USART_RX_DONE_IRQ               0x00000020
/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
typedef struct
{
    volatile u32 SR;
    volatile u32 DR;
    volatile u32 BRR;
    volatile u32 CR1;
    volatile u32 CR2;
    volatile u32 CR3;
    volatile u32 GTPR;
}USART_Registers_t;

typedef struct
{
    u8 *data;
    u32 pos;
    u32 size;
}buffer_t;

typedef enum{
    Req_state_Idle,
    Req_state_Busy
}Req_State_t;

typedef struct
{
    buffer_t buffer;
    Req_State_t state;
    CallBack_t CallBack;
}Tx_Req_t;

typedef struct
{
    buffer_t buffer;
    Req_State_t state;
    CallBack_t CallBack;
}Rx_Req_t;

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
static USART_Registers_t* const USART[NUMBER_OF_USART_INSTANCE] = {(USART_Registers_t*)USART1_BASE_ADDR,
                                                                   (USART_Registers_t*)USART2_BASE_ADDR,
                                                                   (USART_Registers_t*)USART6_BASE_ADDR};

extern const USART_Cfg_t USART_Cfg[_USART_Num];
Tx_Req_t Tx_Req[NUMBER_OF_USART_INSTANCE] = {0};
Rx_Req_t Rx_Req[NUMBER_OF_USART_INSTANCE] = {0};

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

USART_ErrorStatus_t USART_init(void)
{
    USART_ErrorStatus_t ErrorStatus = USART_OK;
    u8 idx = 0;
    u32 USARTDIV_value = 0;
    u8 overSampling_value = 0;
    u32 Mantissa = 0;
    u16 Fraction = 0;
    u32 BRR_value = 0; 
    u32 CR1_value = 0;
    u32 CR2_value = 0;
    for(idx = 0; idx < _USART_Num; idx++)
    {
        if(USART_Cfg[idx].USART_Number > NUMBER_OF_USART_INSTANCE)
        {
            ErrorStatus = USART_InvalidNumber;
        }
        else if(USART_Cfg[idx].USART_BaudRate > USART_MAX_BAUDRATE)
        {
            ErrorStatus = USART_InvalidBaudRate;
        }
        else
        {
            overSampling_value = ((USART_Cfg[idx].USART_OverSampling) >> USART_OVERSAMPLING_BIT);
            USARTDIV_value = ((u64)USART_CLK * 1000)/((8 * (2 - overSampling_value) * (USART_Cfg[idx].USART_BaudRate)));
            Mantissa = USARTDIV_value/1000;
            Fraction = (USARTDIV_value % 1000) * (8 * (2 - overSampling_value));
            if((Fraction % 1000)  >= 500)
            {
                Fraction = ( Fraction / 1000 ) + 1;
            }
            else
            {
                Fraction = Fraction / 1000;
            }
            if(Fraction >= USART_FRACTION_OVERFLOW_LIMIT)
            {
                Mantissa += Fraction & 0xF0;
            }
            BRR_value = (Mantissa << 4U) | (Fraction & 0x0F);
            USART[USART_Cfg[idx].USART_Number]->BRR = BRR_value;

            CR1_value = (USART_Cfg[idx].USART_OverSampling) | USART_ENABLE | (USART_Cfg[idx].USART_WordLen)\
                        |(USART_Cfg[idx].USART_ParityControl) | (USART_Cfg[idx].USART_ParitySelection);
            USART[USART_Cfg[idx].USART_Number]->CR1 = CR1_value;

            CR2_value = USART_Cfg[idx].USART_StopBits;
            USART[USART_Cfg[idx].USART_Number]->CR2 = CR2_value;

            }        
    }
    return ErrorStatus;
}


USART_ErrorStatus_t USART_sendByte(USART_Req_t USART_Req)
{
    USART_ErrorStatus_t ErrorStatus = USART_OK;
    volatile u16 timeOut = 3000;
    if(USART_Req.data == NULL_PTR)
    {
        ErrorStatus = USART_NullPtr;
    }
    else if(USART_Req.USART_Number > _USART_Num)
    {
        ErrorStatus = USART_InvalidNumber;
    }
    else if (Tx_Req[USART_Req.USART_Number].state == Req_state_Busy)
    {
        ErrorStatus = USART_Busy;
    }
    else
    {
        Tx_Req[USART_Req.USART_Number].state = Req_state_Busy;
        USART[USART_Req.USART_Number]->DR = *(USART_Req.data);
        USART[USART_Req.USART_Number]->CR1 |= USART_TX_ENABLE;
        while (timeOut)
        {
            timeOut--;
        }
        
        USART[USART_Req.USART_Number]->CR1 &= ~USART_TX_ENABLE; /*Disable Transmitting After Sending the Data*/
        Tx_Req[USART_Req.USART_Number].state = Req_state_Idle;
    }
    return ErrorStatus;
}

USART_ErrorStatus_t USART_recieveByte(USART_Req_t USART_Req)
{
    USART_ErrorStatus_t ErrorStatus = USART_OK;
    volatile u16 timeOut = 3000;
    if((USART_Req.data == NULL_PTR))
    {
        ErrorStatus = USART_NullPtr;
    }
    else if(USART_Req.USART_Number > _USART_Num)
    {
        ErrorStatus = USART_InvalidNumber;
    }
    else if (Rx_Req[USART_Req.USART_Number].state == Req_state_Busy)
    {
        ErrorStatus = USART_Busy;
    }
    else
    {
        Rx_Req[USART_Req.USART_Number].state = Req_state_Busy;
        USART[USART_Req.USART_Number]->CR1 |= USART_RX_ENABLE;
        while (!(USART[USART_Req.USART_Number]->SR & USART_SR_RXNE_MASK) && timeOut)
        {
            timeOut--;
        }

        if(timeOut == 0)
        {
            ErrorStatus = USART_TimeOut;
        }
        *(USART_Req.data) = USART[USART_Req.USART_Number]->DR;
        USART[USART_Req.USART_Number]->CR1 |= ~USART_RX_ENABLE;
        Rx_Req[USART_Req.USART_Number].state = Req_state_Idle; 
    }
    return ErrorStatus;
}

USART_ErrorStatus_t USART_sendBufferAsyncZC(USART_Req_t USART_Req)
{
    USART_ErrorStatus_t ErrorStatus = USART_OK;
    if((USART_Req.data == NULL_PTR) || (USART_Req.CB == NULL_PTR))
    {
        ErrorStatus = USART_NullPtr;
    }
    else if(USART_Req.USART_Number > _USART_Num)
    {
        ErrorStatus = USART_InvalidNumber;
    }
    else if (Tx_Req[USART_Req.USART_Number].state == Req_state_Busy)
    {
        ErrorStatus = USART_Busy;
    }
    else
    { 
        Tx_Req[USART_Req.USART_Number].buffer.data = USART_Req.data;
        Tx_Req[USART_Req.USART_Number].buffer.pos = 0;
        Tx_Req[USART_Req.USART_Number].buffer.size = USART_Req.length;
        Tx_Req[USART_Req.USART_Number].CallBack = USART_Req.CB;
        Tx_Req[USART_Req.USART_Number].state = Req_state_Busy;

        USART[USART_Req.USART_Number]->CR1 |= USART_TX_ENABLE;
        /*Sending First Byte To Enter Interrupt Then Continue Sending in the Hanlder*/
        USART[USART_Req.USART_Number]->DR = Tx_Req[USART_Req.USART_Number].buffer.data[0];
        Tx_Req[USART_Req.USART_Number].buffer.pos++;
        USART[USART_Req.USART_Number]->CR1 |= USART_TXEIE_ENABLE;   /*Enable Tx Interrupts*/
    }
    return ErrorStatus;
}

USART_ErrorStatus_t USART_recieveBufferAsyncZC(USART_Req_t USART_Req)
{
    USART_ErrorStatus_t ErrorStatus = USART_OK;
    if((USART_Req.data == NULL_PTR) || (USART_Req.CB == NULL_PTR))
    {
        ErrorStatus = USART_NullPtr;
    }
    else if(USART_Req.USART_Number > _USART_Num)
    {
        ErrorStatus = USART_InvalidNumber;
    }
    else if (Rx_Req[USART_Req.USART_Number].state == Req_state_Busy)
    {
        ErrorStatus = USART_Busy;
    }
    else
    { 
        Rx_Req[USART_Req.USART_Number].buffer.data = USART_Req.data;
        Rx_Req[USART_Req.USART_Number].buffer.pos = 0;
        Rx_Req[USART_Req.USART_Number].buffer.size = USART_Req.length;
        Rx_Req[USART_Req.USART_Number].CallBack = USART_Req.CB;
        Rx_Req[USART_Req.USART_Number].state = Req_state_Busy;

        USART[USART_Req.USART_Number]->CR1 |= USART_RX_ENABLE;
        USART[USART_Req.USART_Number]->CR1 |= USART_RXNEIE_ENABLE;   /*Enable Rx Interrupts*/
    }
    return ErrorStatus; 
}

void USART1_IRQHandler(void)
{
    if((USART[USART_NUMBER_1]->SR) & USART_TX_DONE_IRQ)
    {
        if(Tx_Req[USART_NUMBER_1].buffer.pos < Tx_Req[USART_NUMBER_1].buffer.size)
        {
            USART[USART_NUMBER_1]->DR = Tx_Req[USART_NUMBER_1].buffer.data[Tx_Req[USART_NUMBER_1].buffer.pos];
            Tx_Req[USART_NUMBER_1].buffer.pos++;
        }
        else
        {
            USART[USART_NUMBER_1]->CR1 &= ~USART_TX_ENABLE;
            USART[USART_NUMBER_1]->CR1 &= ~USART_TXEIE_ENABLE;   /*Disable Tx Interrupts*/
            Tx_Req[USART_NUMBER_1].state = Req_state_Idle;
            if(Tx_Req[USART_NUMBER_1].CallBack != NULL_PTR)
            {
                Tx_Req[USART_NUMBER_1].CallBack();
            }
        }
    }

    if((USART[USART_NUMBER_1]->SR) & USART_RX_DONE_IRQ)
    {
        if(Rx_Req[USART_NUMBER_1].buffer.pos < Rx_Req[USART_NUMBER_1].buffer.size)
        {
            Rx_Req[USART_NUMBER_1].buffer.data[Rx_Req[USART_NUMBER_1].buffer.pos] = USART[USART_NUMBER_1]->DR;
            Rx_Req[USART_NUMBER_1].buffer.pos++;
        }
        else
        {
            USART[USART_NUMBER_1]->CR1 &= ~USART_RX_ENABLE;
            USART[USART_NUMBER_1]->CR1 &= ~USART_RXNEIE_ENABLE;   /*Disable Rx Interrupts*/
            Rx_Req[USART_NUMBER_1].state = Req_state_Idle;
            if(Rx_Req[USART_NUMBER_1].CallBack != NULL_PTR)
            {
                Rx_Req[USART_NUMBER_1].CallBack();
            }
        }
    }
}

void USART2_IRQHandler(void)
{
    if((USART[USART_NUMBER_2]->SR) & USART_TX_DONE_IRQ)
    {
        if(Tx_Req[USART_NUMBER_2].buffer.pos < Tx_Req[USART_NUMBER_2].buffer.size)
        {
            USART[USART_NUMBER_2]->DR = Tx_Req[USART_NUMBER_2].buffer.data[Tx_Req[USART_NUMBER_2].buffer.pos];
            Tx_Req[USART_NUMBER_2].buffer.pos++;
        }
        else
        {
            USART[USART_NUMBER_2]->CR1 &= ~USART_TX_ENABLE;
            USART[USART_NUMBER_2]->CR1 &= ~USART_TXEIE_ENABLE;   /*Disable Tx Interrupts*/
            Tx_Req[USART_NUMBER_2].state = Req_state_Idle;
            if(Tx_Req[USART_NUMBER_2].CallBack != NULL_PTR)
            {
                Tx_Req[USART_NUMBER_2].CallBack();
            }
        }
    }

    if((USART[USART_NUMBER_2]->SR) & USART_RX_DONE_IRQ)
    {
        if(Rx_Req[USART_NUMBER_2].buffer.pos < Rx_Req[USART_NUMBER_2].buffer.size)
        {
            Rx_Req[USART_NUMBER_2].buffer.data[Rx_Req[USART_NUMBER_2].buffer.pos] = USART[USART_NUMBER_2]->DR;
            Rx_Req[USART_NUMBER_2].buffer.pos++;
        }
        else
        {
            USART[USART_NUMBER_2]->CR1 &= ~USART_RX_ENABLE;
            USART[USART_NUMBER_2]->CR1 &= ~USART_RXNEIE_ENABLE;   /*Disable Rx Interrupts*/
            Rx_Req[USART_NUMBER_2].state = Req_state_Idle;
            if(Rx_Req[USART_NUMBER_2].CallBack != NULL_PTR)
            {
                Rx_Req[USART_NUMBER_2].CallBack();
            }
        }
    }
}

void USART6_IRQHandler(void)
{
    if((USART[USART_NUMBER_6]->SR) & USART_TX_DONE_IRQ)
    {
        if(Tx_Req[USART_NUMBER_6].buffer.pos < Tx_Req[USART_NUMBER_6].buffer.size)
        {
            USART[USART_NUMBER_6]->DR = Tx_Req[USART_NUMBER_6].buffer.data[Tx_Req[USART_NUMBER_6].buffer.pos];
            Tx_Req[USART_NUMBER_6].buffer.pos++;
        }
        else
        {
            USART[USART_NUMBER_6]->CR1 &= ~USART_TX_ENABLE;
            USART[USART_NUMBER_6]->CR1 &= ~USART_TXEIE_ENABLE;   /*Disable Tx Interrupts*/
            Tx_Req[USART_NUMBER_6].state = Req_state_Idle;
            if(Tx_Req[USART_NUMBER_6].CallBack != NULL_PTR)
            {
                Tx_Req[USART_NUMBER_6].CallBack();
            }
        }
    }

    if((USART[USART_NUMBER_6]->SR) & USART_RX_DONE_IRQ)
    {
        if(Rx_Req[USART_NUMBER_6].buffer.pos < Rx_Req[USART_NUMBER_6].buffer.size)
        {
            Rx_Req[USART_NUMBER_6].buffer.data[Rx_Req[USART_NUMBER_6].buffer.pos] = USART[USART_NUMBER_6]->DR;
            Rx_Req[USART_NUMBER_6].buffer.pos++;
        }
        else
        {
            USART[USART_NUMBER_6]->CR1 &= ~USART_RX_ENABLE;
            USART[USART_NUMBER_6]->CR1 &= ~USART_RXNEIE_ENABLE;   /*Disable Rx Interrupts*/
            Rx_Req[USART_NUMBER_6].state = Req_state_Idle;
            if(Rx_Req[USART_NUMBER_6].CallBack != NULL_PTR)
            {
                Rx_Req[USART_NUMBER_6].CallBack();
            }
        }
    }
}
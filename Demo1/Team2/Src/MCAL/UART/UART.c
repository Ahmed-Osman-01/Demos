/*
 * UART.c
 *
 *  Created on: Mar 26, 2024
 *      Author: Osman
 */


/* ============================================================================ */
/*                                  INCLUDEDS                           	    */
/* ============================================================================ */

#include "LIB/STD_TYPES.h"
#include "MCAL/UART/UART.h"
#include <stdint.h>

/* ============================================================================ */
/*                                   MACROS                             	    */
/* ============================================================================ */


#define _UARTS_NUM  3               /* Number of available uarts */
#define _CLK_FREQ   25000000UL      /* Frequency of the cpu clock */

#define TXE_TIMEOUT_VAL     600     /* Timeout count in cast of Synchronous Transfere of a byte */
#define RXE_TIMEOUT_VAL     600     /* Timeout count in cast of Synchronous Recieve of a byte */


#define MANTISSA_POS    4U          /* Position of the mantissa part in the BRR */

#define READY   0U
#define BUSY    1U

#define UART1_INDEX     0
#define UART2_INDEX     1
#define UART6_INDEX     2


/* ------------------------------------- */
/*                  MASKS                */
/* ------------------------------------- */

#define UART_RE_MASK        ((uint32_t)(1 << 2))
#define UART_TE_MASK        ((uint32_t)(1 << 3))
#define UART_MODE_MASK      ((uint32_t)(3 << 2))
#define UART_RXNEIE_MASK    ((uint32_t)(1 << 5))
#define UART_TXEIE_MASK     ((uint32_t)(1 << 7))
#define UART_PARITY_MASK    ((uint32_t)(3 << 9))
#define UART_LENGTH_MASK    ((uint32_t)(1 << 12))
#define UART_UE_MASK        ((uint32_t)(1 << 13))
#define UART_SAMPLING_MASK  ((uint32_t)(1 << 15))

#define UART_TXE_MASK   ((uint32_t)(1 << 7))
#define UART_RXNE_MASK  ((uint32_t)(1 << 5))

#define UART_LBD_MASK   ((uint32_t)(1 << 8))
/* ------------------------------------- */
/*                  VALIDATIONS          */
/* ------------------------------------- */

#define IS_VALID_MODE(MODE)   ((MODE) == (UART_MODE_TX)||\
                               (MODE) == (UART_MODE_RX)||\
                               (MODE) == (UART_MODE_TX_RX))

#define IS_VALID_BAUDRATE(BR)  ((BR) < 1000000U)

#define IS_VALID_PARITY(PARITY)  ((PARITY) == UART_PARITY_DISABLE||\
                                  (PARITY) == UART_PARITY_ENABLE_EVEN||\
                                  (PARITY) == UART_PARITY_ENABLE_ODD)


#define IS_VALID_LENGTH(LENGTH)     ((LENGTH) == UART_WORDLENGTH_8||\
                                     (LENGTH) == UART_WORDLENGTH_9)

#define IS_VALID_UART(UART)         ((UART) == UART1||\
                                     (UART) == UART2||\
                                     (UART) == UART6)

#define IS_VALID_SAMPLING(SAMPLING) ((SAMPLING) == UART_SAMPLING_8 || (SAMPLING) == UART_SAMPLING_16)



/* ============================================================================ */
/*                                  	TYPES                           	    */
/* ============================================================================ */

typedef struct
{
    uint32_t SR;
    uint32_t DR;
    uint32_t BRR;
    uint32_t CR1;
    uint32_t CR2;
    uint32_t CR3;
    uint32_t GTPR;

}UART_Registers_t;


typedef struct
{
    uint8_t * Data;
    uint16_t Length;
    uint16_t CurrIdx;

}UART_Buffer_t;


typedef struct
{
    UART_Buffer_t Buffer;
    uint8_t State;
    UART_ReqCallback_t Cb;

}UART_Req_t;




/* ============================================================================ */
/*                                  GLOBAL VARIABLES                    	    */
/* ============================================================================ */


static UART_Registers_t* UARTs[_UARTS_NUM] = {(UART_Registers_t*)0x40011000, (UART_Registers_t*)0x40004400,
											  (UART_Registers_t*)0x40011400};

static UART_Req_t TXRequests[_UARTS_NUM];
static UART_Req_t RXRequests[_UARTS_NUM];


static UART_LBDCallback_t LBDCallbacks[_UARTS_NUM] = {NULL, NULL, NULL};

/* ============================================================================ */
/*                                  IMPLEMENTATION                           	*/
/* ============================================================================ */


UART_ErrorStatus_t  UART_SetConfig(uint8_t UARTx, UART_Config_t* config)
{
    UART_ErrorStatus_t Ret_ErrorStatus = UART_OK;
    uint32_t tmpReg;
    uint32_t uartdiv;
    uint8_t mantissa;
    uint32_t fraction;

    if(!IS_VALID_UART(UARTx))
    {
        Ret_ErrorStatus = UART_INVALID_UART;
    }
    else if(!IS_VALID_BAUDRATE(config->BaudRate))
    {
        Ret_ErrorStatus = UART_INVALID_BAUDRATE;
    }
    else if(!IS_VALID_LENGTH(config->WordLength))
    {
        Ret_ErrorStatus = UART_INVALID_WORD_LENGTH;
    }
    else if(!IS_VALID_MODE(config->Mode))
    {
        Ret_ErrorStatus = UART_INVALID_MODE;
    }
    else if(!IS_VALID_PARITY(config->Parity))
    {
        Ret_ErrorStatus = UART_INVALID_PARITY;
    }
    else if(!IS_VALID_SAMPLING(config->Sampling))
    {
        Ret_ErrorStatus = UART_INVALID_SAMPLING;
    }
    else
    {
        /* do nothing */
    }


    if(Ret_ErrorStatus == UART_OK)
    {
        tmpReg = UARTs[UARTx]->CR1;
        tmpReg &= (~UART_MODE_MASK) | (~UART_RXNEIE_MASK) | (~UART_TXEIE_MASK) | (~UART_PARITY_MASK)|\
                    (~UART_LENGTH_MASK) | (~UART_SAMPLING_MASK);
        tmpReg |= (config->Mode) | (config->Parity) | (config->WordLength) | (config->Sampling) | (UART_UE_MASK);
        UARTs[UARTx]->CR1 = tmpReg;

        /* Calculate Mantissa and Fraction for BaudRate*/

        uartdiv = ((u64)_CLK_FREQ * 1000)/((2-(UARTs[UARTx]->CR1 & UART_SAMPLING_MASK)) * 8 * config->BaudRate);

        mantissa = uartdiv/1000;

        fraction = (uartdiv % 1000) * ((2-(UARTs[UARTx]->CR1 & UART_SAMPLING_MASK)) * 8);
        

        if(fraction % 1000 >= 500)
        {
            fraction = (fraction/1000) + 1 ;          
        }
        else
        {
            fraction = fraction/1000;
        }

        if( ((UARTs[UARTx]->CR1 & UART_SAMPLING_MASK) == UART_SAMPLING_16) && (fraction > 15) )
        {
            fraction = 0;
            mantissa++;
        }

        if( ((UARTs[UARTx]->CR1 & UART_SAMPLING_MASK) == UART_SAMPLING_8) && (fraction > 7) )
        {
            fraction = 0;
            mantissa++;
        }

        UARTs[UARTx]->BRR = (mantissa << MANTISSA_POS) | fraction;

    }

    return Ret_ErrorStatus;
}


UART_ErrorStatus_t UART_TXByte(uint8_t UARTx, uint8_t byte)
{
    UART_ErrorStatus_t Ret_ErrorStatus = UART_OK;
    uint16_t timeout = TXE_TIMEOUT_VAL;

    if(!IS_VALID_UART(UARTx))
    {
        Ret_ErrorStatus = UART_INVALID_UART;
    }
    else
    {
        /* do nothing */
    }

    if(Ret_ErrorStatus == UART_OK)
    {
        /* Disable TXE Interrupt */
        UARTs[UARTx]->CR1 &= ~UART_TXEIE_MASK;
        /* Send the Byte */
        UARTs[UARTx]->DR = byte;
        /* Wait till Byte is sent */
        while( ((UARTs[UARTx]->SR & UART_TXE_MASK) == 0) && timeout )
        {
            timeout--;
        }

        if( (timeout == 0) && ((UARTs[UARTx]->SR & UART_TXE_MASK) == 0))
        {
            Ret_ErrorStatus = UART_TX_TIMEOUT;
        }
    }

    return Ret_ErrorStatus;
}


UART_ErrorStatus_t UART_TXBufferAsyncZC(uint8_t UARTx, uint8_t * buffer, uint16_t length, UART_ReqCallback_t cb )
{
    UART_ErrorStatus_t Ret_ErrorStatus = UART_OK;

    if(!IS_VALID_UART(UARTx))
    {
        Ret_ErrorStatus = UART_INVALID_UART;
    }
    else if(buffer == NULL || cb == NULL)
    {
        Ret_ErrorStatus = UART_NULL_PTR;
    }
    else
    {
        /* do nothing */
    }

    if(Ret_ErrorStatus == UART_OK)
    {
       
        if(TXRequests[UARTx].State == READY)
        {
            TXRequests[UARTx].State = BUSY;
            TXRequests[UARTx].Cb = cb;

            TXRequests[UARTx].Buffer.Data = buffer;
            TXRequests[UARTx].Buffer.Length = length;

            /* Send the firts byte to start the chain */
            TXRequests[UARTx].Buffer.CurrIdx = 1;
            UARTs[UARTx]->DR = TXRequests[UARTx].Buffer.Data[0];

            /* Enable TXE interrupt */
            UARTs[UARTx]->CR1 |= UART_TXEIE_MASK;
        }
    }

    return Ret_ErrorStatus;
}


UART_ErrorStatus_t UART_RXByte(uint8_t UARTx, uint8_t *byte)
{
    UART_ErrorStatus_t Ret_ErrorStatus = UART_OK;
    uint16_t timeout = RXE_TIMEOUT_VAL;

    if(!IS_VALID_UART(UARTx))
    {
        Ret_ErrorStatus = UART_INVALID_UART;
    }
    else if(byte == NULL)
    {
        Ret_ErrorStatus = UART_NULL_PTR;
    }
    else
    {
        /* do nothing */
    }

    if(Ret_ErrorStatus == UART_OK)
    {
        /* Disable RXNE Interrupt */
        UARTs[UARTx]->CR1 &= ~UART_RXNEIE_MASK;

        /* Wait till Byte is recieved */
        while( ((UARTs[UARTx]->SR & UART_TXE_MASK) == 0) && timeout )
        {
            timeout--;
        }

        if( (timeout == 0) && ((UARTs[UARTx]->SR & UART_TXE_MASK) == 0))
        {
            Ret_ErrorStatus = UART_RX_TIMEOUT;
        }
        else
        {
            /* Get value if no timeout */
            *byte = UARTs[UARTx]->DR;
        }

    }

    return Ret_ErrorStatus;
}


UART_ErrorStatus_t UART_RXBufferAsyncZC(uint8_t UARTx, uint8_t * buffer, uint16_t length, UART_ReqCallback_t cb )
{
    UART_ErrorStatus_t Ret_ErrorStatus = UART_OK;

    if(!IS_VALID_UART(UARTx))
    {
        Ret_ErrorStatus = UART_INVALID_UART;
    }
    else if(buffer == NULL || cb == NULL)
    {
        Ret_ErrorStatus = UART_NULL_PTR;
    }
    else
    {
        /* do nothing */
    }


     if(Ret_ErrorStatus == UART_OK)
    {
        if(RXRequests[UARTx].State == READY)
        {
            RXRequests[UARTx].State = BUSY;
            RXRequests[UARTx].Cb = cb;

            RXRequests[UARTx].Buffer.Data = buffer;
            RXRequests[UARTx].Buffer.Length = length;

            RXRequests[UARTx].Buffer.CurrIdx = 0;

            /* Clear RXNE flag */
            UARTs[UARTx]->SR &= ~UART_RXNE_MASK;

            /* Enable RXNE interrupt */
            UARTs[UARTx]->CR1 |= UART_RXNEIE_MASK;

        }
    }

    return Ret_ErrorStatus;
}





/* =================================================================== */
/*                      LIN FUNCTIONALITIES                            */
/* =================================================================== */

UART_ErrorStatus_t UART_SetLINState(uint8_t UARTx, uint32_t LINState)
{
    UART_ErrorStatus_t Ret_ErrorStatus = UART_OK;

    if(!IS_VALID_UART(UARTx))
    {
        Ret_ErrorStatus = UART_INVALID_UART;
    }
    else
    {
        /* do nothing */
    }


    if(Ret_ErrorStatus == UART_OK)
    {
        UARTs[UARTx]->CR2 |= LINState;
    }

    return Ret_ErrorStatus;
}



UART_ErrorStatus_t UART_SetLINBreakDetectionLength(uint8_t UARTx, uint32_t LINBreakDetectLength)
{
    UART_ErrorStatus_t Ret_ErrorStatus = UART_OK;

    if(!IS_VALID_UART(UARTx))
    {
        Ret_ErrorStatus = UART_INVALID_UART;
    }
    else
    {
        /* do nothing */
    }


    if(Ret_ErrorStatus == UART_OK)
    {
        UARTs[UARTx]->CR2 |= LINBreakDetectLength;
    }

    return Ret_ErrorStatus;
}

UART_ErrorStatus_t UART_SetLINBreakInterruptState(uint8_t UARTx, uint32_t LINBreakInterruptState)
{
    UART_ErrorStatus_t Ret_ErrorStatus = UART_OK;

    if(!IS_VALID_UART(UARTx))
    {
        Ret_ErrorStatus = UART_INVALID_UART;
    }
    else
    {
        /* do nothing */
    }


    if(Ret_ErrorStatus == UART_OK)
    {
        UARTs[UARTx]->CR2 |= LINBreakInterruptState;
    }

    return Ret_ErrorStatus;
}


UART_ErrorStatus_t UART_TXLINBreak(uint8_t UARTx)
{
    UART_ErrorStatus_t Ret_ErrorStatus = UART_OK;

    if(!IS_VALID_UART(UARTx))
    {
        Ret_ErrorStatus = UART_INVALID_UART;
    }
    else
    {
        /* do nothing */
    }


    if(Ret_ErrorStatus == UART_OK)
    {
        /* Disable UART to be able to send the break */
//        UARTs[UARTx]->CR1 &= ~UART_UE_MASK;

        /* send break */
        UARTs[UARTx]->CR1 |= UART_LIN_SEND_BREAK;

        /* wait till break is sent */
//        while( (UARTs[UARTx]->CR1 & UART_LIN_SEND_BREAK) == 1);

        /* Enable UART again */
//        UARTs[UARTx]->CR1 |= UART_UE_MASK;
    }

    return Ret_ErrorStatus;
}

UART_ErrorStatus_t UART_SetLBDCallback(uint8_t UARTx, UART_LBDCallback_t cb)
{
    UART_ErrorStatus_t Ret_ErrorStatus = UART_OK;

    if(!IS_VALID_UART(UARTx))
    {
        Ret_ErrorStatus = UART_INVALID_UART;
    }
    else
    {
        /* do nothing */
    }


    if(Ret_ErrorStatus == UART_OK)
    {
        LBDCallbacks[UARTx] = cb;
    }

    return Ret_ErrorStatus;
}




/* =================================================================== */
/*                              HANDLERS                               */
/* =================================================================== */


void USART1_IRQHandler(void)
{
    if(UARTs[UART1]->SR & UART_TXE_MASK)
    {
        if(TXRequests[UART1_INDEX].Buffer.CurrIdx < TXRequests[UART1_INDEX].Buffer.Length)
        {
            UARTs[UART1]->DR = TXRequests[UART1_INDEX].Buffer.Data[TXRequests[UART1_INDEX].Buffer.CurrIdx];
            TXRequests[UART1_INDEX].Buffer.CurrIdx++;
        }
        else
        {
            TXRequests[UART1_INDEX].State = READY;
            /* DISABLE TXE interrupt */
            UARTs[UART1]->CR1 &= ~UART_TXEIE_MASK;

            TXRequests[UART1_INDEX].Cb();
        }
    }

    if(UARTs[UART1]->SR & UART_RXNE_MASK)
    {
        if(RXRequests[UART1_INDEX].Buffer.CurrIdx < RXRequests[UART1_INDEX].Buffer.Length)
        {
            RXRequests[UART1_INDEX].Buffer.Data[RXRequests[UART1_INDEX].Buffer.CurrIdx] = UARTs[UART1]->DR;
            RXRequests[UART1_INDEX].Buffer.CurrIdx++;

            if(RXRequests[UART1_INDEX].Buffer.CurrIdx == RXRequests[UART1_INDEX].Buffer.Length)
            {
                RXRequests[UART1_INDEX].State = READY;
                /* DISABLE RXE interrupt */
                UARTs[UART1]->CR1 &= ~UART_RXNEIE_MASK;

                RXRequests[UART1_INDEX].Cb();
            }

        }    
    }

    if(UARTs[UART1]->SR & UART_LBD_MASK)
    {
        /* Clear LBD Flag */
        UARTs[UART1]->SR &= ~UART_LBD_MASK;

        /* Call LBD Interrupt CB function if not NULL*/
        if(LBDCallbacks[UART1])
        {
            LBDCallbacks[UART1]();
        }
        
    }
} 


void USART2_IRQHandler(void)
{
    if(UARTs[UART2]->SR & UART_TXE_MASK)
    {
        if(TXRequests[UART2_INDEX].Buffer.CurrIdx < TXRequests[UART2_INDEX].Buffer.Length)
        {
            UARTs[UART2]->DR = TXRequests[UART2_INDEX].Buffer.Data[TXRequests[UART2_INDEX].Buffer.CurrIdx];
            TXRequests[UART2_INDEX].Buffer.CurrIdx++;
        }
        else
        {
            TXRequests[UART2_INDEX].State = READY;
            /* DISABLE TXE interrupt */
            UARTs[UART2]->CR1 &= ~UART_TXEIE_MASK;

            TXRequests[UART2_INDEX].Cb();
        }
    }

    if(UARTs[UART2]->SR & UART_RXNE_MASK)
    {
        if(RXRequests[UART2_INDEX].Buffer.CurrIdx < RXRequests[UART2_INDEX].Buffer.Length)
        {
            RXRequests[UART2_INDEX].Buffer.Data[RXRequests[UART2_INDEX].Buffer.CurrIdx] = UARTs[UART2]->DR;
            RXRequests[UART2_INDEX].Buffer.CurrIdx++;

            if(RXRequests[UART2_INDEX].Buffer.CurrIdx == RXRequests[UART2_INDEX].Buffer.Length)
            {
                RXRequests[UART2_INDEX].State = READY;
                /* DISABLE RXE interrupt */
                UARTs[UART2]->CR1 &= ~UART_RXNEIE_MASK;

                RXRequests[UART2_INDEX].Cb();
            }

        }    
    }

    if(UARTs[UART2]->SR & UART_LBD_MASK)
        {
            /* Clear LBD Flag */
            UARTs[UART2]->SR &= ~UART_LBD_MASK;

            /* Call LBD Interrupt CB function if not NULL*/
            if(LBDCallbacks[UART2])
            {
                LBDCallbacks[UART2]();
            }

        }
} 


void USART6_IRQHandler(void)
{
    if(UARTs[UART6]->SR & UART_TXE_MASK)
    {
        if(TXRequests[UART6_INDEX].Buffer.CurrIdx < TXRequests[UART6_INDEX].Buffer.Length)
        {
            UARTs[UART6]->DR = TXRequests[UART6_INDEX].Buffer.Data[TXRequests[UART6_INDEX].Buffer.CurrIdx];
            TXRequests[UART6_INDEX].Buffer.CurrIdx++;
        }
        else
        {
            TXRequests[UART6_INDEX].State = READY;
            /* DISABLE TXE interrupt */
            UARTs[UART6]->CR1 &= ~UART_TXEIE_MASK;

            TXRequests[UART6_INDEX].Cb();
        }
    }

    if(UARTs[UART6]->SR & UART_RXNE_MASK)
    {
        if(RXRequests[UART6_INDEX].Buffer.CurrIdx < RXRequests[UART6_INDEX].Buffer.Length)
        {
            RXRequests[UART6_INDEX].Buffer.Data[RXRequests[UART6_INDEX].Buffer.CurrIdx] = UARTs[UART6]->DR;
            RXRequests[UART6_INDEX].Buffer.CurrIdx++;

            if(RXRequests[UART6_INDEX].Buffer.CurrIdx == RXRequests[UART6_INDEX].Buffer.Length)
            {
                RXRequests[UART6_INDEX].State = READY;
                /* DISABLE TXE interrupt */
                UARTs[UART6]->CR1 &= ~UART_RXNEIE_MASK;

                RXRequests[UART6_INDEX].Cb();
            }

        }    
    }

    if(UARTs[UART6]->SR & UART_LBD_MASK)
        {
            /* Clear LBD Flag */
            UARTs[UART6]->SR &= ~UART_LBD_MASK;

            /* Call LBD Interrupt CB function if not NULL*/
            if(LBDCallbacks[UART6])
            {
                LBDCallbacks[UART6]();
            }

        }
} 




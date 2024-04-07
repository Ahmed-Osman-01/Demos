/******************************************************************************
*
* Module: USART
*
* File Name: USART.h
*
* Description: Header file for the USART driver for STM32F401xC
*
* Author: Momen Elsayed Shaban
*
*******************************************************************************/

#ifndef D__ITI_STM32F401CC_DRIVERS_INC_MCAL_UART_USART_H_
#define D__ITI_STM32F401CC_DRIVERS_INC_MCAL_UART_USART_H_
/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "LIB/std_types.h"
#include "MCAL/USART/USART_Cfg.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define USART_NUMBER_1                  0U
#define USART_NUMBER_2                  1U
#define USART_NUMBER_6                  2U

#define USART_WORD_LEN_8                0x00000000 
#define USART_WORD_LEN_9                0x00010000 

#define USART_OVERSAMPLING_16           0x00000000
#define USART_OVERSAMPLING_8            0x00008000

#define USART_PARITY_CONTROL_DISABLE    0x00000000
#define USART_PARITY_CONTROL_ENABLE     0x00000400

#define USART_PARITY_EVEN               0x00000000
#define USART_PARITY_ODD                0x00000200

#define USART_STOPBITS_1                0x00000000
#define USART_STOPBITS_HALF             0x00001000
#define USART_STOP_BITS_2               0x00002000
#define USART_STOP_BITS_1_HALF          0x00003000



/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
typedef void (*CallBack_t)(void);

typedef struct{
    u8 USART_Number;
    u32 USART_BaudRate;
    u32 USART_WordLen;
    u32 USART_OverSampling;
    u32 USART_ParityControl;
    u32 USART_ParitySelection;
    u32 USART_StopBits;
}USART_Cfg_t;

typedef struct
{
    u8 USART_Number;
    u8 *data;
    u16 length;
    CallBack_t CB;
}USART_Req_t;

typedef enum{
    USART_OK,
    USART_InvalidNumber,
    USART_InvalidBaudRate,
    USART_NullPtr,
    USART_Busy,
    USART_TimeOut
}USART_ErrorStatus_t;


/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/
USART_ErrorStatus_t USART_init(void);

USART_ErrorStatus_t USART_sendByte(USART_Req_t USART_Req);

USART_ErrorStatus_t USART_recieveByte(USART_Req_t USART_Req);

USART_ErrorStatus_t USART_sendBufferAsyncZC(USART_Req_t USART_Req);

USART_ErrorStatus_t USART_recieveBufferAsyncZC(USART_Req_t USART_Req);

#endif // D__ITI_STM32F401CC_DRIVERS_INC_MCAL_UART_USART_H_
#ifndef USART_DRIVER_
#define USART_DRIVER_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "LIB/std_types.h"
#include "MCAL/USART/USART_cfg.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

#define USART_Peri_1 0
#define USART_Peri_2 1
#define USART_Peri_6 2

#define USART_WORD_LENGTH_8 0x00000000
#define USART_WORD_LENGTH_9 0x00010000
#define USART_PARITY_DISABLE 0x00000000
#define USART_PARITY_ENABLE 0x00000400
#define USART_PARITY_EVEN 0x00000000
#define USART_PARITY_ODD 0x00000200
#define USART_STOP_BITS_1 0x0000000
#define USART_STOP_BITS_1_5 0x00001000
#define USART_STOP_BITS_2 0x00002000
#define USART_OVERSAMPLING_16 0x00000000
#define USART_OVERSAMPLING_8 0x00008000
#define USART_LIN_MODE_ENABLE 0x00004000
#define USART_LIN_MODE_DISABLE 0x00000000
#define USART_LIN_IRQ_ENABLE 0x00000040
#define USART_LIN_IRQ_DISABLE 0x00000000
#define USART_LIN_BRK_LENGTH_10 0x00000000
#define USART_LIN_BRK_LENGTH_11 0x00000020

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
typedef void (*CallBack_t)(void);

typedef struct
{
    uint32_t address;
    uint32_t BaudRate;
    uint32_t WordLength;
    uint32_t ParityControl;
    uint32_t ParitySelect;
    uint32_t StopBits;
    uint32_t OverSampling;
} USART_cfg_t;

typedef struct
{
    uint8_t USART_Peri;
    uint32_t LIN_Mode;
    uint32_t LIN_IRQ;
    uint32_t LIN_BreakLength;
} USART_LIN_cfg_t;

typedef struct
{
    uint8_t USART_Peri;
    uint8_t *buffer;
    uint16_t length;
    CallBack_t CB;
} USART_Req_t;

/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

/*
 * use this function to initialize the USART peripherals
 * parameters:
 * USART_CfgArr -> the configurtion array of the desired USART
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status USART_Init(USART_cfg_t USART_CfgArr);

/*
 * use this function to send a byte synchronously through USART
 * parameters:
 * USART_Req -> the desired buffer to be sent
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_USART_TimeOut
 */
Error_Status USART_SendByte(USART_Req_t USART_Req);

/*
 * use this function to recieve a byte synchronously through USART
 * parameters:
 * USART_Req -> the desired buffer to be recieved
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_USART_TimeOut
 */
Error_Status USART_GetByte(USART_Req_t USART_Req);

/*
 * use this function to send a buffer of bytes asynchronously through USART
 * parameters:
 * USART_Req -> the desired buffer to be sent
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_USART_Busy_Buffer
 */
Error_Status USART_TXBufferAsyncZC(USART_Req_t USART_Req);

/*
 * use this function to recieve a buffer of bytes asynchronously through USART
 * parameters:
 * USART_Req -> the desired buffer to be recieved
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_USART_Busy_Buffer
 */
Error_Status USART_RXBufferAsyncZC(USART_Req_t USART_Req);

/*
 * use this function to initialize the LIN parameters in USART peripherals
 * parameters:
 * USART_LIN_CfgArr -> the LIN configurtion array of the desired USART
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status USART_LIN_Init(USART_LIN_cfg_t USART_LIN_CfgArr);

/*
 * use this function to generate a break condition on the USART
 * parameters:
 * USART_Peri -> the address of the USART peripheral to be used
 *             - USART_Peri_1, USART_Peri_2, USART_Peri_6
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_USART_Busy_Buffer
 */
Error_Status USART_GenerateBreak(uint8_t USART_Peri);

/*
 * use this function to set the callback function to be executed after a break condition
 * parameters:
 * USART_Peri -> the number of the USART peripheral to be assigned a callback
 *             - USART_Peri_1, USART_Peri_2, USART_Peri_6
 * CB         -> the address of the desired call back function
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_USART_Busy_Buffer
 */
Error_Status USART_Set_BreakCallBack(uint8_t USART_Peri, CallBack_t CB);

#endif // USART_DRIVER_
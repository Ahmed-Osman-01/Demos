/******************************************************************************
*
* Module: USART
*
* File Name: USART_Cfg.c
*
* Description: Source file for the USART Configuration driver for STM32F401xC
*
* Author: Momen Elsayed Shaban
*
*******************************************************************************/

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "MCAL/USART/USART.h"

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
const USART_Cfg_t USART_Cfg[_USART_Num] = {
    [USART1]={
        .USART_Number = USART_NUMBER_1,
        .USART_BaudRate = 9600,
        .USART_WordLen = USART_WORD_LEN_8,
        .USART_OverSampling = USART_OVERSAMPLING_8,
        .USART_ParityControl = USART_PARITY_CONTROL_DISABLE,
        .USART_ParitySelection = USART_PARITY_CONTROL_DISABLE,
        .USART_StopBits = USART_STOPBITS_1
    }
};






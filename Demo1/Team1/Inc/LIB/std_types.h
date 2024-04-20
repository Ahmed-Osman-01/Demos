/*
 * STD_Types.h
 *
 *  Created on: Feb 5, 2024
 *      Author: Moamen_Hamed
 */

#ifndef STD_TYPES_H_
#define STD_TYPES_H_

#define NULL_PTR ((void *)0)
#define NULL ((void *)0)

typedef enum
{
    Status_NOK,
    Status_OK,
    Status_Null_Pointer,
    Status_Invalid_Input,
    Status_LCD_Full_Buffer,
    Status_USART_TimeOut,
    Status_USART_Busy_Buffer,
    Status_LIN_OUT_SYNC,
    Status_LIN_WRONG_CHK_SUM
} Error_Status;

typedef signed char sint8_t;
typedef unsigned char uint8_t;
typedef signed short int sint16_t;
typedef unsigned short int uint16_t;
typedef signed int sint32_t;
typedef unsigned int uint32_t;
typedef signed long long sint64_t;
typedef unsigned long long uint64_t;
typedef float float32_t;
typedef double float64_t;

#endif

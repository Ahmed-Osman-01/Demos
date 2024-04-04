/*
 * STD_Types.h
 *
 *  Created on: Feb 5, 2024
 *      Author: Moamen_Hamed
 */

#ifndef STD_TYPES_H_
#define STD_TYPES_H_

#define NULL ((void *)0)

typedef enum
{
    Status_NOK,
    Status_OK,
    Status_Null_Pointer,
    Status_Invalid_Input,
    Status_LCD_Full_Buffer,
    Status_USART_TimeOut,
    Status_USART_Busy_Buffer
} Error_Status;

typedef signed char s8_t;
typedef unsigned char u8_t;
typedef signed short int s16_t;
typedef unsigned short int u16_t;
typedef signed int s32_t;
typedef unsigned int u32_t;
typedef signed long long s64_t;
typedef unsigned long long u64_t;
typedef float f32_t;
typedef double f64_t;

#endif

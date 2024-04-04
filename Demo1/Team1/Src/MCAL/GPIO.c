/******************************************************************************
 *
 * Module: GPIO
 *
 * File Name: GPIO.c
 *
 * Description: Source file for the GPIO driver Configurations for STM32F401xC
 *
 * Author: Momen Elsayed Shaban
 *
 *******************************************************************************/

#include "MCAL/GPIO/GPIO.h"

#define GPIO_CLR_MASK				0x00000003U
#define GPIO_MODE_MASK				0x00000003U		/*Input,output,AF or AN*/
#define GPIO_TYPE_MASK				0x00000001U		/*PushPull or OpenDrain*/
#define GPIO_PUPD_MASK				0x00000018U
#define GPIO_BSRR_RESET_OFFSET		16U
#define GPIO_AFR_CLEAR_MASK 		0x0000000FU

#define IS_GPIO_MODE(MODE) (((MODE) == GPIO_MODE_IN_FL)               ||\
                            ((MODE) == GPIO_MODE_IN_PU)               ||\
                            ((MODE) == GPIO_MODE_IN_PD)               ||\
                            ((MODE) == GPIO_MODE_OUT_PP)              ||\
                            ((MODE) == GPIO_MODE_OUT_PP_PU)           ||\
                            ((MODE) == GPIO_MODE_OUT_PP_PD)           ||\
                            ((MODE) == GPIO_MODE_OUT_OD)              ||\
                            ((MODE) == GPIO_MODE_OUT_OD_PU)           ||\
                            ((MODE) == GPIO_MODE_OUT_OD_PD)           ||\
                            ((MODE) == GPIO_MODE_AF_PP)               ||\
                            ((MODE) == GPIO_MODE_AF_PP_PU)            ||\
                            ((MODE) == GPIO_MODE_AF_PP_PD)            ||\
                            ((MODE) == GPIO_MODE_AF_OD)               ||\
                            ((MODE) == GPIO_MODE_AF_OD_PU)            ||\
                            ((MODE) == GPIO_MODE_AF_OD_PD)            ||\
                            ((MODE) == GPIO_MODE_AN))

typedef struct{
	volatile u32 MODER;
	volatile u32 OTYPER;
	volatile u32 OSPEEDR;
	volatile u32 PUPDR;
	volatile u32 IDR;
	volatile u32 ODR;
	volatile u32 BSRR;
	volatile u32 LCKR;
	volatile u32 AFRL;
	volatile u32 AFRH;
}GPIO_Registers_t;

GPIO_ErrorStatus_t GPIO_Init(GPIO_Pin_t* GPIOx)
{
	GPIO_ErrorStatus_t Error_status = GPIO_OK;
	if(GPIOx->GPIO_Pin > GPIO_PIN_15)
	{
		Error_status = GPIO_InvalidPin;
	}
	else if(GPIOx->GPIO_Port > GPIO_PORT_C)
	{
		Error_status = GPIO_InvalidPort;
	}
	else if(!(IS_GPIO_MODE(GPIOx->GPIO_Mode)))
	{
		Error_status = GPIO_InvalidMode;
	}
	else if(GPIOx->GPIO_Speed > GPIO_SPEED_VHIGH)
	{
		Error_status = GPIO_InvalidSpeed;
	}
	else{
		/*Do Nothing*/
	}

	if(Error_status == GPIO_OK)
	{
		u32 loc_temp_MODER = ((GPIO_Registers_t*)GPIOx->GPIO_Port)->MODER;
		u32 loc_temp_PUPDR = ((GPIO_Registers_t*)GPIOx->GPIO_Port)->PUPDR;
		u32 loc_temp_OSPEEDR = ((GPIO_Registers_t*)GPIOx->GPIO_Port)->OSPEEDR;
		/**********************************Configure MODER**************************************/
		loc_temp_MODER &= ~(GPIO_CLR_MASK << (2U * GPIOx->GPIO_Pin));		/*Each Pin has 2 bits in the MODER*/
		loc_temp_MODER |= (GPIOx->GPIO_Mode & GPIO_MODE_MASK) << (2U * GPIOx->GPIO_Pin);
		((GPIO_Registers_t*)GPIOx->GPIO_Port)->MODER = loc_temp_MODER;

		/**********************************Configure OTYPER**************************************/
		((GPIO_Registers_t*)GPIOx->GPIO_Port)->OTYPER |= ((GPIOx->GPIO_Mode & GPIO_TYPE_MASK) >> 2U) << (GPIOx->GPIO_Pin);

		/**********************************Configure PU/PD**************************************/
		loc_temp_PUPDR &= ~(GPIO_CLR_MASK << (2U * GPIOx->GPIO_Pin));
		loc_temp_PUPDR |= ((GPIOx->GPIO_Mode & GPIO_PUPD_MASK) >> 3U) << (2U * GPIOx->GPIO_Pin);	/*Fourth bit contains the info of PU and PD in the MODE Value so shifting by 3*/
		((GPIO_Registers_t*)GPIOx->GPIO_Port)->PUPDR = loc_temp_PUPDR;

		/**********************************Configure Speed**************************************/
		loc_temp_OSPEEDR &= ~(GPIO_CLR_MASK << (2U * GPIOx->GPIO_Pin));
		loc_temp_OSPEEDR |= (GPIOx->GPIO_Speed) << (2U * GPIOx->GPIO_Pin);
		((GPIO_Registers_t*)GPIOx->GPIO_Port)->OSPEEDR = loc_temp_OSPEEDR;
	}
	return Error_status;
}



GPIO_ErrorStatus_t GPIO_setPinValue(void* GPIO_Port, u8 GPIO_pin, u8 GPIO_State)
{
	GPIO_ErrorStatus_t Error_status = GPIO_OK;
	if(GPIO_Port == NULL_PTR)
	{
		Error_status = GPIO_NULLPTR;
	}
	else if (GPIO_pin > GPIO_PIN_15)
	{
		Error_status = GPIO_InvalidPin;
	}
	else{
		/*Do Nothing*/
	}
	if (Error_status == GPIO_OK)
	{
		switch (GPIO_State)
		{
		case GPIO_STATE_SET:
			((GPIO_Registers_t *)GPIO_Port)->BSRR = GPIO_STATE_SET << GPIO_pin;
			break;
		
		case GPIO_STATE_RESET:
			((GPIO_Registers_t *)GPIO_Port)->BSRR = 1 << (GPIO_pin + GPIO_BSRR_RESET_OFFSET);
			break;

		default:
			Error_status = GPIO_InvalidState;
			break;
		}
	}
	return Error_status;
}


GPIO_ErrorStatus_t GPIO_getPinValue(void* GPIO_Port, u8 GPIO_pin, u8* GPIO_State)
{
	GPIO_ErrorStatus_t Error_status = GPIO_OK;
	if(GPIO_Port == NULL_PTR || GPIO_State == NULL_PTR)
	{
		Error_status = GPIO_NULLPTR;
	}
	else if (GPIO_pin > GPIO_PIN_15)
	{
		Error_status = GPIO_InvalidPin;
	}
	else{
		/*Do Nothing*/
	}
	if(Error_status == GPIO_OK)
	{
		*GPIO_State = (((GPIO_Registers_t*)GPIO_Port)->IDR & (1 << GPIO_pin)) ? GPIO_STATE_SET : GPIO_STATE_RESET;
	}
	return Error_status;
}

GPIO_ErrorStatus_t GPIO_CfgAlternateFn(void* GPIO_Port, u32 GPIO_Pin, u32 GPIO_AF)
{
	GPIO_ErrorStatus_t Error_status = GPIO_OK;
	u32 AFR_value = 0;
	if(GPIO_Port == NULL_PTR)
	{
		Error_status = GPIO_NULLPTR;
	}
	else if (GPIO_Pin > GPIO_PIN_15)
	{
		Error_status = GPIO_InvalidPin;
	}
	else{
		if(GPIO_Pin > GPIO_PIN_7)
		{
			AFR_value = ((GPIO_Registers_t*)GPIO_Port)->AFRH;
			AFR_value &= ~(GPIO_AFR_CLEAR_MASK << ((GPIO_Pin - 8) * 4U));
			AFR_value |= (GPIO_AF << ((GPIO_Pin - 8) * 4U));
			((GPIO_Registers_t*)GPIO_Port)->AFRH = AFR_value;
		}
		else
		{
			AFR_value = ((GPIO_Registers_t*)GPIO_Port)->AFRL;
			AFR_value &= ~(GPIO_AFR_CLEAR_MASK << ((GPIO_Pin) * 4U));
			AFR_value |= (GPIO_AF << ((GPIO_Pin) * 4U));
			((GPIO_Registers_t*)GPIO_Port)->AFRL = AFR_value;
		}
	}
	return Error_status;
}

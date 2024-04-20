/******************************************************************************
 *
 * Module: SysTick
 *
 * File Name: SYSTICK.c
 *
 * Description: Source file for the SysTick driver for STM32F401xC
 *
 * Author: Momen Elsayed Shaban
 *
 *******************************************************************************/
#include "MCAL/SYSTICK/SYSTICK.h"


#define SYSTICK_BASE_ADDR			0xE000E010
#define SYSTICK_START_MASK			0xFFFFFFF8
#define SYSTICK_MAX_LOAD_VAL		0x00FFFFFF
#define SYSTICK_MAX_CALL_BACK_FN	3

typedef struct{
	volatile uint32_t STK_CTRL;
	volatile uint32_t STK_LOAD;
	volatile uint32_t STK_VAL;
	volatile uint32_t STK_CALIB;
}SYSTICK_Registers_t;


static SYSTICK_Registers_t* const SYSTICK = (SYSTICK_Registers_t*)SYSTICK_BASE_ADDR;


static SYSTICKCallBackFn_t App_CBF[SYSTICK_MAX_CALL_BACK_FN];

SYSTICK_ErrorStatus_t SYSTICK_start(uint32_t SYSTICK_Clk)
{
	SYSTICK_ErrorStatus_t Error_Status = SYSTICK_OK;
	uint32_t Systick_ctrl_reg = 0;
	if((SYSTICK_Clk != SYSTICK_CLK_AHB) && (SYSTICK_Clk != SYSTICK_CLK_AHB_DIV_8))
	{
		Error_Status = SYSTICK_InvalidClkSrcValue;
	}
	else
	{
		Systick_ctrl_reg = SYSTICK->STK_CTRL;
		Systick_ctrl_reg &= SYSTICK_START_MASK;
		Systick_ctrl_reg |= SYSTICK_Clk;
		SYSTICK->STK_CTRL = Systick_ctrl_reg;
	}
	return Error_Status;
}

SYSTICK_ErrorStatus_t SYSTICK_setTimeMS(uint32_t timeMS)
{
	SYSTICK_ErrorStatus_t Error_Status = SYSTICK_OK;
	uint32_t loadValue = ((SYSTICK_CLK_VALUE / 1000) * timeMS) - 1;
	if (loadValue > SYSTICK_MAX_LOAD_VAL)
	{
		Error_Status = SYSTICK_InvalidTicksValue;
	}
	else
	{
		SYSTICK->STK_LOAD = loadValue;
	}
	return Error_Status;
}

SYSTICK_ErrorStatus_t SYSTICK_Stop()
{
	SYSTICK_ErrorStatus_t Error_Status = SYSTICK_OK;
	SYSTICK->STK_CTRL = 0;
	return Error_Status;
}

SYSTICK_ErrorStatus_t SYSTICK_setCallBack(SYSTICKCallBackFn_t SYSTICK_CBF, uint8_t req_Index)
{
	SYSTICK_ErrorStatus_t Error_Status = SYSTICK_OK;
	if(req_Index > SYSTICK_MAX_CALL_BACK_FN)
	{
		Error_Status = SYSTICK_InvalidCallBackIndex;
	}
	else if (!SYSTICK_CBF)
	{
		Error_Status = SYSTICK_NullPtr;
	}
	else
	{
		App_CBF[req_Index] = SYSTICK_CBF;
	}
	return Error_Status;
}

void SysTick_Handler(void)
{
	uint8_t iterator = 0;
	for(iterator = 0 ; iterator < SYSTICK_MAX_CALL_BACK_FN ; iterator++)
	{
		if(App_CBF[iterator])
		{
			App_CBF[iterator]();
		}
	}

}



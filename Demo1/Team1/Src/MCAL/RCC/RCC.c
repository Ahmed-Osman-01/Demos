/******************************************************************************
 *
 * Module: RCC
 *
 * File Name: RCC.c
 *
 * Description: Source file for the RCC driver Configurations for STM32F401xC
 *
 * Author: Momen Elsayed Shaban
 *
 *******************************************************************************/
#include "MCAL/RCC/RCC.h"


/*******************************************************************************
 *                                Type Decelerations                           *
 *******************************************************************************/
#define RCC_PLLCFGR_MNPQ_MASK	0xF0FC8000	/*Mask for clean PLL configurations for M,N,P and Q*/
#define RCC_CFGR_SYSCLK_MASK	0xFFFFFFFC	/*Mask for set Sysclk*/
#define RCC_CFGR_R_SYSCLK_MASK	0x00000003	/*Mask for read Sysclk*/
#define RCC_CFGR_HPRE    		4   		/* Position of HPRE bits in RCC_CFGR register */
#define RCC_CFGR_PPRE1   		10  		/* Position of PPRE1 bits in RCC_CFGR register */
#define RCC_CFGR_PPRE2   		13  		/* Position of PPRE2 bits in RCC_CFGR register */
#define RCC_PLLCFGR_PLLM   		0   		/* Position of PLLM bits in RCC_PLLCFGR register */
#define RCC_PLLCFGR_PLLN    	6   		/* Position of PLLN bits in RCC_PLLCFGR register */
#define RCC_PLLCFGR_PLLP    	16  		/* Position of PLLP bits in RCC_PLLCFGR register */
#define RCC_PLLCFGR_PLLSRC  	22			/* Position of PLLSRC bit in RCC_PLLCFGR register */
#define RCC_PLLCFGR_PLLQ    	24  		/* Position of PLLQ bits in RCC_PLLCFGR register */
#define RCC_BASE_ADDR       	0x40023800 /* RCC Base Address */

typedef struct
{
	volatile uint32_t CR;
	volatile uint32_t PLLCFGR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t AHB1RSTR;
	volatile uint32_t AHB2RSTR;
	volatile uint32_t Reserved[2];
	volatile uint32_t APB1RSTR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t Reserved1[2];
	volatile uint32_t AHB1ENR;
	volatile uint32_t AHB2ENR;
	volatile uint32_t Reserved2[2];
	volatile uint32_t APB1ENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t AHB1LPENR;
	volatile uint32_t AHB2LPENR;
	volatile uint32_t Reserved3[2];
	volatile uint32_t APB1LPENR;
	volatile uint32_t APB2LPENR;
	volatile uint32_t Reserved4[2];
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	volatile uint32_t Reserved5[2];
	volatile uint32_t SSCGR;
	volatile uint32_t PLLI2SCFGR;
	volatile uint32_t RESERVED6;
	volatile uint32_t DCKCFGR;
}Rcc_Registers_t;

/*******************************************************************************
 *                                Variables			                           *
 *******************************************************************************/
static Rcc_Registers_t* const RCC = (Rcc_Registers_t*)RCC_BASE_ADDR;


/*******************************************************************************
 *                             Functions Declerations                          *
 *******************************************************************************/

RCC_enuErrorState_t RCC_setClkON(uint32_t RCC_clk)
{
	RCC_enuErrorState_t RCC_ErrorState = RCC_enuOK;
	if ( (RCC_clk != RCC_HSE_ON) && (RCC_clk != RCC_HSE_BYP_ON) && (RCC_clk != RCC_HSI_ON) \
	   	 && (RCC_clk != RCC_PLL_ON) && (RCC_clk != RCC_PLLI2S_ON))
	{
		RCC_ErrorState = RCC_enuInvalidClk;
	}
	else
	{
		/*Do Nothing*/
	}
	if (RCC_ErrorState == RCC_enuOK)
	{
		RCC->CR |= RCC_clk;
	}
	else
	{
		/*Do Nothing*/
	}
	return RCC_ErrorState;
}

RCC_enuErrorState_t RCC_getClkStatus(uint32_t RCC_clk, uint32_t* RCC_clkStatus)
{
	RCC_enuErrorState_t RCC_ErrorState = RCC_enuOK;
	if ( (RCC_clk != RCC_HSE_RDY) && (RCC_clk != RCC_HSI_RDY) && (RCC_clk != RCC_PLL_RDY) \
	   	 && (RCC_clk != RCC_PLLI2S_RDY))
	{
		RCC_ErrorState = RCC_enuInvalidClk;
	}
	else
	{
		/*Do Nothing*/
	}
	if(RCC_ErrorState == RCC_enuOK)
	{
		*RCC_clkStatus = RCC -> CR & RCC_clk;
	}
	else
	{
		/*Do Nothing*/
	}
	return RCC_ErrorState;
}

RCC_enuErrorState_t RCC_setPLLClkSrc(uint32_t RCC_PLLClk)
{
	RCC_enuErrorState_t RCC_ErrorState = RCC_enuOK;
	if( (RCC_PLLClk != RCC_PLL_CLK_HSE) && (RCC_PLLClk != RCC_PLL_CLK_HSI))
	{
		RCC_ErrorState = RCC_enuInvalidClkSrcForPLL;
	}
	else
	{
		/*Do Nothing*/
	}
	if(RCC_ErrorState == RCC_enuOK)
	{
		RCC -> PLLCFGR |= RCC_PLLClk;
	}
	else
	{
		/*Do Nothing*/
	}
	return RCC_ErrorState;
}


RCC_enuErrorState_t RCC_cfgPLLClk(uint8_t PLL_M, uint16_t PLL_N, uint8_t PLL_Q, uint8_t PLL_P)
{
	RCC_enuErrorState_t RCC_ErrorState = RCC_enuOK;
	if(PLL_M < 2 || PLL_M > 63)
	{
		RCC_ErrorState = RCC_enuInvalid_M_Value;
	}
	else if (PLL_N < 192 || PLL_N > 432)
	{
		RCC_ErrorState = RCC_enuInvalid_N_Value;
	}
	else if (PLL_Q < 2 || PLL_Q > 15)				/*Check if PLL is on or not*/
	{
		RCC_ErrorState = RCC_enuInvalid_Q_Value;
	}
	else if (PLL_P != 2 && PLL_P != 4 && PLL_P != 6 && PLL_P != 8)
	{
		RCC_ErrorState = RCC_enuInvalid_P_Value;
	}
	if(RCC_ErrorState == RCC_enuOK)
	{
		uint32_t loc_temp_RCC_PLLCFGR = RCC->PLLCFGR;					/*Save the Value of the register in case of any interrupt during this function*/
		loc_temp_RCC_PLLCFGR &= RCC_PLLCFGR_MNPQ_MASK;				/*Reset all the values of the register (M-N-Q-P)*/
		loc_temp_RCC_PLLCFGR |= (PLL_M << RCC_PLLCFGR_PLLM) | (PLL_N << RCC_PLLCFGR_PLLN) | (PLL_Q << RCC_PLLCFGR_PLLQ) \
								| (((2U * PLL_P) + 2U) << RCC_PLLCFGR_PLLP );	/*Converting from 0 in the register to input user 2 0->2 , 1->4 ,...*/
		RCC->PLLCFGR = loc_temp_RCC_PLLCFGR;
	}
	else
	{
		/*Do Nothing*/
	}
	return RCC_ErrorState;
}

RCC_enuErrorState_t RCC_setSystemClk(uint32_t RCC_Sysclk)
{
	RCC_enuErrorState_t RCC_ErrorState = RCC_enuOK;
	if ( (RCC_Sysclk != RCC_SYSCLK_HSE) && (RCC_Sysclk != RCC_SYSCLK_HSI) && (RCC_Sysclk != RCC_SYSCLK_PLL))
	{
		RCC_ErrorState = RCC_enuInvalidSysClk;
	}
	else
	{
		uint32_t loc_RCC_CFGR_temp = RCC -> CFGR;
		loc_RCC_CFGR_temp &= RCC_CFGR_SYSCLK_MASK;
		loc_RCC_CFGR_temp |= RCC_Sysclk;
		RCC -> CFGR = loc_RCC_CFGR_temp;
	}
	return RCC_ErrorState;
}


RCC_enuErrorState_t RCC_getSystemClk(uint32_t* RCC_Sysclk)
{
	RCC_enuErrorState_t RCC_ErrorState = RCC_enuOK;
	if (RCC_Sysclk != NULL_PTR)
	{
		*RCC_Sysclk = (RCC -> CFGR) & RCC_CFGR_R_SYSCLK_MASK;
	}
	else
	{
		/*Do Nothing*/
	}
	return RCC_ErrorState;	/*always return an Ok but for consistency*/
}

RCC_enuErrorState_t RCC_Set_AHB1_Prescaler(uint8_t Copy_AHB1_Prescaler)
{
	RCC_enuErrorState_t RCC_ErrorState = RCC_enuOK;
	if(Copy_AHB1_Prescaler > RCC_AHB_PRESCALER_512)	/*Condition Is not totally right because some numbers in the middle falls*/
	{
		RCC_ErrorState = RCC_enuInvalidPrescaler;
	}
	else
	{
		/*Do Nothing*/
	}
	if(RCC_ErrorState == RCC_enuOK)
	{
		RCC -> CFGR |= (Copy_AHB1_Prescaler << RCC_CFGR_HPRE);
	}
	else
	{
		/*Do Nothing*/
	}
	return RCC_ErrorState;
}


RCC_enuErrorState_t RCC_Ctrl_AHB1_Clk(uint32_t Copy_uint32_tPeriphral, RCC_enuPeriphralMode_t RCC_enuPeriphralMode)
{
	RCC_enuErrorState_t RCC_ErrorState = RCC_enuOK;
	if (RCC_enuPeriphralMode == RCC_enuPeriphralEnable)
	{
		RCC -> AHB1ENR |= Copy_uint32_tPeriphral;
	}
	else if (RCC_enuPeriphralMode == RCC_enuPeriphralDisable)
	{
		RCC -> AHB1ENR &= ~Copy_uint32_tPeriphral;
	}
	else
	{
		RCC_ErrorState = RCC_enuInvalidPeriphralMode;
	}
	return RCC_ErrorState;
}

RCC_enuErrorState_t RCC_Ctrl_AHB2_Clk(uint32_t Copy_uint32_tPeriphral, RCC_enuPeriphralMode_t RCC_enuPeriphralMode)
{
	RCC_enuErrorState_t RCC_ErrorState = RCC_enuOK;
	if (RCC_enuPeriphralMode == RCC_enuPeriphralEnable)
	{
		RCC -> AHB2ENR |= Copy_uint32_tPeriphral;
	}
	else if (RCC_enuPeriphralMode == RCC_enuPeriphralDisable)
	{
		RCC -> AHB2ENR &= ~Copy_uint32_tPeriphral;
	}
	else
	{
		RCC_ErrorState = RCC_enuInvalidPeriphralMode;
	}
	return RCC_ErrorState;
}

RCC_enuErrorState_t RCC_Set_APB1_Prescaler(uint8_t Copy_APB1_Prescaler)
{
	RCC_enuErrorState_t RCC_ErrorState = RCC_enuOK;
	if(Copy_APB1_Prescaler > RCC_APB1_PRESCALER_16)	/*Condition Is not totally right because some numbers in the middle falls*/
	{
		RCC_ErrorState = RCC_enuInvalidPrescaler;
	}
	else
	{
		/*Do Nothing*/
	}
	if(RCC_ErrorState == RCC_enuOK)
	{
		RCC -> CFGR |= (Copy_APB1_Prescaler << RCC_CFGR_PPRE1);
	}
	else
	{
		/*Do Nothing*/
	}
	return RCC_ErrorState;
}

RCC_enuErrorState_t RCC_Ctrl_APB1_Clk(uint32_t Copy_uint32_tPeriphral, RCC_enuPeriphralMode_t RCC_enuPeriphralMode)
{
	RCC_enuErrorState_t RCC_ErrorState = RCC_enuOK;
	if (RCC_enuPeriphralMode == RCC_enuPeriphralEnable)
	{
		RCC -> APB1ENR |= Copy_uint32_tPeriphral;
	}
	else if (RCC_enuPeriphralMode == RCC_enuPeriphralDisable)
	{
		RCC -> APB1ENR &= ~Copy_uint32_tPeriphral;
	}
	else
	{
		RCC_ErrorState = RCC_enuInvalidPeriphralMode;
	}
	return RCC_ErrorState;
}

RCC_enuErrorState_t RCC_Set_APB2_Prescaler(uint8_t Copy_APB2_Prescaler)
{
	RCC_enuErrorState_t RCC_ErrorState = RCC_enuOK;
	if(Copy_APB2_Prescaler > RCC_APB2_PRESCALER_16)	/*Condition Is not totally right because some numbers in the middle falls*/
	{
		RCC_ErrorState = RCC_enuInvalidPrescaler;
	}
	else
	{
		/*Do Nothing*/
	}
	if(RCC_ErrorState == RCC_enuOK)
	{
		RCC -> CFGR |= (Copy_APB2_Prescaler << RCC_CFGR_PPRE2);
	}
	else
	{
		/*Do Nothing*/
	}
	return RCC_ErrorState;
}

RCC_enuErrorState_t RCC_Ctrl_APB2_Clk(uint32_t Copy_uint32_tPeriphral, RCC_enuPeriphralMode_t RCC_enuPeriphralMode)
{
	RCC_enuErrorState_t RCC_ErrorState = RCC_enuOK;
	if (RCC_enuPeriphralMode == RCC_enuPeriphralEnable)
	{
		RCC -> APB2ENR |= Copy_uint32_tPeriphral;
	}
	else if (RCC_enuPeriphralMode == RCC_enuPeriphralDisable)
	{
		RCC -> APB2ENR &= ~Copy_uint32_tPeriphral;
	}
	else
	{
		RCC_ErrorState = RCC_enuInvalidPeriphralMode;
	}
	return RCC_ErrorState;
}

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
	volatile u32 CR;
	volatile u32 PLLCFGR;
	volatile u32 CFGR;
	volatile u32 CIR;
	volatile u32 AHB1RSTR;
	volatile u32 AHB2RSTR;
	volatile u32 Reserved[2];
	volatile u32 APB1RSTR;
	volatile u32 APB2RSTR;
	volatile u32 Reserved1[2];
	volatile u32 AHB1ENR;
	volatile u32 AHB2ENR;
	volatile u32 Reserved2[2];
	volatile u32 APB1ENR;
	volatile u32 APB2ENR;
	volatile u32 AHB1LPENR;
	volatile u32 AHB2LPENR;
	volatile u32 Reserved3[2];
	volatile u32 APB1LPENR;
	volatile u32 APB2LPENR;
	volatile u32 Reserved4[2];
	volatile u32 BDCR;
	volatile u32 CSR;
	volatile u32 Reserved5[2];
	volatile u32 SSCGR;
	volatile u32 PLLI2SCFGR;
	volatile u32 RESERVED6;
	volatile u32 DCKCFGR;
}Rcc_Registers_t;

/*******************************************************************************
 *                                Variables			                           *
 *******************************************************************************/
static Rcc_Registers_t* const RCC = (Rcc_Registers_t*)RCC_BASE_ADDR;


/*******************************************************************************
 *                             Functions Declerations                          *
 *******************************************************************************/

RCC_enuErrorState_t RCC_setClkON(u32 RCC_clk)
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

RCC_enuErrorState_t RCC_getClkStatus(u32 RCC_clk, u32* RCC_clkStatus)
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

RCC_enuErrorState_t RCC_setPLLClkSrc(u32 RCC_PLLClk)
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


RCC_enuErrorState_t RCC_cfgPLLClk(u8 PLL_M, u16 PLL_N, u8 PLL_Q, u8 PLL_P)
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
		u32 loc_temp_RCC_PLLCFGR = RCC->PLLCFGR;					/*Save the Value of the register in case of any interrupt during this function*/
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

RCC_enuErrorState_t RCC_setSystemClk(u32 RCC_Sysclk)
{
	RCC_enuErrorState_t RCC_ErrorState = RCC_enuOK;
	if ( (RCC_Sysclk != RCC_SYSCLK_HSE) && (RCC_Sysclk != RCC_SYSCLK_HSI) && (RCC_Sysclk != RCC_SYSCLK_PLL))
	{
		RCC_ErrorState = RCC_enuInvalidSysClk;
	}
	else
	{
		u32 loc_RCC_CFGR_temp = RCC -> CFGR;
		loc_RCC_CFGR_temp &= RCC_CFGR_SYSCLK_MASK;
		loc_RCC_CFGR_temp |= RCC_Sysclk;
		RCC -> CFGR = loc_RCC_CFGR_temp;
	}
	return RCC_ErrorState;
}


RCC_enuErrorState_t RCC_getSystemClk(u32* RCC_Sysclk)
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

RCC_enuErrorState_t RCC_Set_AHB1_Prescaler(u8 Copy_AHB1_Prescaler)
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


RCC_enuErrorState_t RCC_Ctrl_AHB1_Clk(u32 Copy_u32Periphral, RCC_enuPeriphralMode_t RCC_enuPeriphralMode)
{
	RCC_enuErrorState_t RCC_ErrorState = RCC_enuOK;
	if (RCC_enuPeriphralMode == RCC_enuPeriphralEnable)
	{
		RCC -> AHB1ENR |= Copy_u32Periphral;
	}
	else if (RCC_enuPeriphralMode == RCC_enuPeriphralDisable)
	{
		RCC -> AHB1ENR &= ~Copy_u32Periphral;
	}
	else
	{
		RCC_ErrorState = RCC_enuInvalidPeriphralMode;
	}
	return RCC_ErrorState;
}

RCC_enuErrorState_t RCC_Ctrl_AHB2_Clk(u32 Copy_u32Periphral, RCC_enuPeriphralMode_t RCC_enuPeriphralMode)
{
	RCC_enuErrorState_t RCC_ErrorState = RCC_enuOK;
	if (RCC_enuPeriphralMode == RCC_enuPeriphralEnable)
	{
		RCC -> AHB2ENR |= Copy_u32Periphral;
	}
	else if (RCC_enuPeriphralMode == RCC_enuPeriphralDisable)
	{
		RCC -> AHB2ENR &= ~Copy_u32Periphral;
	}
	else
	{
		RCC_ErrorState = RCC_enuInvalidPeriphralMode;
	}
	return RCC_ErrorState;
}

RCC_enuErrorState_t RCC_Set_APB1_Prescaler(u8 Copy_APB1_Prescaler)
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

RCC_enuErrorState_t RCC_Ctrl_APB1_Clk(u32 Copy_u32Periphral, RCC_enuPeriphralMode_t RCC_enuPeriphralMode)
{
	RCC_enuErrorState_t RCC_ErrorState = RCC_enuOK;
	if (RCC_enuPeriphralMode == RCC_enuPeriphralEnable)
	{
		RCC -> APB1ENR |= Copy_u32Periphral;
	}
	else if (RCC_enuPeriphralMode == RCC_enuPeriphralDisable)
	{
		RCC -> APB1ENR &= ~Copy_u32Periphral;
	}
	else
	{
		RCC_ErrorState = RCC_enuInvalidPeriphralMode;
	}
	return RCC_ErrorState;
}

RCC_enuErrorState_t RCC_Set_APB2_Prescaler(u8 Copy_APB2_Prescaler)
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

RCC_enuErrorState_t RCC_Ctrl_APB2_Clk(u32 Copy_u32Periphral, RCC_enuPeriphralMode_t RCC_enuPeriphralMode)
{
	RCC_enuErrorState_t RCC_ErrorState = RCC_enuOK;
	if (RCC_enuPeriphralMode == RCC_enuPeriphralEnable)
	{
		RCC -> APB2ENR |= Copy_u32Periphral;
	}
	else if (RCC_enuPeriphralMode == RCC_enuPeriphralDisable)
	{
		RCC -> APB2ENR &= ~Copy_u32Periphral;
	}
	else
	{
		RCC_ErrorState = RCC_enuInvalidPeriphralMode;
	}
	return RCC_ErrorState;
}

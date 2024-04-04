/******************************************************************************
 *
 * Module: RCC
 *
 * File Name: RCC.h
 *
 * Description: Header file for the RCC driver Configurations for STM32F401xC
 *
 * Author: Momen Elsayed Shaban
 *
 *******************************************************************************/

#ifndef RCC_H_
#define RCC_H_

#include "std_types.h"


/*******************************************************************************
 *                                Type Decelerations                           *
 *******************************************************************************/
/********************used in RCC_setClkON(u32 RCC_clk) API**********************/
#define RCC_HSI_ON						0x00000001
#define RCC_HSE_ON						0x00010000
#define RCC_HSE_BYP_ON					0x00050000
#define RCC_PLL_ON						0x01000000
#define RCC_PLLI2S_ON					0x04000000

/********used in RCC_getClkStatus(u32 RCC_clk, u32* RCC_clkStatus) API**********/
#define RCC_HSI_RDY						0x00000002
#define RCC_HSE_RDY						0x00020000
#define RCC_PLL_RDY                     0x02000000
#define RCC_PLLI2S_RDY                  0x08000000

/****************used in RCC_setPLLClkSrc(u32 RCC_PLLClk) API*******************/
#define RCC_PLL_CLK_HSI                 0x00000000
#define RCC_PLL_CLK_HSE                 0x00400000

/****************used in RCC_setSystemClk(u32 RCC_Sysclk) API*******************/
#define RCC_SYSCLK_HSI                  0x00000000
#define RCC_SYSCLK_HSE                  0x00000001
#define RCC_SYSCLK_PLL                  0x00000002

/************RCC AHB1 peripherals******************/
#define RCC_GPIOA_ENABLE_DISABLE		0x00000001
#define RCC_GPIOB_ENABLE_DISABLE		0x00000002
#define RCC_GPIOC_ENABLE_DISABLE		0x00000004
#define RCC_GPIOD_ENABLE_DISABLE		0x00000008
#define RCC_GPIOE_ENABLE_DISABLE		0x00000010
#define RCC_GPIOH_ENABLE_DISABLE		0x00000080
#define RCC_CRCRST_ENABLE_DISABLE		0x00001000
#define RCC_DMA1_ENABLE_DISABLE			0x00200000
#define RCC_DMA2_ENABLE_DISABLE			0x00400000

/************RCC AHB2 peripherals******************/
#define RCC_OTGFS_ENABLE_DISABLE		0x00000080

/************RCC APB1 peripherals******************/
#define RCC_TIM2_ENABLE_DISABLE			0x00000001
#define RCC_TIM3_ENABLE_DISABLE			0x00000002
#define RCC_TIM4_ENABLE_DISABLE			0x00000004
#define RCC_TIM5_ENABLE_DISABLE			0x00000008
#define RCC_WWDG_ENABLE_DISABLE			0x00000800
#define RCC_SPI2_ENABLE_DISABLE			0x00004000
#define RCC_SPI3_ENABLE_DISABLE			0x00008000
#define RCC_USART2_ENABLE_DISABLE		0x00020000
#define RCC_I2C1_ENABLE_DISABLE			0x00200000
#define RCC_I2C2_ENABLE_DISABLE			0x00400000
#define RCC_I2C3_ENABLE_DISABLE			0x00800000
#define RCC_PWR_ENABLE_DISABLE			0x10000000

/************RCC APB2 peripherals******************/
#define RCC_TIM1_ENABLE_DISABLE       	0x00000001
#define RCC_USART1_ENABLE_DISABLE     	0x00000010
#define RCC_USART6_ENABLE_DISABLE     	0x00000020
#define RCC_ADC1_ENABLE_DISABLE       	0x00000100
#define RCC_SDIO_ENABLE_DISABLE       	0x00000800
#define RCC_SPI1_ENABLE_DISABLE       	0x00001000
#define RCC_SPI4_ENABLE_DISABLE       	0x00002000
#define RCC_SYSCFG_ENABLE_DISABLE     	0x00004000
#define RCC_TIM9_ENABLE_DISABLE       	0x00010000
#define RCC_TIM10_ENABLE_DISABLE      	0x00020000
#define RCC_TIM11_ENABLE_DISABLE      	0x00040000

/************RCC AHB prescalers******************/
#define RCC_AHB_PRESCALER_NONE				0x0
#define RCC_AHB_PRESCALER_2					0x8
#define RCC_AHB_PRESCALER_4					0x9
#define RCC_AHB_PRESCALER_8					0xA
#define RCC_AHB_PRESCALER_16				0xB
#define RCC_AHB_PRESCALER_64				0xC
#define RCC_AHB_PRESCALER_128				0xD
#define RCC_AHB_PRESCALER_256				0xE
#define RCC_AHB_PRESCALER_512				0xF

/************RCC APB1 prescalers******************/
#define RCC_APB1_PRESCALER_NONE				0x0
#define RCC_APB1_PRESCALER_2				0x4
#define RCC_APB1_PRESCALER_4				0x5
#define RCC_APB1_PRESCALER_8				0x6
#define RCC_APB1_PRESCALER_16				0x7

/************RCC APB2 prescalers******************/
#define RCC_APB2_PRESCALER_NONE				0x0
#define RCC_APB2_PRESCALER_2				0x4
#define RCC_APB2_PRESCALER_4				0x5
#define RCC_APB2_PRESCALER_8				0x6
#define RCC_APB2_PRESCALER_16				0x7



typedef enum {
    RCC_enuPeriphralEnable,    /* Enable the specified peripheral(s) */
    RCC_enuPeriphralDisable    /* Disable the specified peripheral(s) */
} RCC_enuPeriphralMode_t;

typedef enum {
    RCC_enuOK,                      /* Operation completed successfully */
    RCC_enuInvalidClk,              /* Invalid clock option provided */
    RCC_enuInvalidClkSrcForPLL,     /* Invalid clock source provided for PLL configuration */
    RCC_enuPLLisONwhileCfg,         /* Attempted PLL configuration while PLL is already enabled */
    RCC_enuInvalidSysClk,           /* Invalid system clock option provided */
    RCC_enuInvalidPeriphralMode,    /* Invalid peripheral mode provided for control */
    RCC_enuInvalidPrescaler,         /* Invalid prescaler value provided */
    RCC_enuInvalid_M_Value,
    RCC_enuInvalid_N_Value,
    RCC_enuInvalid_P_Value,
    RCC_enuInvalid_Q_Value
} RCC_enuErrorState_t;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
/*****************************************************
 * Function: RCC_setClkON
 * Description: Enables the specified clock source in the RCC (Reset and Clock Control) module.
 *
 * Parameters:
 *   - RCC_clk: A 32-bit value representing the clock source to be enabled.
 *
 * Return:
 *   - RCC_enuErrorState_t: An error state indicating the success or failure of the clock enabling operation.
 *
 * Errors:
 *   - RCC_enuInvalidClk: Returned if an invalid clock source is provided.
 *   - RCC_enuOK: Returned if the clock enabling operation is successful.
 *
 * Clock Source Definitions:
 *   - RCC_HSI_ON: Enable the HSI (High-Speed Internal) oscillator.
 *   - RCC_HSE_ON: Enable the HSE (High-Speed External) oscillator.
 *   - RCC_HSE_BYP_ON: Enable the HSE (High-Speed External) oscillator with bypass mode.
 *   - RCC_PLL_ON: Enable the PLL (Phase-Locked Loop) oscillator.
 *   - RCC_PLLI2S_ON: Enable the PLLI2S (I2S-specific PLL) oscillator.
 *
 * Usage: RCC_enuErrorState_t error = RCC_setClkON(RCC_PLL_ON); 
 * Example: Enable PLL
 *
 * Notes:
 *   - You Need to Enable Any Clk before using it in Sysclk or PLL , ...etc.
 *****************************************************/
RCC_enuErrorState_t RCC_setClkON(u32 RCC_clk);

/*****************************************************
 * Function: RCC_getClkStatus
 * Description: Retrieves the status of the specified clock source in the RCC (Reset and Clock Control) module.
 *
 * Parameters:
 *   - RCC_clk: A 32-bit value representing the clock source status to be retrieved.
 *   - RCC_clkStatus: A pointer to a variable where the clock source status will be stored.
 *
 * Return:
 *   - RCC_enuErrorState_t: An error state indicating the success or failure of the clock status retrieval operation.
 *
 * Errors:
 *   - RCC_enuInvalidClk: Returned if an invalid clock source is provided.
 *   - RCC_enuOK: Returned if the clock status retrieval operation is successful.
 *
 * Clock Source Definitions:
 *   - RCC_HSE_RDY: HSE (High-Speed External) oscillator ready status.
 *   - RCC_HSI_RDY: HSI (High-Speed Internal) oscillator ready status.
 *   - RCC_PLL_RDY: PLL (Phase-Locked Loop) oscillator ready status.
 *   - RCC_PLLI2S_RDY: PLLI2S (I2S-specific PLL) oscillator ready status.
 *
 * Usage: 
 *   RCC_enuErrorState_t error;
 *   u32 clkStatus;
 *   error = RCC_getClkStatus(RCC_PLL_RDY, &clkStatus); 
 *   Example: Get PLL ready status
 *
 * Notes:
 *   - The clock source status is stored in the variable pointed to by RCC_clkStatus.
 *   - If the Value of RCC_clkStatus Not Equal Zeros Means that the Clock is Ready to use.
 *****************************************************/
RCC_enuErrorState_t RCC_getClkStatus(u32 RCC_clk, u32* RCC_clkStatus);

/*****************************************************
 * Function: RCC_setPLLClkSrc
 * Description: Configures the PLL (Phase-Locked Loop) clock source in the RCC (Reset and Clock Control) module.
 *
 * Parameters:
 *   - RCC_PLLClk: A 32-bit value representing the PLL clock source to be configured.
 *               - RCC_PLL_CLK_HSI: PLL clock source selection as HSI (High-Speed Internal) oscillator.
 *               - RCC_PLL_CLK_HSE: PLL clock source selection as HSE (High-Speed External) oscillator.
 *
 * Return:
 *   - RCC_enuErrorState_t: An error state indicating the success or failure of the PLL clock source configuration.
 *
 * Errors:
 *   - RCC_enuInvalidClkSrcForPLL: Returned if an invalid PLL clock source is provided.
 *   - RCC_enuOK: Returned if the PLL clock source configuration is successful.
 *
 * Usage: 
 *   RCC_enuErrorState_t error;
 *   error = RCC_setPLLClkSrc(RCC_PLL_CLK_HSE);
 *   Example: Set PLL clock source to HSE
 *
 * Notes:
 *   - You Must Enable the Desired Clock and Check if it is ready or not
 *     before using it by "RCC_setClkON and RCC_getClkStatus"
 *****************************************************/
RCC_enuErrorState_t RCC_setPLLClkSrc(u32 RCC_PLLClk);


/*****************************************************
 * Function: RCC_cfgPLLClk
 * Description: Configures the PLL (Phase-Locked Loop) parameters in the RCC (Reset and Clock Control) module.
 *
 * Parameters:
 *   - PLL_M: The PLL M parameter (integer value between 2 and 63, inclusive).
 *   - PLL_N: The PLL N parameter (integer value between 192 and 432, inclusive).
 *   - PLL_Q: The PLL Q parameter (integer value between 2 and 15, inclusive).
 *   - PLL_P: The PLL P parameter (either 2, 4, 6, or 8).
 *
 * Return:
 *   - RCC_enuErrorState_t: An error state indicating the success or failure of the PLL configuration.
 *
 * Errors:
 *   - RCC_enuInvalid_M_Value: Returned if the PLL M parameter is out of range.
 *   - RCC_enuInvalid_N_Value: Returned if the PLL N parameter is out of range.
 *   - RCC_enuInvalid_Q_Value: Returned if the PLL Q parameter is out of range.
 *   - RCC_enuInvalid_P_Value: Returned if the PLL P parameter is invalid.
 *   - RCC_enuOK: Returned if the PLL configuration is successful.
 *
 * Usage: 
 *   RCC_enuErrorState_t error;
 *   error = RCC_cfgPLLClk(8, 240, 4, 2); 
 *   Example: Configure PLL with parameters M=8, N=240, Q=4, P=2
 *
 * Notes:
 *   - The function checks for valid PLL parameter ranges before configuration.
 *   - PLL must be OFF before Configure it and you can do it by switch to another
 *     clock by using this API RCC_setSystemClk and you can check the current system
 *     clock by RCC_getSystemClk
 *****************************************************/
RCC_enuErrorState_t RCC_cfgPLLClk(u8 PLL_M, u16 PLL_N, u8 PLL_Q, u8 PLL_P);

/*****************************************************
 * Function: RCC_setSystemClk
 * Description: Configures the system clock source in the RCC (Reset and Clock Control) module.
 *
 * Parameters:
 *   - RCC_Sysclk: A 32-bit value representing the system clock source to be configured.
 *   - RCC_SYSCLK_HSE: System clock source selection as HSE (High-Speed External) oscillator.
 *   - RCC_SYSCLK_HSI: System clock source selection as HSI (High-Speed Internal) oscillator.
 *   - RCC_SYSCLK_PLL: System clock source selection as PLL (Phase-Locked Loop) oscillator.
 *
 * Return:
 *   - RCC_enuErrorState_t: An error state indicating the success or failure of the system clock configuration.
 *
 * Errors:
 *   - RCC_enuInvalidSysClk: Returned if an invalid system clock source is provided.
 *   - RCC_enuOK: Returned if the system clock configuration is successful.
 *
 * Usage: 
 *   RCC_enuErrorState_t error;
 *   error = RCC_setSystemClk(RCC_SYSCLK_PLL); 
 *   Example: Set system clock source to PLL
 *
 * Notes:
 *   - You Must Enable the Desired Clock and Check if it is ready or not
 *     before using it by "RCC_setClkON and RCC_getClkStatus"
 *****************************************************/
RCC_enuErrorState_t RCC_setSystemClk(u32 RCC_Sysclk);


/*****************************************************
 * Function: RCC_getSystemClk
 * Description: Retrieves the current system clock source from the RCC (Reset and Clock Control) module.
 *
 * Parameters:
 *   - RCC_Sysclk: A pointer to a 32-bit variable where the current system clock source will be stored.
 *
 * Return:
 *   - RCC_enuErrorState_t: An error state indicating the success or failure of retrieving the system clock source.
 *
 * Errors:
 *   - RCC_enuOK: Always returned to indicate success.
 *
 * System Clock Source Definitions:
 *   - RCC_SYSCLK_HSI: Value indicating that HSI (High-Speed Internal) oscillator is the system clock.
 *   - RCC_SYSCLK_HSE: Value indicating that HSE (High-Speed External) oscillator is the system clock.
 *   - RCC_SYSCLK_PLL: Value indicating that PLL (Phase-Locked Loop) oscillator is the system clock.
 *
 * Usage: 
 *   RCC_enuErrorState_t error;
 *   u32 currentSysclk;
 *   error = RCC_getSystemClk(&currentSysclk); 
 *   Example: Get the current system clock source
 *
 * Notes:
 *   - The function returns the current system clock source in the provided pointer variable.
 *****************************************************/
RCC_enuErrorState_t RCC_getSystemClk(u32* RCC_Sysclk);

/*****************************************************
 * Function: RCC_Set_AHB1_Prescaler
 * Description: Configures the AHB1 (Advanced High-Performance Bus 1) prescaler in the RCC (Reset and Clock Control) module.
 *
 * Parameters:
 *   - Copy_AHB1_Prescaler: The AHB1 prescaler value to be configured.
 *
 * Return:
 *   - RCC_enuErrorState_t: An error state indicating the success or failure of the AHB1 prescaler configuration.
 *
 * Errors:
 *   - RCC_enuInvalidPrescaler: Returned if an invalid AHB1 prescaler value is provided.
 *   - RCC_enuOK: Returned if the AHB1 prescaler configuration is successful.
 *
 * AHB1 Prescaler Values:
 *   - RCC_AHB_PRESCALER_NONE: No prescaling (0x0).
 *   - RCC_AHB_PRESCALER_2: Division by 2 (0x8).
 *   - RCC_AHB_PRESCALER_4: Division by 4 (0x9).
 *   - RCC_AHB_PRESCALER_8: Division by 8 (0xA).
 *   - RCC_AHB_PRESCALER_16: Division by 16 (0xB).
 *   - RCC_AHB_PRESCALER_64: Division by 64 (0xC).
 *   - RCC_AHB_PRESCALER_128: Division by 128 (0xD).
 *   - RCC_AHB_PRESCALER_256: Division by 256 (0xE).
 *   - RCC_AHB_PRESCALER_512: Division by 512 (0xF).
 *
 * Usage: 
 *   RCC_enuErrorState_t error;
 *   error = RCC_Set_AHB1_Prescaler(RCC_AHB_PRESCALER_8); 
 *   Example: Set AHB1 prescaler to divide by 8
 *
 * Notes:
 *   - The function checks for a valid AHB1 prescaler value before configuration.
 *****************************************************/
RCC_enuErrorState_t RCC_Set_AHB1_Prescaler(u8 Copy_AHB1_Prescaler);

/*****************************************************
 * Function: RCC_Ctrl_AHB1_Clk
 * Description: Controls the clock enable/disable for peripherals in the AHB1 (Advanced High-Performance Bus 1)
 *  in the RCC (Reset and Clock Control) module.
 *
 * Parameters:
 *   - Copy_u32Periphral: The bitmask representing the peripherals to be enabled or disabled.
 *   - RCC_enuPeriphralMode: The mode indicating whether to enable or disable the specified peripherals.
 *
 * Return:
 *   - RCC_enuErrorState_t: An error state indicating the success or failure of controlling the AHB1 peripherals' clock.
 *
 * Errors:
 *   - RCC_enuInvalidPeriphralMode: Returned if an invalid peripheral mode is provided.
 *   - RCC_enuOK: Returned if the AHB1 peripherals' clock control is successful.
 *
 * AHB1 Peripheral Control Definitions:
 *   - RCC_GPIOA_ENABLE_DISABLE: Enable/Disable GPIOA clock.
 *   - RCC_GPIOB_ENABLE_DISABLE: Enable/Disable GPIOB clock.
 *   - RCC_GPIOC_ENABLE_DISABLE: Enable/Disable GPIOC clock.
 *   - RCC_GPIOD_ENABLE_DISABLE: Enable/Disable GPIOD clock.
 *   - RCC_GPIOE_ENABLE_DISABLE: Enable/Disable GPIOE clock.
 *   - RCC_GPIOH_ENABLE_DISABLE: Enable/Disable GPIOH clock.
 *   - RCC_CRCRST_ENABLE_DISABLE: Enable/Disable CRC clock.
 *   - RCC_DMA1_ENABLE_DISABLE: Enable/Disable DMA1 clock.
 *   - RCC_DMA2_ENABLE_DISABLE: Enable/Disable DMA2 clock.
 *
 * Usage: 
 *   RCC_enuErrorState_t error;
 *   error = RCC_Ctrl_AHB1_Clk(RCC_GPIOA_ENABLE_DISABLE | RCC_GPIOB_ENABLE_DISABLE, RCC_enuPeriphralEnable); 
 *   Example: Enable GPIOA and GPIOB clocks
 *
 * Notes:
 *   - The function checks for a valid peripheral mode before controlling the AHB1 peripherals' clock.
 *****************************************************/
RCC_enuErrorState_t RCC_Ctrl_AHB1_Clk(u32 Copy_u32Periphral, RCC_enuPeriphralMode_t RCC_enuPeriphralMode);


/*****************************************************
 * Function: RCC_Ctrl_AHB2_Clk
 * Description: Controls the clock enable/disable for peripherals in the AHB2 (Advanced High-Performance Bus 2) in the RCC (Reset and Clock Control) module.
 *
 * Parameters:
 *   - Copy_u32Periphral: The bitmask representing the peripherals to be enabled or disabled.
 *   - RCC_enuPeriphralMode: The mode indicating whether to enable or disable the specified peripherals.
 *
 * Return:
 *   - RCC_enuErrorState_t: An error state indicating the success or failure of controlling the AHB2 peripherals' clock.
 *
 * Errors:
 *   - RCC_enuInvalidPeriphralMode: Returned if an invalid peripheral mode is provided.
 *   - RCC_enuOK: Returned if the AHB2 peripherals' clock control is successful.
 *
 * AHB2 Peripheral Control Definitions:
 *   - RCC_OTGFS_ENABLE_DISABLE: Enable/Disable OTG FS clock.
 *
 * Usage: 
 *   RCC_enuErrorState_t error;
 *   error = RCC_Ctrl_AHB2_Clk(RCC_OTGFS_ENABLE_DISABLE, RCC_enuPeriphralEnable); 
 *   Example: Enable OTG FS clock
 *
 * Notes:
 *   - The function checks for a valid peripheral mode before controlling the AHB2 peripherals' clock.
 *****************************************************/
RCC_enuErrorState_t RCC_Ctrl_AHB2_Clk(u32 Copy_u32Periphral, RCC_enuPeriphralMode_t RCC_enuPeriphralMode);

/*****************************************************
 * Function: RCC_Set_APB1_Prescaler
 * Description: Sets the prescaler for peripherals in the APB1 (Advanced Peripheral Bus 1) 
 * in the RCC (Reset and Clock Control) module.
 *
 * Parameters:
 *   - Copy_APB1_Prescaler: The prescaler value to be set for APB1 peripherals.
 *
 * Return:
 *   - RCC_enuErrorState_t: An error state indicating the success or failure of setting the APB1 prescaler.
 *
 * Errors:
 *   - RCC_enuInvalidPrescaler: Returned if an invalid prescaler value is provided.
 *   - RCC_enuOK: Returned if setting the APB1 prescaler is successful.
 *
 * APB1 Prescaler Definitions:
 *   - RCC_APB1_PRESCALER_NONE: No division (prescaler = 1).
 *   - RCC_APB1_PRESCALER_2: Division by 2.
 *   - RCC_APB1_PRESCALER_4: Division by 4.
 *   - RCC_APB1_PRESCALER_8: Division by 8.
 *   - RCC_APB1_PRESCALER_16: Division by 16.
 *
 * Usage:
 *   RCC_enuErrorState_t error;
 *   error = RCC_Set_APB1_Prescaler(RCC_APB1_PRESCALER_4); 
 *   Example: Set APB1 prescaler to 4
 *
 * Notes:
 *   - The function checks for a valid prescaler value before setting the APB1 prescaler.
 *   - This function not fully validate the input.
 *****************************************************/
RCC_enuErrorState_t RCC_Set_APB1_Prescaler(u8 Copy_APB1_Prescaler);

/*****************************************************
 * Function: RCC_Ctrl_APB1_Clk
 * Description: Controls the clock enable/disable for peripherals in the APB1 (Advanced Peripheral Bus 1) in the RCC (Reset and Clock Control) module.
 *
 * Parameters:
 *   - Copy_u32Periphral: The bitmask representing the peripherals to be enabled or disabled.
 *   - RCC_enuPeriphralMode: The mode indicating whether to enable or disable the specified peripherals.
 *
 * Return:
 *   - RCC_enuErrorState_t: An error state indicating the success or failure of controlling the APB1 peripherals' clock.
 *
 * Errors:
 *   - RCC_enuInvalidPeriphralMode: Returned if an invalid peripheral mode is provided.
 *   - RCC_enuOK: Returned if the APB1 peripherals' clock control is successful.
 *
 * APB1 Peripheral Control Definitions:
 *   - RCC_TIM2_ENABLE_DISABLE: Enable/Disable TIM2 clock.
 *   - RCC_TIM3_ENABLE_DISABLE: Enable/Disable TIM3 clock.
 *   - RCC_TIM4_ENABLE_DISABLE: Enable/Disable TIM4 clock.
 *   - RCC_TIM5_ENABLE_DISABLE: Enable/Disable TIM5 clock.
 *   - RCC_WWDG_ENABLE_DISABLE: Enable/Disable WWDG clock.
 *   - RCC_SPI2_ENABLE_DISABLE: Enable/Disable SPI2 clock.
 *   - RCC_SPI3_ENABLE_DISABLE: Enable/Disable SPI3 clock.
 *   - RCC_USART2_ENABLE_DISABLE: Enable/Disable USART2 clock.
 *   - RCC_I2C1_ENABLE_DISABLE: Enable/Disable I2C1 clock.
 *   - RCC_I2C2_ENABLE_DISABLE: Enable/Disable I2C2 clock.
 *   - RCC_I2C3_ENABLE_DISABLE: Enable/Disable I2C3 clock.
 *   - RCC_PWR_ENABLE_DISABLE: Enable/Disable Power interface clock.
 *
 * Usage: 
 *   RCC_enuErrorState_t error;
 *   error = RCC_Ctrl_APB1_Clk(RCC_TIM2_ENABLE_DISABLE, RCC_enuPeriphralEnable); 
 *   Example: Enable TIM2 clock
 *
 * Notes:
 *   - The function checks for a valid peripheral mode before controlling the APB1 peripherals' clock.
 *****************************************************/
RCC_enuErrorState_t RCC_Ctrl_APB1_Clk(u32 Copy_u32Periphral, RCC_enuPeriphralMode_t RCC_enuPeriphralMode);


/*****************************************************
 * Function: RCC_Set_APB2_Prescaler
 * Description: Sets the prescaler for the APB2 (Advanced Peripheral Bus 2) in the RCC (Reset and Clock Control) module.
 *
 * Parameters:
 *   - Copy_APB2_Prescaler: The prescaler value to be set for APB2.
 *
 * Return:
 *   - RCC_enuErrorState_t: An error state indicating the success or failure of setting the APB2 prescaler.
 *
 * Errors:
 *   - RCC_enuInvalidPrescaler: Returned if an invalid prescaler value is provided.
 *   - RCC_enuOK: Returned if the APB2 prescaler is set successfully.
 *
 * APB2 Prescaler Definitions:
 *   - RCC_APB2_PRESCALER_NONE: No prescaler (divided by 1).
 *   - RCC_APB2_PRESCALER_2: Divided by 2.
 *   - RCC_APB2_PRESCALER_4: Divided by 4.
 *   - RCC_APB2_PRESCALER_8: Divided by 8.
 *   - RCC_APB2_PRESCALER_16: Divided by 16.
 *
 * Usage:
 *   RCC_enuErrorState_t error;
 *   error = RCC_Set_APB2_Prescaler(RCC_APB2_PRESCALER_2); 
 *   Example: Set APB2 prescaler to divide by 2
 *
 * Notes:
 *   - The function checks for a valid prescaler value before setting the APB2 prescaler.
 *   - This function not fully validate the input.
 *****************************************************/
RCC_enuErrorState_t RCC_Set_APB2_Prescaler(u8 Copy_APB2_Prescaler);

/*****************************************************
 * Function: RCC_Ctrl_APB2_Clk
 * Description: Controls the clock for peripherals in the APB2 (Advanced Peripheral Bus 2) in the RCC (Reset and Clock Control) module.
 *
 * Parameters:
 *   - Copy_u32Periphral: Bitmask representing the peripherals to be controlled.
 *   - RCC_enuPeriphralMode: The mode to set for the peripherals (Enable or Disable).
 *
 * Return:
 *   - RCC_enuErrorState_t: An error state indicating the success or failure of controlling the APB2 peripherals.
 *
 * Errors:
 *   - RCC_enuInvalidPeriphralMode: Returned if an invalid peripheral mode is provided.
 *   - RCC_enuOK: Returned if the APB2 peripherals are controlled successfully.
 *
 * APB2 Peripheral Control Definitions:
 *   - RCC_TIM1_ENABLE_DISABLE: TIM1 peripheral enable/disable.
 *   - RCC_USART1_ENABLE_DISABLE: USART1 peripheral enable/disable.
 *   - RCC_USART6_ENABLE_DISABLE: USART6 peripheral enable/disable.
 *   - RCC_ADC1_ENABLE_DISABLE: ADC1 peripheral enable/disable.
 *   - RCC_SDIO_ENABLE_DISABLE: SDIO peripheral enable/disable.
 *   - RCC_SPI1_ENABLE_DISABLE: SPI1 peripheral enable/disable.
 *   - RCC_SPI4_ENABLE_DISABLE: SPI4 peripheral enable/disable.
 *   - RCC_SYSCFG_ENABLE_DISABLE: SYSCFG peripheral enable/disable.
 *   - RCC_TIM9_ENABLE_DISABLE: TIM9 peripheral enable/disable.
 *   - RCC_TIM10_ENABLE_DISABLE: TIM10 peripheral enable/disable.
 *   - RCC_TIM11_ENABLE_DISABLE: TIM11 peripheral enable/disable.
 *
 * Usage:
 *   RCC_enuErrorState_t error;
 *   error = RCC_Ctrl_APB2_Clk(RCC_TIM1_ENABLE_DISABLE, RCC_enuPeriphralEnable); 
 *   Example: Enable TIM1 peripheral in APB2
 *
 * Notes:
 *   - The function checks for a valid peripheral mode before controlling the APB2 peripherals.
 *****************************************************/
RCC_enuErrorState_t RCC_Ctrl_APB2_Clk(u32 Copy_u32Periphral, RCC_enuPeriphralMode_t RCC_enuPeriphralMode);


#endif /* RCC_H_ */

#ifndef RCC_H__
#define RCC_H__

#include <stdint.h>
/*********************************************defines****************************************************************/

/**
 * This is the choices for main Clocks on MC which could be enabled (hint PLL should be configured before it could be enabled)
*/
#define RCC_CLKENABLEPLL 0x1000000
#define RCC_CLKENABLEHSE 0x10000
#define RCC_CLKENABLEHSI 0x1

/**
 * @brief This the options for system clock sources
 * 
 */
#define RCC_SYSCLK_HSI 0x0
#define RCC_SYSCLK_HSE 0x1
#define RCC_SYSCLK_PLL 0x2

/**
 * @brief These are the return options for the current system clock
 * 
 */
#define RCC_SYSCLKRETURN_HSI 0x0
#define RCC_SYSCLKRETURN_HSE 0x4
#define RCC_SYSCLKRETURN_PLL 0x8

/**
 * @brief These are the options for the PLL clock sources
 * 
 */
#define RCC_PLL_CLKSOURCEHSI 0x0
#define RCC_PLL_CLKSOURCEHSE 0x400000

/**
 * @brief These are AHB peripherals to be enabled
 * 
 */
#define RCC_AHB1_DMA2  0x400000
#define RCC_AHB1_DMA1  0x200000
#define RCC_AHB1_CRCEN 0x1000
#define RCC_AHB1_GPIOH 0x80
#define RCC_AHB1_GPIOE 0x10
#define RCC_AHB1_GPIOD 0x08
#define RCC_AHB1_GPIOC 0x04
#define RCC_AHB1_GPIOB 0x02
#define RCC_AHB1_GPIOA 0x01

#define RCC_AHB2_OTGFS 0x80

/**
 * @brief These are APB peripherals to be enabled
 * 
 */
#define RCC_APB1_PWR    0x10000000
#define RCC_APB1_I2C3   0x800000
#define RCC_APB1_I2C2   0x400000
#define RCC_APB1_I2C1   0x200000
#define RCC_APB1_USART2 0x20000
#define RCC_APB1_SPI3   0x8000
#define RCC_APB1_SPI2   0x4000
#define RCC_APB1_WWDG   0x800
#define RCC_APB1_TIM5   0x8
#define RCC_APB1_TIM4   0x4
#define RCC_APB1_TIM3   0x2
#define RCC_APB1_TIM2   0x1

#define RCC_APB2_TIM11  0x40000
#define RCC_APB2_TIM10   0x20000
#define RCC_APB2_TIM9   0x10000
#define RCC_APB2_SYSCFG 0x4000
#define RCC_APB2_SPI4   0x2000
#define RCC_APB2_SPI1   0x1000
#define RCC_APB2_SDIO   0x800
#define RCC_APB2_ADC1   0x100
#define RCC_APB2_USART6 0x20
#define RCC_APB2_USART1 0x10
#define RCC_APB2_TIM1   0x01

#define MCO2_SOURCE_SYSCLK 0x00
#define MCO2_SOURCE_PLLI2S 0x40000000
#define MCO2_SOURCE_HSE    0x80000000
#define MCO2_SOURCE_PLL    0xC0000000

#define MCO2_PRE_1  0x0
#define MCO2_PRE_2  0x20000000
#define MCO2_PRE_3  0x28000000
#define MCO2_PRE_4  0x30000000
#define MCO2_PRE_5  0x38000000

#define MCO1_SOURCE_HSI 0x0
#define MCO1_SOURCE_LSE 0x200000
#define MCO1_SOURCE_HSE 0x400000
#define MCO1_SOURCE_PLL 0x600000

#define MCO1_PRE_1  0x0
#define MCO1_PRE_2  0x4000000
#define MCO1_PRE_3  0x5000000
#define MCO1_PRE_4  0x6000000
#define MCO1_PRE_5  0x7000000

#define APB2_PRE_1  0x00
#define APB2_PRE_2  0x8000
#define APB2_PRE_4  0xA000
#define APB2_PRE_8  0xC000
#define APB2_PRE_16 0xE000

#define APB1_PRE_1  0x00
#define APB1_PRE_2  0x1000
#define APB1_PRE_4  0x1400
#define APB1_PRE_8  0x1800
#define APB1_PRE_16 0x1C00

#define AHB_PRE_1   0x0
#define AHB_PRE_2   0x80
#define AHB_PRE_4   0x90
#define AHB_PRE_8   0xA0
#define AHB_PRE_16  0xB0
#define AHB_PRE_64  0xC0
#define AHB_PRE_128 0xD0
#define AHB_PRE_256 0xE0
#define AHB_PRE_512 0xF0

/********************************************************************************************************************/

/**********************************************types*****************************************************************/

typedef enum{
    RCC_isOk,
    RCC_isNotOk,
    RCC_Wrong_CLK_Option,
    RCC_SYSCLK_Is_Already_Selected,
    RCC_WrongAHBPeri,
    RCC_WrongAPBPeri

}RCC_WrongConfig_t;

typedef enum{
    RCC_WrongPLL_M_value,
    RCC_WrongPLL_N_value,
    RCC_WrongPLL_P_value,
    RCC_WrongPLL_Q_value,
    RCC_PLLOk,
    RCC_PLLNotOk,
    RCC_WrongPLLCLKSource,
    RCC_WrongMCO2Source,
    RCC_WrongMCO2PRE,
    RCC_WrongMCO1Source,
    RCC_WrongMCO1PRE,
    RCC_WrongAPB1_Prescaler,
    RCC_WrongAPB2_Prescaler,
    RCC_WrongAHB_Prescaler,
}RCC_WrongPLLConfig_t;

/********************************************************************************************************************/

/*************************************************APIs***************************************************************/

/**
 * @brief :This function enable the selected clock
 * 
 * @param Copy_ClockOption : please choose between HSI, HSE and PLL
 * @return RCC_WrongConfig_t : return the error status of this operation
 */
RCC_WrongConfig_t RCC_enableClock(uint32_t CLKEnable);
/**
 * @brief This function to disable a clock (note you should not disable the current system clock)
 * 
 * @param CLKEnable Choose the clock (HSI or HSE or PLL)
 * @return RCC_WrongConfig_t 
 */
RCC_WrongConfig_t RCC_disableClock(uint32_t CLKEnable);

/**
 * @brief :This function select a clock to be the system clock of the system
 * 
 * @param Copy_ClockOption : please choose between HSI, HSE and PLL
 * @return RCC_WrongConfig_t : return the error status of this operation
 */
RCC_WrongConfig_t RCC_selectSYSCLK(uint32_t SYSCLKOption);

/**
 * @brief : this function return the system clock which is configured(HSI, HSE or PLL)
 * 
 * 
 */
uint32_t RCC_getSYSCLK(void);

/**
 * @brief :this function enable the clock of a selected AHB peripherial 
 * 
 * @param Copy_AHPPeriOption : choose an option for AHB peripherial 
 * @return RCC_WrongConfig_t : return the error status of this operation
 */
RCC_WrongConfig_t RCC_enableAHBPeriCLK(uint32_t AHBPeriOption);

/**
 * @brief : This function enable the clock of a selected APB peripheral
 * 
 * @param Copy_APBPeriOption : choose an option for APB peripherial 
 * @return RCC_WrongConfig_t : return the error status of this operation
 */
RCC_WrongConfig_t RCC_enableAPBPeriCLK(uint32_t APBPeriOption);

/**
 * @brief This function configure the prescalers of the PLL 
 * @param RCC_PLLCLKSource: specifies the clk source for PLL (HSI or HSE)
 * @param M : this value from 2 to 63
 * @param N : this value from 192 to 432
 * @param P : this value 2,4,5,8
 * @param Q : this value from 2 to 15
 * @return RCC_WrongPLLConfig_t 
 */
RCC_WrongPLLConfig_t RCC_configurePLLCLK( uint32_t M, uint32_t N, uint32_t P, uint32_t Q);
/**
 * @brief This function select the clock source for PLL (HSE or HSI)
 * 
 * @param RCC_PLLCLKSource : choose between HSE and HSI
 * @return RCC_WrongPLLConfig_t : return error status for the function 
 */
RCC_WrongPLLConfig_t RCC_selectPLLCLK( uint32_t RCC_PLLCLKSource);

/**
 * @brief This function to configure the MCO2 pin
 * 
 * @param MCO2_SOURCE : MCO2_CLK_Source
 * @param MCO2_PRE : MCO2_Prescaler
 * @return RCC_WrongConfig_t 
 */
RCC_WrongConfig_t RCC_configMCO2(uint32_t MCO2_SOURCE, uint32_t MCO2_PRE);
/**
 * @brief This function to configure the MCO1 pin
 * 
 * @param MCO1_SOURCE MCO1_CLK_Source
 * @param MCO1_PRE MCO1_Prescalar
 * @return RCC_WrongConfig_t 
 */
RCC_WrongConfig_t RCC_configMCO1(uint32_t MCO1_SOURCE, uint32_t MCO1_PRE);

/**
 * @brief this function to configure the APB1 Prescaler
 * 
 * @param APB1_PRE : choose prescaler between the predefined options
 * @return RCC_WrongConfig_t 
 */
RCC_WrongConfig_t RCC_ConfigAPB1PRE(uint32_t APB1_PRE);

/**
 * @brief this function to configure the APB2 Prescaler
 * 
 * @param APB1_PRE : choose prescaler between the predefined options
 * @return RCC_WrongConfig_t 
 */
RCC_WrongConfig_t RCC_ConfigAPB2PRE(uint32_t APB2_PRE);

/**
 * @brief this function to configure the AHB Prescaler
 * 
 * @param APB1_PRE : choose prescaler between the predefined options
 * @return RCC_WrongConfig_t 
 */
RCC_WrongConfig_t RCC_ConfigAHBPRE(uint32_t AHB_PRE);
/*********************************************************************************************************************/
#endif
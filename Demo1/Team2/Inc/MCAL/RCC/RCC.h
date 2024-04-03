/*
 * RCC.h
 *
 *  Created on: Feb 10, 2024
 *      Author: Osman
 */

#ifndef RCC_RCC_H_
#define RCC_RCC_H_


#define RCC_SYSCLK_HSI      0x00000000
#define RCC_SYSCLK_HSE      0x00000001
#define RCC_SYSCLK_PLL      0x00000002


#define RCC_CLK_HSI     0
#define RCC_CLK_HSE     1
#define RCC_CLK_PLL     2
#define RCC_CLK_PLL2    3
#define RCC_CLK_LSE     4
#define RCC_CLK_LSI     5

#define RCC_PLL_P_2     0x0
#define RCC_PLL_P_4     0x1
#define RCC_PLL_P_6     0x2
#define RCC_PLL_P_8     0x3

#define RCC_AHB_PRESCALE_1          0b0111
#define RCC_AHB_PRESCALE_2          0b1000
#define RCC_AHB_PRESCALE_4          0b1001
#define RCC_AHB_PRESCALE_8          0b1010
#define RCC_AHB_PRESCALE_16         0b1011
#define RCC_AHB_PRESCALE_64         0b1100
#define RCC_AHB_PRESCALE_128        0b1101
#define RCC_AHB_PRESCALE_256        0b1110
#define RCC_AHB_PRESCALE_512        0b1111


#define RCC_APB_PRESCALE_1          0b011
#define RCC_APB_PRESCALE_2          0b100
#define RCC_APB_PRESCALE_4          0b101
#define RCC_APB_PRESCALE_8          0b110
#define RCC_APB_PRESCALE_16         0b111


typedef enum
{
    RCC_OK,
    RCC_NOK,
}RCC_ErrorStatus_t;


/* AHB1 Peripherals */
typedef enum {
    RCC_GPIOA  = (u64)0 + 0x300000000ULL,
    RCC_GPIOB  = (u64)1 + 0x300000000ULL,
    RCC_GPIOC  = (u64)2 + 0x300000000ULL,
    RCC_CRC    = (u64)12 + 0x300000000ULL,
    RCC_DMA1   = (u64)21 + 0x300000000ULL,
    RCC_DMA2   = (u64)22 + 0x300000000ULL,


/* AHB2 Peripherals*/

    RCC_OTGFS   = (u64)39 + 0x300000000ULL,


/* APB1 Peripherals */

    RCC_TIM2   = (u64)64 + 0x300000000ULL,
    RCC_TIM3   = (u64)65 + 0x300000000ULL,
    RCC_TIM4   = (u64)66 + 0x300000000ULL,
    RCC_TIM5   = (u64)67 + 0x300000000ULL,
    RCC_WWDG   = (u64)75 + 0x300000000ULL,
    RCC_SPI2   = (u64)78 + 0x300000000ULL,
    RCC_SPI3   = (u64)79 + 0x300000000ULL,
    RCC_USART2 = (u64)81 + 0x300000000ULL,
    RCC_I2C1   = (u64)85 + 0x300000000ULL,
    RCC_I2C2   = (u64)85 + 0x300000000ULL,
    RCC_I2C3   = (u64)87 + 0x300000000ULL,
    RCC_PWR    = (u64)92 + 0x300000000ULL,


/* APB2 Peripherals */

    RCC_TIM1   = (u64)96 + 0x300000000ULL,
    RCC_USART1 = (u64)100 + 0x300000000ULL,
    RCC_USART6 = (u64)101 + 0x300000000ULL,
    RCC_SDIO   = (u64)107 + 0x300000000ULL,
    RCC_SPI1   = (u64)108 + 0x300000000ULL,
    RCC_SPI4   = (u64)109 + 0x300000000ULL,
    RCC_SYSCFG = (u64)110 + 0x300000000ULL,
    RCC_TIM9   = (u64)112 + 0x300000000ULL,
    RCC_TIM10  = (u64)113 + 0x300000000ULL,
    RCC_TIM11  = (u64)114 + 0x300000000ULL,
}RCC_Peripheral_t;


/**
 * @brief Sets the Desired Clock ON
 *
 *
 * @param Copy_Clk The Clock to set ON
 * @return RCC_ErrorStatus_t RCC_OK if all good, RCC_NOK if argument is not in range
 *
 * Notes:
 *      - Once PLL or PLL2 are enabled, they can't be configured
 */
RCC_ErrorStatus_t RCC_EnableClk(u8 Copy_Clk);


/**
 * @brief Sets the Desired Clock ON
 *
 *
 * @param Copy_Clk The Clock to set ON
 * @return RCC_ErrorStatus_t RCC_OK if all good, RCC_NOK if argument is not in range
 *
 * Notes:
 *      - Can't set the Clock OFF it is the System clock
 */
RCC_ErrorStatus_t RCC_DisableClk(u8 Copy_Clk);


/**
 * @brief Sets the selected System Clock as the current System clock
 *
 * @param Copy_SystemClk System Clock
 * @return RCC_ErrorStatus_t RCC_OK if all good, RCC_NOK if argument is not in range
 *
 * Notes:
 *      - The selected Clock has to be enabled first
 */
RCC_ErrorStatus_t RCC_SetSystemClk(u8 Copy_SystemClk);


/**
 * @brief Sets the Source of PLL as either HSI or HSE
 *
 * @param Copy_SystemClk RCC_SystemClk_HSI or RCC_SystemClk_HSE
 * @return RCC_ErrorStatus_t RCC_OK if all good, RCC_NOK if argument is not in range
 */
RCC_ErrorStatus_t RCC_SetPllSource(u8 Copy_SystemClk);


/**
 * @brief Sets the output of the PLL Clock according to the given Prescaler values
 *
 * @param Copy_M Value to divide PLL source (HSI or HSE) by.
 * @param Copy_N Value to Multiply the output of VCO (output  of M prescaler) by.
 * @param Copy_P Prescaler Value of PLL.
 * @param Copy_Q Prescaler value for USB OTG FS.
 * @return RCC_ErrorStatus_t RCC_OK if all good, RCC_NOK if argument is not in range
 *
 * Notes:
 *      - Range of M is from 2 to 63 and its output must be in the range from 1 to 2 MHz
 *      - Range of N is 2 to 510 (433 is not valid) and its output must be in range from 192 to 432 MHz
 *      - P values are either 2, 4, 6, 8. Output must not exceed 84MHz
 *      - Range of Q is 2 to 15 and its output must be 48Mhz
 */
RCC_ErrorStatus_t RCC_SetPllClk(u8 Copy_M, u16 Copy_N, u8 Copy_P, u8 Copy_Q);

/**
 * @brief Enables the clock of the selected peripheral
 *
 * @param Copy_Peripheral Peripheral to turn its clock ON
 * @return RCC_ErrorStatus_t RCC_OK if all good, RCC_NOK if argument is not in range
 */
RCC_ErrorStatus_t RCC_EnablePeripheral(RCC_Peripheral_t Copy_Peripheral);

/**
 * @brief Disables the clock of the selected peripheral
 *
 * @param Copy_Peripheral Peripheral to turn its clock OFF
 * @return RCC_ErrorStatus_t RCC_OK if all good, RCC_NOK if argument is not in range
 */
RCC_ErrorStatus_t RCC_DisablePeripheral(RCC_Peripheral_t Copy_Peripheral);



/**
 * @brief Sets the Prescale for AHB Bus
 *
 * @param Copy_AHB_Prescale Prescale value
 * @return RCC_ErrorStatus_t RCC_OK if all good, RCC_NOK if argument is not in range
 */
RCC_ErrorStatus_t RCC_SetAHBPrescale(u8 Copy_AHB_Prescale);

/**
 * @brief Sets the Prescale for APB1 Bus
 *
 * @param Copy_AHB_Prescale Prescale value
 * @return RCC_ErrorStatus_t RCC_OK if all good, RCC_NOK if argument is not in range
 */
RCC_ErrorStatus_t RCC_SetAPB1Prescale(u8 Copy_APB_Prescale);

/**
 * @brief Sets the Prescale for APB2 Bus
 *
 * @param Copy_AHB_Prescale Prescale value
 * @return RCC_ErrorStatus_t RCC_OK if all good, RCC_NOK if argument is not in range
 */
RCC_ErrorStatus_t RCC_SetAPB2Prescale(u8 Copy_APB_Prescale);

#endif /* RCC_RCC_H_ */

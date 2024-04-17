/*
 * RCC.c
 *
 *  Created on: Feb 10, 2024
 *      Author: Osman
 */

#include "LIB/STD_TYPES.h"
#include "MCAL/RCC/RCC.h"
#include <stdint.h>

#define RCC_SYSCLK_MASK          0x00000003
#define RCC_SYSCLK_STATE_MASK    0x0000000C
#define RCC_PLL_M_PRESCALE_MASK  0x0000003F
#define RCC_PLL_N_PRESCALE_MASK	 0x00007FC0
#define RCC_PLL_P_PRESCALE_MASK  0x00030000
#define RCC_PLL_Q_PRESCALE_MASK  0x0F000000
#define RCC_AHB_PRESCALE_MASK    0x000000F0
#define RCC_APB1_PRESCALE_MASK   0x00001C00
#define RCC_APB2_PRESCALE_MASK   0x0000E000

#define RCC_TIMEOUT_PERIOD  (600UL)

#define RCC_BASE_ADDRESS        0x40023800
#define RCC     ((volatile RCC_Registers_t * const)RCC_BASE_ADDRESS)


typedef struct
{
    uint32_t CR;
    uint32_t PLLCFGR;
    uint32_t CFGR;
    uint32_t CIR;
    uint32_t AHB1RSTR;
    uint32_t AHB2RSTR;
    uint32_t reserved1[2];
    uint32_t APB1RSTR;
    uint32_t APB2RSTR;
    uint32_t reserved2[2];
    uint32_t AHB1ENR;
    uint32_t AHB2ENR;
    uint32_t reserved3[2];
    uint32_t APB1ENR;
    uint32_t APB2ENR;
    uint32_t reserved4[2];
    uint32_t AHB1LPENR;
    uint32_t AHB2LPENR;
    uint32_t reserved5[2];
    uint32_t APB1LPENR;
    uint32_t APB2LPENR;
    uint32_t reserved6[2];
    uint32_t BDCR;
    uint32_t CSR;
    uint32_t reserved7[2];
    uint32_t SSCGR;
    uint32_t RCC_PLLI2SCFGR;
    uint32_t reserved8;
    uint32_t DCKCFGR;

}RCC_Registers_t;

typedef enum
{
    RCC_HSION = 0,
    RCC_HSIRDY,
    RCC_HSITRIM = 3,
    RCC_HSICAL = 8,
    RCC_HSEON = 16,
    RCC_HSERDY = 17,
    RCC_HSEBYP = 18,
    RCC_CSSON = 19,
    RCC_PLLON = 24,
    RCC_PLLRDY = 25,
    RCC_PLLI2SON = 26,
    RCC_PLLI2SRDY = 27,

}RCC_CR_Bits_t;

typedef enum
{
    RCC_PLLM = 0,
    RCC_PLLN = 6,
    RCC_PLLP = 16,
    RCC_PLLSRC = 22,
    RCC_PLLQ = 24,

}RCC_PLLCFGR_Bits_t;


typedef enum
{
    RCC_SW = 0,
    RCC_SWS = 2,
    RCC_HPRE = 4,
    RCC_PPRE1 = 10,
    RCC_PPRE2 = 13,
    RCC_RTCPRE = 16,
    RCC_MCO1 = 22,
    RCC_I2SSCR = 23,
    RCC_MCO1PRE= 24,
    RCC_MCO2PRE = 27,
    RCC_MCO2 = 31,

}RCC_CFGR_Bits_t;

typedef enum
{
    RCC_GPIOARST = 0,
    RCC_GPIOBRST = 1,
    RCC_GPIOCRST = 2,
    RCC_CRCRST = 12,
    RCC_DMA1RST = 21,
    RCC_DMA2RST = 22,
}RCC_AHB1RSTR_Bits_t;

typedef enum
{
    RCC_OTGFSRST = 7,
}RCC_AHB2RSTR_Bits_t;

typedef enum
{
    RCC_TIM2RST = 0,
    RCC_TIM3RST = 1,
    RCC_TIM4RST = 2,
    RCC_TIM5RST = 3,
    RCC_WWDGRST = 11,
    RCC_SPI2RST = 14,
    RCC_SPI3RST = 15,
    RCC_USART2RST = 17,
    RCC_I2C1RST = 21,
    RCC_I2C2RST = 22,
    RCC_I2C3RST = 23,
    RCC_PWRRST = 28,

}RCC_APB1RSTR_Bits_t;


typedef enum
{
    RCC_TIM1RST = 0,
    RCC_USART1RST = 4,
    RCC_USART6RST = 5,
    RCC_ADC1_RST = 8,
    RCC_SDIORST = 11,
    RCC_SPI1RST = 12,
    RCC_SPI4RST = 13,
    RCC_SYSCFGRST = 14,
    RCC_TIM9RST = 16,
    RCC_TIM10RST = 17,
    RCC_TIM11RST = 18,

}RCC_APB2RSTR_Bits_t;

typedef enum
{
    RCC_GPIOAEN = 0,
    RCC_GPIOBEN = 1,
    RCC_GPIOCEN = 2,
    RCC_CRCEN = 12,
    RCC_DMA1EN = 21,
    RCC_DMA2EN = 22,
}RCC_AHB1ENR_Bits_t;

typedef enum
{
    RCC_OTGFSEN = 7,
}RCC_AHB2ENR_Bits_t;

typedef enum
{
    RCC_TIM2EN = 0,
    RCC_TIM3EN = 1,
    RCC_TIM4EN = 2,
    RCC_TIM5EN = 3,
    RCC_WWDGEN = 11,
    RCC_SPI2EN = 14,
    RCC_SPI3EN = 15,
    RCC_USART2EN = 17,
    RCC_I2C1EN = 21,
    RCC_I2C2EN = 22,
    RCC_I2C3EN = 23,
    RCC_PWREN = 28,

}RCC_APB1ENR_Bits_t;


typedef enum
{
    RCC_TIM1EN = 0,
    RCC_USART1EN = 4,
    RCC_USART6EN = 5,
    RCC_ADC1_EN = 8,
    RCC_SDIOEN = 11,
    RCC_SPI1EN = 12,
    RCC_SPI4EN = 13,
    RCC_SYSCFGEN = 14,
    RCC_TIM9EN = 16,
    RCC_TIM10EN = 17,
    RCC_TIM11EN = 18,

}RCC_APB2ENR_Bits_t;

typedef enum
{
    RCC_GPIOALPEN = 0,
    RCC_GPIOBLPEN = 1,
    RCC_GPIOCLPEN = 2,
    RCC_CRCLPEN = 12,
    RCC_DMA1LPEN = 21,
    RCC_DMA2LPEN = 22,
}RCC_AHB1LPENR_Bits_t;

typedef enum
{
    RCC_OTGFSLPEN = 7,
}RCC_AHB2LPENR_Bits_t;

typedef enum
{
    RCC_TIM2LPEN = 0,
    RCC_TIM3LPEN = 1,
    RCC_TIM4LPEN = 2,
    RCC_TIM5LPEN = 3,
    RCC_WWDGLPEN = 11,
    RCC_SPI2LPEN = 14,
    RCC_SPI3LPEN = 15,
    RCC_USART2LPEN = 17,
    RCC_I2C1LPEN = 21,
    RCC_I2C2LPEN = 22,
    RCC_I2C3LPEN = 23,
    RCC_PWRLPEN = 28,

}RCC_APB1LPENR_Bits_t;


typedef enum
{
    RCC_TIM1LPEN = 0,
    RCC_USART1LPEN = 4,
    RCC_USART6LPEN = 5,
    RCC_ADC1_LPEN = 8,
    RCC_SDIOLPEN = 11,
    RCC_SPI1LPEN = 12,
    RCC_SPI4LPEN = 13,
    RCC_SYSCFGLPEN = 14,
    RCC_TIM9LPEN = 16,
    RCC_TIM10LPEN = 17,
    RCC_TIM11LPEN = 18,

}RCC_APB2LPENR_Bits_t;

typedef enum
{
    RCC_LSEON = 0,
    RCC_LSERDY = 1,
    RCC_LSEBYP = 2,
    RCC_RTCSEL = 8,
    RCC_RTCEN = 15,
    RCC_BDRST = 16,
}RCC_CDCR_Bits_t;

typedef enum
{
    RCC_LSION = 0,
    RCC_LSIRDY = 1,
    RCC_RMVF = 24,
    RCC_BORRSTF = 25,
    RCC_PINRSTF = 26,
    RCC_PORRSTF = 27,
    RCC_SETRSTF = 28,
    RCC_IWDGRSTF = 29,
    RCC_WWDGRSTF = 30,
    RCC_LPWRRSTF = 31,

}RCC_CSR_Bits_t;

RCC_ErrorStatus_t RCC_EnableClk(uint8_t Copy_Clk)
{
    RCC_ErrorStatus_t Ret_ErrorStatus = RCC_OK;
    uint16_t Local_Timeout = RCC_TIMEOUT_PERIOD;
    switch(Copy_Clk)
    {
        case RCC_CLK_HSI:
            RCC->CR |= (1<<RCC_HSION);
            /* Wait till Ready */
            while( (((RCC->CR >> RCC_HSIRDY) & 1) == 0) && (!Local_Timeout) ){Local_Timeout--;}
            break;

        case RCC_CLK_HSE:
            RCC->CR |= (1<<RCC_HSEON);
            /* Wait till Ready */
            while( (((RCC->CR >> RCC_HSERDY) & 1) == 0) && (!Local_Timeout) ){Local_Timeout--;}
            break;

        case RCC_CLK_PLL:
            RCC->CR |= (1<<RCC_PLLON);
            /* Wait till Ready */
            while( (((RCC->CR >> RCC_PLLRDY) & 1) == 0) && (!Local_Timeout) ){Local_Timeout--;}
            break;

        case RCC_CLK_PLL2:
            RCC->CR |= (1<<RCC_PLLI2SON);
            /* Wait till Ready */
            while( (((RCC->CR >> RCC_PLLI2SRDY) & 1) == 0) && (!Local_Timeout) ){Local_Timeout--;}
            break;

        case RCC_CLK_LSE:
            RCC->BDCR |= (1<<RCC_LSEON);
            /* Wait till Ready */
            while( (((RCC->BDCR >> RCC_LSERDY) & 1) == 0) && (!Local_Timeout) ){Local_Timeout--;}
            break;

        case RCC_CLK_LSI:
            RCC->CSR |= (1<<RCC_LSION);
            /* Wait till Ready */
            while( (((RCC->CSR >> RCC_LSIRDY) & 1) == 0) && (!Local_Timeout) ){Local_Timeout--;}
            break;

        default:
            Ret_ErrorStatus = RCC_NOK;
    }


    return Ret_ErrorStatus;
}



RCC_ErrorStatus_t RCC_DisableClk(uint8_t Copy_Clk)
{
    RCC_ErrorStatus_t Ret_ErrorStatus = RCC_OK;

    if( (RCC->CFGR & RCC_SYSCLK_STATE_MASK) == Copy_Clk)
    {
        Ret_ErrorStatus = RCC_NOK;
    }
    else
    {
        switch(Copy_Clk)
        {
            case RCC_CLK_HSI:
                RCC->CR &= ~(1<<RCC_HSION);
                break;

            case RCC_CLK_HSE:
                RCC->CR &= ~(1<<RCC_HSEON);
                break;

            case RCC_CLK_PLL:
                RCC->CR &= ~(1<<RCC_PLLON);
                break;

            case RCC_CLK_PLL2:
                RCC->CR &= ~(1<<RCC_PLLI2SON);
                break;

            case RCC_CLK_LSE:
                RCC->BDCR &= ~(1<<RCC_LSEON);
                break;

            case RCC_CLK_LSI:
                RCC->CSR &= ~(1<<RCC_LSION);
                break;

            default:
                Ret_ErrorStatus = RCC_NOK;
        }
    }

    return Ret_ErrorStatus;
}




RCC_ErrorStatus_t RCC_SetSystemClk(uint8_t Copy_SystemClk)
{
    RCC_ErrorStatus_t Ret_ErrorStatus = RCC_OK;
    uint32_t Local_TmpReg;

    if(
        ( (Copy_SystemClk == RCC_SYSCLK_HSI) && !( (RCC->CR >> RCC_HSION) & 1 ) )
        ||
        ( (Copy_SystemClk == RCC_SYSCLK_HSE) && !( (RCC->CR >> RCC_HSEON) & 1 ) )
        ||
        ( (Copy_SystemClk == RCC_SYSCLK_PLL) && !( (RCC->CR >> RCC_PLLON) & 1 ) )
                                                                                )

    {
        Ret_ErrorStatus = RCC_NOK;
    }
    else
    {
        Local_TmpReg = RCC->CFGR;
        Local_TmpReg &= (~RCC_SYSCLK_MASK);
        Local_TmpReg |= Copy_SystemClk;
        RCC->CFGR = Local_TmpReg;

    }

    return Ret_ErrorStatus;
}



RCC_ErrorStatus_t RCC_SetPllSource(uint8_t Copy_SystemClk)
{
    RCC_ErrorStatus_t Ret_ErrorStatus = RCC_OK;

    switch(Copy_SystemClk)
    {
        case RCC_SYSCLK_HSI:
            if(!((RCC->CR >> RCC_HSION) & 1) )
            {
                Ret_ErrorStatus = RCC_NOK;
            }
            else
            {
                RCC->PLLCFGR &= ~(1<<RCC_PLLSRC);
            }
            break;

        case RCC_SYSCLK_HSE:
            if(!((RCC->CR >> RCC_HSEON) & 1) )
            {
                Ret_ErrorStatus = RCC_NOK;
            }
            else
            {
                RCC->PLLCFGR |= (1<<RCC_PLLSRC);
            }
            break;

        default:
           Ret_ErrorStatus = RCC_NOK;
    }

    return Ret_ErrorStatus;
}



RCC_ErrorStatus_t RCC_SetPllClk(uint8_t Copy_M, uint16_t Copy_N, uint8_t Copy_P, uint8_t Copy_Q)
{
    RCC_ErrorStatus_t Ret_ErrorStatus = RCC_OK;
    uint32_t Local_TmpReg;

    if(
        (Copy_M < 2 || Copy_M > 63)
        ||
        (Copy_N < 2 || Copy_N > 510 || Copy_N == 433)
        ||
        (Copy_P > RCC_PLL_P_8)
        ||
        (Copy_Q < 2 || Copy_Q > 15)
                                    )
    {
        Ret_ErrorStatus = RCC_NOK;
    }
    else
    {
        Local_TmpReg = RCC->PLLCFGR;
        Local_TmpReg &= ~RCC_PLL_M_PRESCALE_MASK;
        Local_TmpReg &= ~RCC_PLL_N_PRESCALE_MASK;
        Local_TmpReg &= ~RCC_PLL_P_PRESCALE_MASK;
        Local_TmpReg &= ~RCC_PLL_Q_PRESCALE_MASK;
        Local_TmpReg |= (Copy_M << RCC_PLLM) | (Copy_N << RCC_PLLN) | (Copy_P << RCC_PLLP) | (Copy_Q << RCC_PLLQ);
        RCC->PLLCFGR = Local_TmpReg;
    }

    return Ret_ErrorStatus;
}


RCC_ErrorStatus_t RCC_EnablePeripheral(RCC_Peripheral_t Copy_Peripheral)
{
    uint32_t Local_PeripheralCode = ((uint32_t*)&Copy_Peripheral)[1];         /* Code part of the peripheral enum */      
    RCC_ErrorStatus_t Ret_ErrorStatus = RCC_OK;

    /* Check for the Peripherals enums Specific code to make sure
     * that the recived argument is only a Peripheral enum */
    if(Local_PeripheralCode != 3)
    {
        Ret_ErrorStatus = RCC_NOK;
    }
    else
    {
        if((uint32_t)Copy_Peripheral < 32)        /* AHB1 REG peripherals */
        {
            RCC->AHB1ENR |= (1<<(uint32_t)Copy_Peripheral);
        }
        else if((uint32_t)Copy_Peripheral < 64)   /* AHB2 REG peripherals */
        {
            RCC->AHB2ENR |= (1<< ((uint32_t)Copy_Peripheral - 32) );
        }
        else if((uint32_t)Copy_Peripheral < 96)   /* APB1 REG peripherals */
        {
            RCC->APB1ENR |= (1<< ((uint32_t)Copy_Peripheral - 64) );
        }
        else if((uint32_t)Copy_Peripheral < 128)  /* APB2 REG peripherals */
        {
            RCC->APB2ENR |= (1<< ((uint32_t)Copy_Peripheral - 96) );
        }
        else
        {
            Ret_ErrorStatus = RCC_NOK;
        }
    }

    return Ret_ErrorStatus;
}


RCC_ErrorStatus_t RCC_DisablePeripheral(RCC_Peripheral_t Copy_Peripheral)
{
    uint32_t Local_PeripheralCode = ((uint32_t*)&Copy_Peripheral)[1];         /* Code part of the peripheral enum */
    RCC_ErrorStatus_t Ret_ErrorStatus = RCC_OK;

    if(Local_PeripheralCode != 3)
    {
        Ret_ErrorStatus = RCC_NOK;
    }
    else
    {
        if((uint32_t)Copy_Peripheral < 32)        /* AHB1 REG peripherals */
        {
            RCC->AHB1ENR &= ~(1<<(uint32_t)Copy_Peripheral);
        }
        else if((uint32_t)Copy_Peripheral < 64)   /* AHB2 REG peripherals */
        {
            RCC->AHB2ENR &= ~(1<< ((uint32_t)Copy_Peripheral - 32) );
        }
        else if((uint32_t)Copy_Peripheral < 96)   /* APB1 REG peripherals */
        {
            RCC->APB1ENR &= ~(1<< ((uint32_t)Copy_Peripheral - 64) );
        }
        else if((uint32_t)Copy_Peripheral < 128)  /* APB2 REG peripherals */
        {
            RCC->APB2ENR &= ~(1<< ((uint32_t)Copy_Peripheral - 96) );
        }
        else
        {
            Ret_ErrorStatus = RCC_NOK;
        }
    }

    return Ret_ErrorStatus;
}


RCC_ErrorStatus_t RCC_SetAHBPrescale(uint8_t Copy_AHB_Prescale)
{
    RCC_ErrorStatus_t Ret_ErrorStatus = RCC_OK;
    uint32_t Local_TmpReg;

    if( (Copy_AHB_Prescale < RCC_AHB_PRESCALE_1) || (Copy_AHB_Prescale > RCC_AHB_PRESCALE_512) )
    {
        Ret_ErrorStatus = RCC_NOK;
    }
    else
    {
        Local_TmpReg = RCC->CFGR;
        Local_TmpReg &= ~RCC_AHB_PRESCALE_MASK;
        Local_TmpReg |= (Copy_AHB_Prescale << RCC_HPRE);
        RCC->CFGR = Local_TmpReg;
    }

    return Ret_ErrorStatus;
}


RCC_ErrorStatus_t RCC_SetAPB1Prescale(uint8_t Copy_APB_Prescale)
{
    RCC_ErrorStatus_t Ret_ErrorStatus = RCC_OK;
    uint32_t Local_TmpReg;

    if( (Copy_APB_Prescale < RCC_APB_PRESCALE_1) || (Copy_APB_Prescale > RCC_APB_PRESCALE_16) )
    {
        Ret_ErrorStatus = RCC_NOK;
    }
    else
    {
        Local_TmpReg = RCC->CFGR;
        Local_TmpReg &= ~RCC_APB1_PRESCALE_MASK;
        Local_TmpReg |= (Copy_APB_Prescale << RCC_PPRE1);
        RCC->CFGR = Local_TmpReg;
    }

    return Ret_ErrorStatus;
}


RCC_ErrorStatus_t RCC_SetAPB2Prescale(uint8_t Copy_APB_Prescale)
{
    RCC_ErrorStatus_t Ret_ErrorStatus = RCC_OK;
    uint32_t Local_TmpReg;

    if( (Copy_APB_Prescale < RCC_APB_PRESCALE_1) || (Copy_APB_Prescale > RCC_APB_PRESCALE_16) )
    {
        Ret_ErrorStatus = RCC_NOK;
    }
    else
    {
        Local_TmpReg = RCC->CFGR;
        Local_TmpReg &= ~RCC_APB2_PRESCALE_MASK;
        Local_TmpReg |= (Copy_APB_Prescale << RCC_PPRE2);
        RCC->CFGR = Local_TmpReg;
    }

    return Ret_ErrorStatus;
}



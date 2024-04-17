/*
 * GPIO.c
 *
 *  Created on: Feb 13, 2024
 *      Author: Osman
 */

#include "LIB/STD_TYPES.h"
#include "MCAL/GPIO/GPIO.h"

/* ============================================================================ */
/*                                  MASKS MACROS                           */
/* ============================================================================ */

#define GPIO_MODE_MASK              0x00000003
#define GPIO_OTYPE_MASK             0x00000004
#define GPIO_PUPD_MASK              0x00000018
#define GPIO_2_BIT_MASK             0x00000003
#define GPIO_4_BIT_MASK             0x0000000F
#define GPIO_PIN_OFFSET_2           0x00000002      /* In the case of registers where each pin is represented by 2 bits */
#define GPIO_PIN_OFFSET_4           0x00000004      /* In the case of registers where each pin is represented by 4 bits */
#define GPIO_BSRR_RESET_OFFSET      0x00000010      /* To access the Reset part of the BSRR Register */
#define GPIO_OTYPE_POS_IN_MODE		0x00000002		/* Position of OTYPE bit in the MODE Macro */
#define GPIO_PUPD_POS_IN_MODE		0x00000003		/* Position of PUPD bit in the MODE Macro */



/* ============================================================================ */
/*                                  VALIDATION MACROS                           */
/* ============================================================================ */

#define IS_GPIO_MODE(MODE)      (((MODE) == GPIO_MODE_IN)       || \
                                ((MODE) == GPIO_MODE_IN_FL)    || \
                                ((MODE) == GPIO_MODE_IN_PU)    || \
                                ((MODE) == GPIO_MODE_IN_PD)    || \
                                ((MODE) == GPIO_MODE_OP)       || \
                                ((MODE) == GPIO_MODE_OP_PP)    || \
                                ((MODE) == GPIO_MODE_OP_PP_PU) || \
                                ((MODE) == GPIO_MODE_OP_PP_PD) || \
                                ((MODE) == GPIO_MODE_OP_OD)    || \
                                ((MODE) == GPIO_MODE_OP_OD_PU) || \
                                ((MODE) == GPIO_MODE_OP_OD_PD) || \
                                ((MODE) == GPIO_MODE_AF)       || \
                                ((MODE) == GPIO_MODE_AF_PP)    || \
                                ((MODE) == GPIO_MODE_AF_PP_PU) || \
                                ((MODE) == GPIO_MODE_AF_PP_PD) || \
                                ((MODE) == GPIO_MODE_AF_OD)    || \
                                ((MODE) == GPIO_MODE_AF_OD_PU) || \
                                ((MODE) == GPIO_MODE_AF_OD_PD) || \
                                ((MODE) == GPIO_MODE_ANALOG)) 


#define IS_GPIO_PORT(PORT)     (((PORT) == GPIO_PORT_A) || \
                                ((PORT) == GPIO_PORT_B) || \
                                ((PORT) == GPIO_PORT_C))

#define IS_GPIO_PIN(PORT)      (((PORT) >= GPIO_PIN_0) && ((PORT) <= GPIO_PIN_15))

#define IS_GPIO_STATE(STATE)   (((STATE) == GPIO_STATE_HIGH) || ((STATE) == GPIO_STATE_LOW))

#define IS_GPIO_SPEED(SPEED)   (((SPEED) >= GPIO_SPEED_LOW) && ((SPEED) <= GPIO_SPEED_VHIGH ))

#define IS_GPIO_AF(AF)         (((AF) == GPIO_AF_SYSTEM)   || \
                                ((AF) == GPIO_AF_TIM1_2)   || \
                                ((AF) == GPIO_AF_TIM3_5)   || \
                                ((AF) == GPIO_AF_TIME9_11) || \
                                ((AF) == GPIO_AF_I2C1_3)   || \
                                ((AF) == GPIO_AF_SPI1_4)   || \
                                ((AF) == GPIO_AF_SPI3)     || \
                                ((AF) == GPIO_AF_USART1_2) || \
                                ((AF) == GPIO_AF_USART6)   || \
                                ((AF) == GPIO_AF_I2C2_3)   || \
                                ((AF) == GPIO_AF_OTG_FS)   || \
                                ((AF) == GPIO_AF_SDIO)     || \
                                ((AF) == GPIO_AF_EVENTOUT))


/* ============================================================================ */
/*                                    TYPES                                     */
/* ============================================================================ */
typedef struct
{
    uint32_t MODER;
    uint32_t OTYPER;
    uint32_t OSPEEDR;
    uint32_t PUPDR;
    uint32_t IDR;
    uint32_t ODR;
    uint32_t BSRR;
    uint32_t LCKR;
    u64 AFR;
//    uint32_t AFRL;
//    uint32_t AFRH;

}GPIO_Registers_t;




/* ========================================================================================= */
/*                                      IMPLEMENTATION                                       */
/* ========================================================================================= */

GPIO_ErrorStatus_t GPIO_Init(GPIO_Pin_t* ADD_Config)
{
    GPIO_ErrorStatus_t Ret_ErrorStatus = GPIO_OK;
    uint32_t Local_TmpReg;
    u64 Local_AFTmpReg; /* AFH and AFL are treated as one 64bit Register */

    if(ADD_Config == NULL)
    {
        Ret_ErrorStatus = GPIO_NULLPTR;
    }
    else if(!IS_GPIO_PORT(ADD_Config->Port))
    {
        Ret_ErrorStatus = GPIO_INVALID_PORT;
    }
    else if(!IS_GPIO_PIN(ADD_Config->Pin))
    {
        Ret_ErrorStatus = GPIO_INVALID_PIN;
    }
    else if(!IS_GPIO_MODE(ADD_Config->Mode))
    {
        Ret_ErrorStatus = GPIO_INVALID_MODE;
    }
    else if(!IS_GPIO_SPEED(ADD_Config->Speed))
    {
        Ret_ErrorStatus = GPIO_INVALID_SPEED;
    }
    else if(!IS_GPIO_AF(ADD_Config->AF))
    {
        Ret_ErrorStatus = GPIO_INVALID_AF;
    }
    else
    {
        /* do nothing */
    }

    if(Ret_ErrorStatus == GPIO_OK)
    {
        /* Set the Pin Mode (Input, Output, AF, Analog)*/
        Local_TmpReg = ((GPIO_Registers_t *)(ADD_Config->Port))->MODER;
        Local_TmpReg &= ~( GPIO_2_BIT_MASK << (ADD_Config->Pin * GPIO_PIN_OFFSET_2)) ;
        Local_TmpReg |= ( (ADD_Config->Mode & GPIO_MODE_MASK) << (ADD_Config->Pin * GPIO_PIN_OFFSET_2) );
        ((GPIO_Registers_t *)(ADD_Config->Port))->MODER = Local_TmpReg;

        /* Set the Pin output type (Push Pull or Open Drain)*/
        Local_TmpReg = ((GPIO_Registers_t *)(ADD_Config->Port))->OTYPER;
        Local_TmpReg &= ~ ( 1 << (ADD_Config->Pin) );
        Local_TmpReg |= ( ((ADD_Config->Mode & GPIO_OTYPE_MASK) >> GPIO_OTYPE_POS_IN_MODE) << (ADD_Config->Pin) );
        ((GPIO_Registers_t *)(ADD_Config->Port))->OTYPER = Local_TmpReg;

        /* Set pin pullup/pulldown/floating */
        Local_TmpReg = ((GPIO_Registers_t *)(ADD_Config->Port))->PUPDR;
        Local_TmpReg &= ~( (GPIO_2_BIT_MASK)<< (ADD_Config->Pin*GPIO_PIN_OFFSET_2) );
        Local_TmpReg |= ( ((ADD_Config->Mode & GPIO_PUPD_MASK) >> GPIO_PUPD_POS_IN_MODE) << (ADD_Config->Pin * GPIO_PIN_OFFSET_2) );
        ((GPIO_Registers_t *)(ADD_Config->Port))->PUPDR = Local_TmpReg;

        /* Set Pin speed */
        Local_TmpReg = ((GPIO_Registers_t *)(ADD_Config->Port))->OSPEEDR;
        Local_TmpReg &= ~( (GPIO_2_BIT_MASK)<< (ADD_Config->Pin*GPIO_PIN_OFFSET_2) );
        Local_TmpReg |= ( (ADD_Config->Speed) << (ADD_Config->Pin * GPIO_PIN_OFFSET_2) );
        ((GPIO_Registers_t *)(ADD_Config->Port))->OSPEEDR = Local_TmpReg;


        /* Set AF */
        Local_AFTmpReg = ((GPIO_Registers_t *)(ADD_Config->Port))->AFR;
        Local_AFTmpReg &= ~( ((u64)GPIO_4_BIT_MASK) << (ADD_Config->Pin*GPIO_PIN_OFFSET_4) );
        Local_AFTmpReg |= ( ((u64)ADD_Config->AF) << (ADD_Config->Pin*GPIO_PIN_OFFSET_4) );
        ((GPIO_Registers_t *)(ADD_Config->Port))->AFR = Local_AFTmpReg;

    //    if(ADD_Config->Pin < 8)
    //    {
    //    	Local_TmpReg = ((GPIO_Registers_t *)(ADD_Config->Port))->AFRL;
    //		Local_TmpReg &= ~( (GPIO_4_BIT_MASK)<< (ADD_Config->Pin*GPIO_PIN_OFFSET_4) );
    //		Local_TmpReg |= ( (ADD_Config->AF) << (ADD_Config->Pin * GPIO_PIN_OFFSET_4) );
    //		((GPIO_Registers_t *)(ADD_Config->Port))->AFRL = Local_TmpReg;
    //    }
    //    else
    //    {
    //    	Local_TmpReg = ((GPIO_Registers_t *)(ADD_Config->Port))->AFRH;
    //		Local_TmpReg &= ~( (GPIO_4_BIT_MASK)<< ((ADD_Config->Pin - 8) * GPIO_PIN_OFFSET_4) );
    //		Local_TmpReg |= ( (ADD_Config->AF) << ((ADD_Config->Pin - 8) * GPIO_PIN_OFFSET_4) );
    //		((GPIO_Registers_t *)(ADD_Config->Port))->AFRH = Local_TmpReg;
    //    }

    }

    return Ret_ErrorStatus;
}


GPIO_ErrorStatus_t GPIO_SetPinState(void * Copy_Port, uint32_t Copy_Pin, uint8_t Copy_State)
{
    GPIO_ErrorStatus_t Ret_ErrorStatus = GPIO_OK;
    
    if(!IS_GPIO_PORT(Copy_Port))
    {
        Ret_ErrorStatus = GPIO_INVALID_PORT;
    }
    else if(!IS_GPIO_PIN(Copy_Pin))
    {
        Ret_ErrorStatus = GPIO_INVALID_PIN;
    }
    else if(!IS_GPIO_STATE(Copy_State))
    {
        Ret_ErrorStatus = GPIO_INVALID_STATE;
    }
    else
    {
        /* do nothing */
    }

    if(Ret_ErrorStatus == GPIO_OK)
    {
        switch(Copy_State)
        {
            case GPIO_STATE_LOW:
                ((GPIO_Registers_t *)Copy_Port)->BSRR = ( 1<< (Copy_Pin+GPIO_BSRR_RESET_OFFSET) );
                break;

            case GPIO_STATE_HIGH:
                ((GPIO_Registers_t *)Copy_Port)->BSRR = (1<< Copy_Pin);
                break;

            default:
                Ret_ErrorStatus = GPIO_NOK;
        }
    }
    

    return Ret_ErrorStatus;
}


GPIO_ErrorStatus_t GPIO_GetPinState(void * Copy_Port, uint32_t Copy_Pin, uint8_t *Add_PinState)
{
    GPIO_ErrorStatus_t Ret_ErrorStatus = GPIO_OK;

    if(!IS_GPIO_PORT(Copy_Port))
    {
        Ret_ErrorStatus = GPIO_INVALID_PORT;
    }
    else if(!IS_GPIO_PIN(Copy_Pin))
    {
        Ret_ErrorStatus = GPIO_INVALID_PIN;
    }
    else if(Add_PinState == NULL)
    {
        Ret_ErrorStatus = GPIO_NULLPTR;
    }
    else
    {
        /* do nothing */
    }

    if(Ret_ErrorStatus == GPIO_OK)
    {
        *Add_PinState = ( ((GPIO_Registers_t *)Copy_Port)->IDR & ( 1 << Copy_Pin) ) >> Copy_Pin;
    }


    return Ret_ErrorStatus;
}


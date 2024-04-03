/*
 * GPIO.h
 *
 *  Created on: Feb 13, 2024
 *      Author: Osman
 */

#ifndef MCAL_GPIO_GPIO_H_
#define MCAL_GPIO_GPIO_H_

/*******Macros for the GPIO pins*******/
#define GPIO_PIN_0  0x00000000
#define GPIO_PIN_1  0x00000001
#define GPIO_PIN_2  0x00000002
#define GPIO_PIN_3  0x00000003
#define GPIO_PIN_4  0x00000004
#define GPIO_PIN_5  0x00000005
#define GPIO_PIN_6  0x00000006
#define GPIO_PIN_7  0x00000007
#define GPIO_PIN_8  0x00000008
#define GPIO_PIN_9  0x00000009
#define GPIO_PIN_10 0x0000000A
#define GPIO_PIN_11 0x0000000B
#define GPIO_PIN_12 0x0000000C
#define GPIO_PIN_13 0x0000000D
#define GPIO_PIN_14 0x0000000E
#define GPIO_PIN_15 0x0000000F

/*******Macros for the GPIO ports*******/
#define GPIO_PORT_A  (void *)0x40020000
#define GPIO_PORT_B  (void *)0x40020400
#define GPIO_PORT_C  (void *)0x40020800

/*******Macros for the GPIO pin modes*******/
#define GPIO_MODE_IN       0x00000000
#define GPIO_MODE_IN_FL    0x00000000
#define GPIO_MODE_IN_PU    0x00000008
#define GPIO_MODE_IN_PD    0x00000010

#define GPIO_MODE_OP       0x00000001
#define GPIO_MODE_OP_PP    0x00000001
#define GPIO_MODE_OP_PP_PU 0x00000009
#define GPIO_MODE_OP_PP_PD 0x00000011
#define GPIO_MODE_OP_OD    0x00000005
#define GPIO_MODE_OP_OD_PU 0x0000000D
#define GPIO_MODE_OP_OD_PD 0x00000015

#define GPIO_MODE_AF       0x00000002
#define GPIO_MODE_AF_PP    0x00000002
#define GPIO_MODE_AF_PP_PU 0x0000000A
#define GPIO_MODE_AF_PP_PD 0x00000012
#define GPIO_MODE_AF_OD    0x00000006
#define GPIO_MODE_AF_OD_PU 0x0000000E
#define GPIO_MODE_AF_OD_PD 0x00000016

#define GPIO_MODE_ANALOG   0x00000003

/*******Macros for the GPIO pin speeds*******/
#define GPIO_SPEED_LOW     0x00000000
#define GPIO_SPEED_MED     0x00000001
#define GPIO_SPEED_HIGH    0x00000002
#define GPIO_SPEED_VHIGH   0x00000003

/*******Macros for Pin states*******/
#define GPIO_STATE_HIGH          0x00000001
#define GPIO_STATE_LOW           0x00000000


/*******Macros for The Alternate Functions*******/
#define GPIO_AF_SYSTEM          0x00000000
#define GPIO_AF_TIM1_2          0x00000001
#define GPIO_AF_TIM3_5          0x00000002
#define GPIO_AF_TIME9_11        0x00000003
#define GPIO_AF_I2C1_3          0x00000004
#define GPIO_AF_SPI1_4          0x00000005
#define GPIO_AF_SPI3            0x00000006
#define GPIO_AF_USART1_2        0x00000007
#define GPIO_AF_USART6          0x00000008
#define GPIO_AF_I2C2_3          0x00000009
#define GPIO_AF_OTG_FS          0x0000000A
#define GPIO_AF_SDIO            0x0000000C
#define GPIO_AF_EVENTOUT        0x0000000F


/* Pin configuration Struct */
typedef struct
{
    void* Port;
    u32 Pin;
    u32 Mode;
    u32 Speed;
    u32 AF;
}GPIO_Pin_t;

typedef enum
{
    GPIO_OK,
    GPIO_NOK,
    GPIO_NULLPTR,
    GPIO_INVALID_MODE,
    GPIO_INVALID_AF,
    GPIO_INVALID_PORT,
    GPIO_INVALID_PIN,
    GPIO_INVALID_SPEED,
    GPIO_INVALID_STATE,

}GPIO_ErrorStatus_t;


/**
 * @brief Initializes the Pin Mode and Speed according to the 
 * sent Configuration struct
 * 
 * @param ADD_Config Pointer to struct of Pin configurations
 * @return GPIO_ErrorStatus_t Error Status
 */
GPIO_ErrorStatus_t GPIO_Init(GPIO_Pin_t* ADD_Config);

/**
 * @brief Sets the selected pin state as either High or Low
 * 
 * @param Copy_Port Port containing the Pin
 * @param Copy_Pin Pin number in the port
 * @param Copy_State State to set the Pin to (High, Low)
 * @return GPIO_ErrorStatus_t Error status 
 */
GPIO_ErrorStatus_t GPIO_SetPinState(void * Copy_Port, u32 Copy_Pin, u8 Copy_State);


/**
 * @brief Returns the selected Pin state 
 * 
 * @param Copy_Port Port containing the Pin
 * @param Copy_Pin Pin number in the port
 * @param Add_PinState The returned state
 * @return GPIO_ErrorStatus_t Error status
 */
GPIO_ErrorStatus_t GPIO_GetPinState(void * Copy_Port, u32 Copy_Pin, u8 *Add_PinState);


#endif /* MCAL_GPIO_GPIO_H_ */

/******************************************************************************
 *
 * Module: GPIO
 *
 * File Name: GPIO.h
 *
 * Description: Header file for the GPIO driver Configurations for STM32F401xC
 *
 * Author: Momen Elsayed Shaban
 *
 *******************************************************************************/
#ifndef GPIO_H_
#define GPIO_H_

#include "LIB/std_types.h"

/*******************************************************************************
 *                                Type Decelerations                           *
 *******************************************************************************/
/*******Macros for the GPIO pins*******/
#define GPIO_PIN_0  		0x00000000
#define GPIO_PIN_1  		0x00000001
#define GPIO_PIN_2  		0x00000002
#define GPIO_PIN_3  		0x00000003
#define GPIO_PIN_4  		0x00000004
#define GPIO_PIN_5  		0x00000005
#define GPIO_PIN_6  		0x00000006
#define GPIO_PIN_7  		0x00000007
#define GPIO_PIN_8  		0x00000008
#define GPIO_PIN_9  		0x00000009
#define GPIO_PIN_10 		0x0000000A
#define GPIO_PIN_11 		0x0000000B
#define GPIO_PIN_12 		0x0000000C
#define GPIO_PIN_13 		0x0000000D
#define GPIO_PIN_14 		0x0000000E
#define GPIO_PIN_15 		0x0000000F

/*******Macros for the GPIO ports*******/
#define GPIO_PORT_A  		(void*)0x40020000 /*casted to void to be used for any hardware with different rgister size*/
#define GPIO_PORT_B  		(void*)0x40020400
#define GPIO_PORT_C  		(void*)0x40020800


/*******Macros for the GPIO pin modes*******/
#define GPIO_MODE_IN_FL 	  0x00000000
#define GPIO_MODE_IN_PU 	  0x00000008
#define GPIO_MODE_IN_PD 	  0x00000010

#define GPIO_MODE_OUT_PP   	0x00000001
#define GPIO_MODE_OUT_PP_PU	0x00000009
#define GPIO_MODE_OUT_PP_PD	0x00000011
#define GPIO_MODE_OUT_OD   	0x00000005
#define GPIO_MODE_OUT_OD_PU	0x0000000D
#define GPIO_MODE_OUT_OD_PD	0x00000015

#define GPIO_MODE_AF_PP    	0x00000002
#define GPIO_MODE_AF_PP_PU 	0x0000000A
#define GPIO_MODE_AF_PP_PD 	0x00000012
#define GPIO_MODE_AF_OD    	0x00000006
#define GPIO_MODE_AF_OD_PU 	0x0000000E
#define GPIO_MODE_AF_OD_PD 	0x00000016

#define GPIO_MODE_AN       	0x00000003

/*******Macros for the Alternate Functions*******/
#define GPIO_FUNC_AF0       0x00000000
#define GPIO_FUNC_AF1       0x00000001
#define GPIO_FUNC_AF2       0x00000002
#define GPIO_FUNC_AF3       0x00000003
#define GPIO_FUNC_AF4       0x00000004
#define GPIO_FUNC_AF5       0x00000005
#define GPIO_FUNC_AF6       0x00000006
#define GPIO_FUNC_AF7       0x00000007
#define GPIO_FUNC_AF8       0x00000008
#define GPIO_FUNC_AF9       0x00000009
#define GPIO_FUNC_AF10      0x0000000A
#define GPIO_FUNC_AF11      0x0000000B
#define GPIO_FUNC_AF12      0x0000000C
#define GPIO_FUNC_AF13      0x0000000D
#define GPIO_FUNC_AF14      0x0000000E
#define GPIO_FUNC_AF15      0x0000000F

/*******Macros for the GPIO pin speeds*******/
#define GPIO_SPEED_LOW     	0x00000000
#define GPIO_SPEED_MED     	0x00000001
#define GPIO_SPEED_HIGH    	0x00000002
#define GPIO_SPEED_VHIGH   	0x00000003

#define GPIO_STATE_RESET    0
#define GPIO_STATE_SET      1

typedef struct
{
  u32 GPIO_Pin;
  void* GPIO_Port;
  u32 GPIO_Speed;
  u32 GPIO_Mode;
}GPIO_Pin_t;

typedef enum{
    GPIO_OK,
    GPIO_InvalidPort,
    GPIO_InvalidPin,
    GPIO_InvalidMode,
    GPIO_InvalidSpeed,
    GPIO_NULLPTR,
    GPIO_InvalidState,
    GPIO_NOK
}GPIO_ErrorStatus_t;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
/*****************************************************
 * Function: GPIO_Init
 * Description: Initializes the specified GPIO pin based on the provided configuration.
 *
 * Parameters:
 *   - GPIOx: Pointer to a GPIO_Pin_t structure containing pin configuration.
 *
 * Return:
 *   - GPIO_ErrorStatus_t: An error status indicating the success or failure of the GPIO initialization.
 *
 * Errors:
 *   - GPIO_InvalidPin: Returned if an invalid GPIO pin is provided.
 *   - GPIO_InvalidPort: Returned if an invalid GPIO port is provided.
 *   - GPIO_InvalidMode: Returned if an invalid GPIO mode is provided.
 *   - GPIO_InvalidSpeed: Returned if an invalid GPIO speed is provided.
 *   - GPIO_OK: Returned if the GPIO initialization is successful.
 *
 * Usage: GPIO_ErrorStatus_t error = GPIO_Init(&gpioPinConfig); 
 * Example: Configure a GPIO pin with the specified settings.
 *
 * Notes:
 *   - Ensure that no errors are present before using the GPIO pin.
 *   - Before Using GPIO Ensure that you are enabling the clock for the periphral by using RCC Driver
 *   - The GPIO_Pin_t structure holds the configuration for GPIO pins, including pin number, port, speed, and mode.
 *****************************************************/
GPIO_ErrorStatus_t GPIO_Init(GPIO_Pin_t* GPIOx);

/*****************************************************
 * Function: GPIO_setPinValue
 * Description: Sets or resets the state of a specified GPIO pin.
 *
 * Parameters:
 *   - GPIO_Port: Pointer to the GPIO port to which the pin belongs.
 *   - GPIO_pin: The GPIO pin number to be set or reset.
 *   - GPIO_State: The desired state to set the GPIO pin (GPIO_STATE_SET or GPIO_STATE_RESET).
 *
 * Return:
 *   - GPIO_ErrorStatus_t: An error status indicating the success or failure of the operation.
 *
 * Errors:
 *   - GPIO_NULLPTR: Returned if the GPIO_Port pointer is NULL.
 *   - GPIO_InvalidPin: Returned if an invalid GPIO pin is provided.
 *   - GPIO_InvalidState: Returned if an invalid GPIO state is provided.
 *   - GPIO_OK: Returned if the operation is successful.
 *
 * Usage: GPIO_ErrorStatus_t error = GPIO_setPinValue(GPIO_PORT_A, GPIO_PIN_5, GPIO_STATE_SET);
 * Example: Set GPIO pin 5 on port A to a high state.
 *
 * Notes:
 *   - Ensure that no errors are present before using the GPIO pin.
 *****************************************************/
GPIO_ErrorStatus_t GPIO_setPinValue(void* GPIO_Port, u8 GPIO_pin, u8 GPIO_State);

/*****************************************************
 * Function: GPIO_getPinValue
 * Description: Reads the current state of a specified GPIO pin.
 *
 * Parameters:
 *   - GPIO_Port: Pointer to the GPIO port to which the pin belongs.
 *   - GPIO_pin: The GPIO pin number to read.
 *   - GPIO_State: Pointer to store the read state of the GPIO pin (GPIO_STATE_SET or GPIO_STATE_RESET).
 *
 * Return:
 *   - GPIO_ErrorStatus_t: An error status indicating the success or failure of the operation.
 *
 * Errors:
 *   - GPIO_NULLPTR: Returned if the GPIO_Port pointer is NULL.
 *   - GPIO_InvalidPin: Returned if an invalid GPIO pin is provided.
 *   - GPIO_OK: Returned if the operation is successful.
 *
 * Usage: GPIO_ErrorStatus_t error; u8 state; error = GPIO_getPinValue(GPIO_PORT_A, GPIO_PIN_5, &state);
 * Example: Read the state of GPIO pin 5 on port A and store it in the 'state' variable.
 *
 * Notes:
 *   - Ensure that no errors are present before using the GPIO pin state.
 *****************************************************/
GPIO_ErrorStatus_t GPIO_getPinValue(void* GPIO_Port, u8 GPIO_pin, u8* GPIO_State);

/*****************************************************
 * Function: GPIO_CfgAlternateFn
 * Description: Configures alternate function for a GPIO pin.
 *
 * Parameters:
 *   - GPIO_Port: Pointer to the GPIO port base address.
 *   - GPIO_Pin: The GPIO pin number.
 *   - GPIO_AF: The alternate function value to be configured.
 *
 * Return:
 *   - GPIO_ErrorStatus_t: Error status indicating the success or failure of the configuration.
 *     - GPIO_OK: Configuration successful.
 *     - GPIO_NULLPTR: GPIO port pointer is NULL.
 *     - GPIO_InvalidPin: Invalid GPIO pin number.
 *
 * Usage:
 *   GPIO_ErrorStatus_t status = GPIO_CfgAlternateFn(GPIOA, GPIO_PIN_0, GPIO_AF_USART1);
 *   if(status == GPIO_OK) {
 *     // Alternate function configured successfully
 *   } else {
 *     // Handle error
 *   }
 *
 * Notes:
 *   - This function configures the alternate function for a GPIO pin.
 *   - It accepts the GPIO port base address, pin number, and the alternate function value.
 *   - If the pin number is greater than 7, it configures the alternate function in AFRH register, otherwise in AFRL register.
 *****************************************************/
GPIO_ErrorStatus_t GPIO_CfgAlternateFn(void* GPIO_Port, u32 GPIO_Pin, u32 GPIO_AF)

#endif /* GPIO_H_ */

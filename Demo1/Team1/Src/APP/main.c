
#include "MCAL/RCC/RCC.h"
#include "HAL/KEYPAD/Keypad.h"
#include "HAL/LCD/LCD_DRIVER.h"
#include "SERVICE/SCHED/sched.h"
#include "MCAL/USART/USART_DRIVER.h"
#include "HAL/LIN/LIN_MASTER/LIN_Master.h"
#include "HAL/LIN/LIN_MASTER/LIN_MasterAppData.h"
#include "HAL/LIN/LIN_SLAVE/LIN_SLave.h"
#include "HAL/LIN/LIN_SLAVE/LIN_SlaveAppData.h"
#include "MCAL/GPIO/GPIO_DRIVER.h"

int main(int argc, char* argv[])
{
	RCC_Ctrl_AHB1_Clk(RCC_GPIOA_ENABLE_DISABLE,RCC_enuPeriphralEnable);
	RCC_Ctrl_AHB1_Clk(RCC_GPIOB_ENABLE_DISABLE,RCC_enuPeriphralEnable);
	RCC_Ctrl_APB2_Clk(RCC_USART1_ENABLE_DISABLE,RCC_enuPeriphralEnable);
	RCC_Ctrl_APB1_Clk(RCC_USART2_ENABLE_DISABLE,RCC_enuPeriphralEnable);

	LIN_Pin_cfg_t LIN_Pins[4] = {[0] = {.Port = GPIO_PORT_B, .Pin = GPIO_PIN_6}, [1] = {.Port = GPIO_PORT_B, .Pin = GPIO_PIN_7},
	[2] = {.Port = GPIO_PORT_A, .Pin = GPIO_PIN_2}, 
	[3] = {.Port = GPIO_PORT_A, .Pin = GPIO_PIN_3}};

  	LIN_cfg_t LIN_Arr1 = {.TX_Pin = LIN_Pins[0], .RX_Pin = LIN_Pins[1], .address = USART_Peri_1, .BaudRate = 9600,
	.WordLength = USART_WORD_LENGTH_8, .ParityControl = USART_PARITY_DISABLE, .ParitySelect = USART_PARITY_DISABLE,
	.StopBits = USART_STOP_BITS_1, .OverSampling = USART_OVERSAMPLING_16, .LIN_Mode = USART_LIN_MODE_ENABLE,
	.LIN_IRQ = USART_LIN_IRQ_ENABLE, .LIN_BreakLength = USART_LIN_BRK_LENGTH_11};

  	LIN_cfg_t LIN_Arr2 = {.TX_Pin = LIN_Pins[2], .RX_Pin = LIN_Pins[3], .address = USART_Peri_2, .BaudRate = 9600,
	.WordLength = USART_WORD_LENGTH_8, .ParityControl = USART_PARITY_DISABLE, .ParitySelect = USART_PARITY_DISABLE,
	.StopBits = USART_STOP_BITS_1, .OverSampling = USART_OVERSAMPLING_16, .LIN_Mode = USART_LIN_MODE_ENABLE,
	.LIN_IRQ = USART_LIN_IRQ_ENABLE, .LIN_BreakLength = USART_LIN_BRK_LENGTH_11};

  	LIN_MasterInit(LIN_Arr1);
  	LIN_SlaveInit(LIN_Arr2);
	//KEYPAD_Init();
	//LCD_Init();

	SlaveSendControls(7,8);
	MasterSendControls(1,2);

	Sched_Init();
	Sched_Start();
}






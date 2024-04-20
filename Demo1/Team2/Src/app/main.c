
#include "MCAL/RCC/RCC.h"
#include "HAL/LCD/LCD_HAL.h"
#include "HAL/KEYPAD/Keypad.h"
#include "scheduler/scheduler.h"
#include "MCAL/NVIC/NVIC.h"

#include "MCAL/GPIO/GPIO.h"

#include "COMM/LIN_SLAVE/LIN_SLave.h"
#include "COMM/LIN_MASTER/LIN_Master.h"

#include "COMM/LIN_MASTER/LIN_MasterAppData.h"
#include "COMM/LIN_SLAVE/LIN_SlaveAppData.h"

#define TEST_LCD 		0
#define TEST_USART		0
#define TEST_KEYPAD		0
#define TEST_LIN		1

#if TEST_LCD == 1
int main(int argc, char* argv[])
{
	// RCC_EnableClk(RCC_CLK_HSE);
	// RCC_SetSystemClk(RCC_SYSCLK_HSE);
	RCC_EnablePeripheral(RCC_GPIOA);
	RCC_EnablePeripheral(RCC_GPIOB);
	LCD_Init();
	SCH_init();
	SCH_Start();
}
#elif TEST_USART == 1

#define TX1		0
#define RX1		1	
void toggle_Led (void);
int main(int argc, char* argv[])
{
	GPIO_Pin_t USART_pins[2] = {[TX1] = {.GPIO_Pin = GPIO_PIN_9 , .GPIO_Port = GPIO_PORT_A, .GPIO_Mode = GPIO_MODE_AF_PP, .GPIO_Speed = GPIO_SPEED_HIGH},
								[RX1] = {.GPIO_Pin = GPIO_PIN_10, .GPIO_Port = GPIO_PORT_A, .GPIO_Mode = GPIO_MODE_AF_PP, .GPIO_Speed = GPIO_SPEED_HIGH}};
	uint8_t data1 ;
	uint8_t data2[] = "MOMEN";
	USART_Req_t USART_Req1 = {.data = &data1, .length = 1, .USART_Number= USART1 ,.CB = NULL_PTR};
	USART_Req_t USART_Req2 = {.data = data2, .length = 6, .USART_Number= USART1 ,.CB = toggle_Led};

	RCC_Ctrl_AHB1_Clk(RCC_GPIOA_ENABLE_DISABLE,RCC_enuPeriphralEnable);
	RCC_Ctrl_APB2_Clk(RCC_USART1_ENABLE_DISABLE,RCC_enuPeriphralEnable);
	NVIC_EnableIRQ(USART1_IRQn);
	GPIO_Init(&USART_pins[TX1]);
	GPIO_Init(&USART_pins[RX1]);
	LED_Init();
	USART_init();
	GPIO_CfgAlternateFn(USART_pins[TX1].GPIO_Port,USART_pins[TX1].GPIO_Pin,GPIO_FUNC_AF7);
	GPIO_CfgAlternateFn(USART_pins[RX1].GPIO_Port,USART_pins[RX1].GPIO_Pin,GPIO_FUNC_AF7);
	uint16_t timeout = 50000;
	while(1)
	{
		USART_recieveBufferAsyncZC(USART_Req2);
		while(timeout)
		{
			timeout--;
		}
		if(*(USART_Req2.data) == "Momen")
		{
			void toggle_Led (void);
		}
	}
	return 0;
}
void toggle_Led (void)
{
	LED_toggle(LED_Alarm);
}


#elif TEST_KEYPAD == 1

int main(int argc, char* argv[])
{
	RCC_EnablePeripheral(RCC_GPIOA);
	RCC_EnablePeripheral(RCC_GPIOB);
	KEYPAD_Init();
	
	GPIO_Pin_t ledPin;
	ledPin.Mode = GPIO_MODE_OP_PP;
	ledPin.AF = GPIO_AF_SYSTEM;
	ledPin.Pin = GPIO_PIN_13;
	ledPin.Port = GPIO_PORT_B;
	ledPin.Speed = GPIO_SPEED_HIGH;
	GPIO_Init(&ledPin);

	// GPIO_Pin_t ledPin1;
	// ledPin1.Mode = GPIO_MODE_OP_PP;
	// ledPin1.AF = GPIO_AF_SYSTEM;
	// ledPin1.Pin = GPIO_PIN_10;
	// ledPin1.Port = GPIO_PORT_B;
	// ledPin1.Speed = GPIO_SPEED_HIGH;
	// GPIO_Init(&ledPin1);

	// GPIO_Pin_t ledPin2;
	// ledPin2.Mode = GPIO_MODE_OP_PP;
	// ledPin2.AF = GPIO_AF_SYSTEM;
	// ledPin2.Pin = GPIO_PIN_9;
	// ledPin2.Port = GPIO_PORT_B;
	// ledPin2.Speed = GPIO_SPEED_HIGH;
	// GPIO_Init(&ledPin2);

	// GPIO_Pin_t ledPin3;
	// ledPin3.Mode = GPIO_MODE_OP_PP;
	// ledPin3.AF = GPIO_AF_SYSTEM;
	// ledPin3.Pin = GPIO_PIN_8;
	// ledPin3.Port = GPIO_PORT_B;
	// ledPin3.Speed = GPIO_SPEED_HIGH;
	// GPIO_Init(&ledPin3);

	// GPIO_Pin_t ledPin4;
	// ledPin4.Mode = GPIO_MODE_OP_PP;
	// ledPin4.AF = GPIO_AF_SYSTEM;
	// ledPin4.Pin = GPIO_PIN_7;
	// ledPin4.Port = GPIO_PORT_B;
	// ledPin4.Speed = GPIO_SPEED_HIGH;
	// GPIO_Init(&ledPin4);

	// GPIO_Pin_t ledPin5;
	// ledPin5.Mode = GPIO_MODE_OP_PP;
	// ledPin5.AF = GPIO_AF_SYSTEM;
	// ledPin5.Pin = GPIO_PIN_6;
	// ledPin5.Port = GPIO_PORT_B;
	// ledPin5.Speed = GPIO_SPEED_HIGH;
	// GPIO_Init(&ledPin5);

	// GPIO_Pin_t ledPin6;
	// ledPin6.Mode = GPIO_MODE_OP_PP;
	// ledPin6.AF = GPIO_AF_SYSTEM;
	// ledPin6.Pin = GPIO_PIN_5;
	// ledPin6.Port = GPIO_PORT_B;
	// ledPin6.Speed = GPIO_SPEED_HIGH;
	// GPIO_Init(&ledPin6);

	// GPIO_SetPinState(GPIO_PORT_B, GPIO_PIN_10, GPIO_STATE_HIGH);
	// GPIO_SetPinState(GPIO_PORT_B, GPIO_PIN_9, GPIO_STATE_HIGH);
	// GPIO_SetPinState(GPIO_PORT_B, GPIO_PIN_8, GPIO_STATE_HIGH);
	// GPIO_SetPinState(GPIO_PORT_B, GPIO_PIN_7, GPIO_STATE_HIGH);
	// GPIO_SetPinState(GPIO_PORT_B, GPIO_PIN_6, GPIO_STATE_HIGH);
	// GPIO_SetPinState(GPIO_PORT_B, GPIO_PIN_5, GPIO_STATE_HIGH);

	LCD_Init();
	SCH_init();
	SCH_Start();
}

#elif TEST_LIN == 1

int main(void)
{

	// RCC_EnableClk(RCC_CLK_HSE);
	// RCC_SetSystemClk(RCC_SYSCLK_HSE);
	RCC_EnablePeripheral(RCC_GPIOA);
    RCC_EnablePeripheral(RCC_GPIOB);
	
    RCC_EnablePeripheral(RCC_USART1);
    RCC_EnablePeripheral(RCC_USART2);
    RCC_EnablePeripheral(RCC_USART6);

	NVIC_EnableIRQ(NVIC_USART1_INTERRUPT);
	NVIC_EnableIRQ(NVIC_USART2_INTERRUPT);

	
	LIN_SlaveInit();
	// LIN_MasterInit();

	KEYPAD_Init();
	LCD_Init();
	
	SCH_init();
	SCH_Start();

}

#endif




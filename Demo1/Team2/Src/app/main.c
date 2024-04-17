#include "GPIO.h"
#include "RCC.h"
#include "NVIC.h"
#include "SYSTICK.h"
#include "LCD_HAL.h"
#include "Keypad.h"
#include "scheduler.h"

#define TEST_LCD 		0
#define TEST_USART		0
#define TEST_KEYPAD		1

#if TEST_LCD == 1
int main(int argc, char* argv[])
{
	RCC_Ctrl_AHB1_Clk(RCC_GPIOA_ENABLE_DISABLE,RCC_enuPeriphralEnable);
	LCD_InitAsync();
	Sched_Init();
	Sched_Start();
}
#elif TEST_USART == 1

#define TX1		0
#define RX1		1	
void toggle_Led (void);
int main(int argc, char* argv[])
{
	GPIO_Pin_t USART_pins[2] = {[TX1] = {.GPIO_Pin = GPIO_PIN_9 , .GPIO_Port = GPIO_PORT_A, .GPIO_Mode = GPIO_MODE_AF_PP, .GPIO_Speed = GPIO_SPEED_HIGH},
								[RX1] = {.GPIO_Pin = GPIO_PIN_10, .GPIO_Port = GPIO_PORT_A, .GPIO_Mode = GPIO_MODE_AF_PP, .GPIO_Speed = GPIO_SPEED_HIGH}};
	u8 data1 ;
	u8 data2[] = "MOMEN";
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
	u16 timeout = 50000;
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
	RCC_enableAHBPeriCLK(RCC_AHB1_GPIOA);
	RCC_enableAHBPeriCLK(RCC_AHB1_GPIOB);
	KEYPAD_Init();
	LCD_GPIO_init();
	SCH_init();
	SCH_Start();
}


#endif




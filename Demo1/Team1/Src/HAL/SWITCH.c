/******************************************************************************
 *
 * Module: Switch
 *
 * File Name: SWITCH.c
 *
 * Description: Source file for the SWITCH driver for STM32F401xC
 *
 * Author: Momen Elsayed Shaban
 *
 *******************************************************************************/
#include "GPIO.h"
#include "SWITCH.h"


extern const SWITCH_Cfg_t Switches[_SWITCH_NUM];

u8 Switch_State[_SWITCH_NUM];

SWITCH_ErrorStatus_t SWITCH_Init()
{
    SWITCH_ErrorStatus_t Error_Status = SWITCH_OK;
    GPIO_Pin_t Switch;
    u8 iterator;
    for(iterator = 0; iterator < _SWITCH_NUM; iterator++)
    {
        if(!(Switches[iterator].dir == SWITCH_PULL_UP || Switches[iterator].dir == SWITCH_PULL_DOWN))
        {
            Error_Status = SWITCH_InvalidDir;
        }
        else
        {
            Switch.GPIO_Pin = Switches[iterator].pin;
            Switch.GPIO_Port = Switches[iterator].port;
            Switch.GPIO_Mode = Switches[iterator].dir;
            GPIO_Init(&Switch);
        }
    }
    return Error_Status;
}

SWITCH_ErrorStatus_t SWITCH_getState(u32 Switch_Name,u8* Switch_state)
{
    
    SWITCH_ErrorStatus_t Error_Status = SWITCH_OK;
    if(Switch_Name >= _SWITCH_NUM)
    {
        Error_Status = SWITCH_InvalidName;
    }
    else if(Switch_state == NULL_PTR)
    {
        Error_Status = SWITCH_NullPtr;
    }
    else{
        /*Shifting by 3 because it is the position of pull-up and pull-down bits in the defined value*/
        *Switch_state = !(Switch_State[Switch_Name] ^ (Switches[Switch_Name].dir >> 4UL));
    }
    return Error_Status;

}

void SW_Runnable(void)
{
	u8 iterator = 0;
	u8 SW_CurrentState;
	static u8 prev[_SWITCH_NUM] = {0};
	static u8 counts[_SWITCH_NUM] = {0};
	for(iterator = 0 ; iterator < _SWITCH_NUM ; iterator++)
	{
		GPIO_getPinValue(Switches[iterator].port, Switches[iterator].pin, &SW_CurrentState);
		if(SW_CurrentState == prev[iterator])
		{
			counts[iterator]++;
		}
		else
		{
			counts[iterator] = 0;
		}
		if(counts[iterator] == 5) /*Scheduler Ticks 10 Ms so we Check for Stable State for 50 MS*/
		{
			Switch_State[iterator] = SW_CurrentState;
			counts[iterator] = 0;
		}
		prev[iterator] = SW_CurrentState;
	}

}

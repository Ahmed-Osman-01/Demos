/*
* SWITCH.c
*
*  Created on: Feb 18, 2024
*      Author: Osman
*/

/* ============================================================================ */
/*                                  INCLUDEDS                           	    */
/* ============================================================================ */

#include "LIB/STD_TYPES.h"
#include "HAL/SWITCH/SWITCH.h"
#include "MCAL/GPIO/GPIO.h"


/* ============================================================================ */
/*                                  MASKS MACROS                           	    */
/* ============================================================================ */
#define SWITCH_PULLUP_OFFSET    0x00000003


/* ============================================================================ */
/*                                  VALIDATION MACROS                          	*/
/* ============================================================================ */

#define IS_SWITCH_CONNECTION(CONNECTION)    ((CONNECTION == SWITCH_CONNECTION_FLOATING) ||\
                                             (CONNECTION == SWITCH_CONNECTION_PULLUP)   ||\
                                             (CONNECTION == SWITCH_CONNECTION_PULLDOWN))

/* ============================================================================ */
/*                                  	EXTERNS                           	    */
/* ============================================================================ */

extern const SWITCH_Config_t Switches[_SWITCH_NUM];


/* ============================================================================ */
/*                               GLOBAL VARIABLES                         	    */
/* ============================================================================ */

static u8 G_States[_SWITCH_NUM];        /* Actual states of the switches after sampling */

/* ============================================================================ */
/*                                  IMPLEMENTATION                           	*/
/* ============================================================================ */

SWITCH_ErrorStatus_t SWITCH_Init(void)
{
   SWITCH_ErrorStatus_t Ret_ErrorStatus = SWITCH_OK;

   GPIO_Pin_t Local_Switch;
   u8 Local_Iter;
   Local_Switch.Speed = GPIO_SPEED_HIGH;
   Local_Switch.AF = GPIO_AF_SYSTEM;

   for(Local_Iter = 0; Local_Iter < _SWITCH_NUM; Local_Iter++)
   {

        if(!IS_SWITCH_CONNECTION(Switches[Local_Iter].Connection))
        {
            Ret_ErrorStatus = SWITCH_INVALID_CONNECTION;
        }
        else
        {
            Local_Switch.Port = Switches[Local_Iter].Port;
            Local_Switch.Pin = Switches[Local_Iter].Pin;
            Local_Switch.Mode = Switches[Local_Iter].Connection;
            Ret_ErrorStatus = GPIO_Init(&Local_Switch);
        }

        if(Ret_ErrorStatus != SWITCH_OK)
        {
            break;
        }

   }

   return Ret_ErrorStatus;
}

/* Updates the state of the switches by sampling the hardware state and if the hw state is stable for 5
consecutive samples then the switch state is updated.
This is a Runnable that is called by Schedular                                                  */
void SWITCH_Update(void)
{
    static u8 prevHWState[_SWITCH_NUM];     /* Prev state for each switch */
    static u8 stateCount[_SWITCH_NUM];      /* Count number of consecutive similar states for each switch*/
    u8 currHWState;                         /* Latest reading from HW */
    u8 currSwitch;                          /* Iterator for switches */

    for(currSwitch = 0; currSwitch < _SWITCH_NUM; currSwitch++)
    {
        GPIO_GetPinState(Switches[currSwitch].Port, Switches[currSwitch].Pin, &currHWState);

        /* This line to return Either Pressed Or Released Regardless of the Connection
            If The connection is PullUp then inverse the returned state from GPIO to match the
            actuall state (Pressed or Released) */
        currHWState =  currHWState ^ ( (Switches[currSwitch].Connection & SWITCH_CONNECTION_PULLUP) >> SWITCH_PULLUP_OFFSET);

        if(currHWState == prevHWState[currSwitch])
        {
            stateCount[currSwitch]++;
        }
        else
        {
            stateCount[currSwitch] = 0;
        }

        /* Checking for 4 instead of 5 here because the transition from
          pressed to not pressed or vice versa is not counted so
          this transition step + 4 = 5 consecutive similar states*/
        if(stateCount[currSwitch] == 4)
        {
            /* If the reading was stable for 5 consecutive readings then update the state of this switch */
            G_States[currSwitch] = currHWState;
            stateCount[currSwitch] = 0;
        }

        prevHWState[currSwitch] = currHWState;

    }
}

SWITCH_ErrorStatus_t SWITCH_GetSwitchState(SWITCH_ID_t Copy_SwitchID, u8 *Add_SwitchState)
{
    SWITCH_ErrorStatus_t Ret_ErrorStatus = SWITCH_OK;
    if(Copy_SwitchID >= _SWITCH_NUM)
    {
        Ret_ErrorStatus = SWITCH_INVALID_SWITCH_ID;
    }
    else if(Add_SwitchState == NULL)
    {
        Ret_ErrorStatus = SWITCH_NULLPTR;
    }
    else
    {
        /* do nothing */
    }

    if(Ret_ErrorStatus == SWITCH_OK)
    {
       *Add_SwitchState = G_States[Copy_SwitchID];
    }

    return Ret_ErrorStatus;
}

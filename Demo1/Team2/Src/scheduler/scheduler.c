/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "scheduler.h"
#include "SYSTICK.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

#ifndef NULL
#define NULL ((void*)0)
#endif

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

typedef struct{
    Runnable_USER_t Runnable_USER;
    uint32_t RemainingTime;
}Runnable_System_t;

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
extern const Runnable_USER_t Runnable_User [_RUNNABLE_NO];
volatile uint8_t PendingTicks = 0;
volatile Runnable_System_t Runnable_System [_RUNNABLE_NO] ;
/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/

static void TickCB(void)
{
    PendingTicks++;
}

static void Sched (void)
{
    int index = 0;

    for(index = 0; index <_RUNNABLE_NO; index++)
    {
        if(Runnable_System[index].Runnable_USER.CB && Runnable_System[index].RemainingTime == 0)
        {
            Runnable_System[index].Runnable_USER.CB();
            Runnable_System[index].RemainingTime = Runnable_User[index].Periodicity;
        }
        Runnable_System[index].RemainingTime -= TICKTIME ;
    }

}
/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

void SCH_init(void)
{
    int index = 0;
    SYSTICK_CLKSOURCE(AHBCLK);
    SYSTICK_SetCallBack(&TickCB);
    SYSTICK_SetTimeMS(TICKTIME);

    for(index = 0; index <_RUNNABLE_NO;index++)
    {
        if(Runnable_User[index].CB &&  Runnable_System[index].Runnable_USER.CB == NULL)
        {
            Runnable_System[index].Runnable_USER = Runnable_User[index];
            Runnable_System[index].RemainingTime = Runnable_User[index].First_Delay ;
        }
    }
}

void SCH_Start(void)
{
    SYSTICK_Start();
    while(1)
    {
        if(PendingTicks)
        {
            PendingTicks--;
            Sched();
        }
    }
}

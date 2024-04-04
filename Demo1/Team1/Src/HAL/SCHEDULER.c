/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "HAL/SCHEDULER.h"
#include "MCAL/SYSTICK_DRIVER.h"
#include "Run_List.h"
#include "SYSTICK_cfg.h"

/********************************************************************************************************/
/**************************************************Types*************************************************/
/********************************************************************************************************/
typedef struct
{
    Runnable_t *Runnable;
    u32_t Remining_time;
} Run_Info_t;

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
static volatile u32_t PendingTicks;
extern const Runnable_t Run_List[_Run_Num];
Run_Info_t Run_Info_List[_Run_Num];

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/
void TickCB(void);
static void SCHD(void);

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

Error_Status SCH_CTRL_StartScheduler()
{
    Error_Status LOC_Status = Status_NOK;
    LOC_Status = SYSTICK_CTRL_StartTimer();

    while (1)
    {
        if (PendingTicks)
        {
            PendingTicks--;
            SCHD();
        }
    }
    return LOC_Status;
}

Error_Status SCH_CFG_SchedulerInit()
{
    Error_Status LOC_Status = Status_NOK;
    u8_t Index;

    LOC_Status = SYSTICK_CTRL_Interrupt(SYSTICK_IRQ_ENABLE);
    LOC_Status = SYSTICK_SET_CurrentVal(0);
    LOC_Status = SYSTICK_SET_CallBack(TickCB);
    LOC_Status = SYSTICK_CFG_CLKSource(SYSTICK_CLK_AHB);
    LOC_Status = SYSTICK_SET_TimeTicksMs(TICK_TIME_MS);

    for (Index = 0; Index < _Run_Num; Index++)
    {
        Run_Info_List[Index].Runnable = &Run_List[Index];
        Run_Info_List[Index].Remining_time = Run_List[Index].delay_Ms;
    }

    return LOC_Status;
}

static void SCHD(void)
{
    u32_t Index;

    for (Index = 0; Index < _Run_Num; Index++)
    {
        if ((Run_Info_List[Index].Runnable->CB) && ((Run_Info_List[Index].Remining_time) == 0))
        {
            Run_Info_List[Index].Remining_time = Run_Info_List[Index].Runnable->periodMs;
            Run_Info_List[Index].Runnable->CB();
        }
        Run_Info_List[Index].Remining_time -= TICK_TIME_MS;
    }
}

void TickCB(void)
{
    PendingTicks++;
}
/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "SYSTICK.h"


/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define SYSTICK ((volatile SYSTICK_t* const) 0xE000E010)

#define SYSTICK_COUNT_ENABLE 0x01
#define SYSTICK_INT_ENABLE 0x02

#ifndef NULL
#define NULL ((void *)0)
#endif
/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
typedef struct {
    uint32_t STK_CTRL;
    uint32_t STK_LOAD;
    uint32_t STK_VAL;
    uint32_t STK_CALIB;
} SYSTICK_t;


/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
static SYSTICK_CBF_t CBF = NULL;
volatile uint32_t GLOBAL_CLK_RESULT = 0;
volatile uint32_t COUNT = 0;
volatile uint32_t LASTLOADVAL = 0;
volatile uint64_t LOADVAL = 0;
/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/


/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

SYSTICK_errorStatus_t SYSTICK_SetTimeMS(uint32_t Time)
{
    SYSTICK_errorStatus_t LOC_errorStatus = SYSTICK_isNotOk;
    volatile uint32_t LOC_Clk_Source= 0;
    if(Time <= 0)
    {
        LOC_errorStatus = SYSTICK_WRONGCOUNTS;
    }
    else
    {
        LOC_errorStatus = SYSTICK_isOk;
        if(GLOBAL_CLK_RESULT)
        {
            LOC_Clk_Source = MC_FREQ;
        }
        else
        {
            LOC_Clk_Source = MC_FREQ / 8;
        }
        LOADVAL = (((uint64_t)Time * (uint64_t)LOC_Clk_Source)/1000) -1;

        if(LOADVAL > 0xFFFFFF)
        {
            COUNT = LOADVAL/ 0xFFFFFF;
            LASTLOADVAL = LOADVAL % 0xFFFFFF;
            LOADVAL = 0xFFFFFF;
            
        }

        SYSTICK->STK_LOAD = (uint32_t)LOADVAL;
    }

    return LOC_errorStatus;
}
void SYSTICK_Start()
{
    uint32_t temp = 0;
    SYSTICK->STK_VAL = 0;
    temp = SYSTICK->STK_CTRL;
    temp |= SYSTICK_COUNT_ENABLE;
    temp |= SYSTICK_INT_ENABLE;
    SYSTICK->STK_CTRL = temp;
}
SYSTICK_errorStatus_t SYSTICK_SetCallBack(SYSTICK_CBF_t Function)
{
    SYSTICK_errorStatus_t LOC_errorStatus = SYSTICK_isNotOk;
    if(Function)
    {
        LOC_errorStatus = SYSTICK_isOk;
        CBF = Function;
    }
    else
    {
        LOC_errorStatus = SYSTICK_NULLPTR;
    }

    return LOC_errorStatus;
}   

SYSTICK_errorStatus_t SYSTICK_CLKSOURCE(uint8_t CLKSOURCE)
{
    SYSTICK_errorStatus_t LOC_errorStatus = SYSTICK_isNotOk;    
    if(CLKSOURCE < 0 || CLKSOURCE > 1)
    {
        LOC_errorStatus = SYSTICK_WrongCLKSOURCE;
    }
    else
    {
        LOC_errorStatus = SYSTICK_isOk; 
        SYSTICK->STK_CTRL |= (CLKSOURCE << 2);
        GLOBAL_CLK_RESULT = CLKSOURCE;
    }
    return LOC_errorStatus;
}

void SysTick_Handler(void)
{
    static uint32_t count = 0;
    count++;
    if(CBF && count< COUNT)
    {
        //do nothing
    }
    else if(CBF && count == COUNT)
    {
        SYSTICK->STK_LOAD = LASTLOADVAL;
    }
    else if(CBF && count == COUNT+1)
    {
        CBF();
        count = 0;
        SYSTICK->STK_LOAD = LOADVAL;
    }

}
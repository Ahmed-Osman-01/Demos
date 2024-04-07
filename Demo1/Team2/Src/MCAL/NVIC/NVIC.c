/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include"NVIC.h"


/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define NVIC ((volatile NVIC_t* const ) 0xE000E100)
#define SCB ((volatile SCB_t* const  ) 0xE000E008)
#define SET_BIT   1
#define RESET_BIT 0
#define REGISTER_WIDTH 32
#define REGISTER_GET 5
#define PRIORITY_LEVELS_MASK 0xFFFF0700
#define REGISTER_IPR_INT_WIDTH 4
#define REGISTER_IPR_INT_GET   2
#define PRIORIOTY_MASK 0xFF
#define PRIORITY_SHIFT_VALUE   8
#define PRIORITY_SHIFT_OFFSET  1
#define PRIORITY_NO_GROUP      8
#define PRIORITY_NO_SUBGROUP   4
#define REGISTER_IPR_PRIORITY_SECTION 8

#ifndef NULL
#define NULL ((void *)0)
#endif

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

typedef struct{

    uint32_t NVIC_ISER[8];
    uint32_t RESERVED_1[24];
    uint32_t NVIC_ICER[8];
    uint32_t RESERVED_2[24];
    uint32_t NVIC_ISPR[8];
	uint32_t RESERVED_3[24];
	uint32_t NVIC_ICPR[8];
	uint32_t RESERVED_4[24];
	uint32_t NVIC_IABR[8];
	uint32_t RESERVED_5[56];
	uint32_t NVIC_IPR[59];
	uint32_t RESERVED_6[644];
	uint32_t NVIC_STIR;
}NVIC_t;

typedef struct
{
    volatile uint32_t ACTLR;
    volatile uint32_t Reserved1[829];
    volatile uint32_t CPUID;
    volatile uint32_t ICSR;
    volatile uint32_t VTOR;
    volatile uint32_t AIRCR;
    volatile uint32_t SCR;
    volatile uint32_t CCR;
    volatile uint32_t SHPR[3];
    volatile uint32_t SHCSR;
    volatile uint32_t CFSR;
    volatile uint32_t HFSR;
    volatile uint32_t Reserved2;
    volatile uint32_t MMAR;
    volatile uint32_t BFAR;
    volatile uint32_t AFSR;

} SCB_t;
/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
uint32_t GLOBAL_Priority_Level_SHIFT = 0;


/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

NVIC_errorStatus_t NVIC_EnableIRQ(uint32_t IRQn)
{
    NVIC_errorStatus_t LOC_errorStatus = NVIC_isOk;
    uint32_t Bit_NO=0;
    uint32_t Reg_Shift=0;

    if(IRQn < NVIC_WINDOW_WATCHDOG_TIMER || IRQn > NVIC_SPI4_INTERRUPT)
    {
        LOC_errorStatus = NVIC_WRONGIRQ;
    }
    else
    {
        LOC_errorStatus = NVIC_isOk;
        Bit_NO = IRQn % REGISTER_WIDTH;
        Reg_Shift = IRQn >> REGISTER_GET;
        NVIC->NVIC_ISER[Reg_Shift] |= SET_BIT << Bit_NO;
    }
    return LOC_errorStatus;
}

NVIC_errorStatus_t NVIC_DisableIRQ(uint32_t IRQn)
{
    NVIC_errorStatus_t LOC_errorStatus = NVIC_isOk;
    uint32_t Bit_NO=0;
    uint32_t Reg_Shift=0;

    if(IRQn < NVIC_WINDOW_WATCHDOG_TIMER || IRQn > NVIC_SPI4_INTERRUPT)
    {
        LOC_errorStatus = NVIC_WRONGIRQ;
    }
    else
    {
        LOC_errorStatus = NVIC_isOk;
        Bit_NO = IRQn % REGISTER_WIDTH;
        Reg_Shift = IRQn >> REGISTER_GET;
        NVIC->NVIC_ICER[Reg_Shift] = SET_BIT << Bit_NO;
    }
    return LOC_errorStatus;    
}

NVIC_errorStatus_t NVIC_SetPendingIRQ (uint32_t IRQn)
{
    NVIC_errorStatus_t LOC_errorStatus = NVIC_isOk;
    uint32_t Bit_NO=0;
    uint32_t Reg_Shift=0;

    if(IRQn < NVIC_WINDOW_WATCHDOG_TIMER || IRQn > NVIC_SPI4_INTERRUPT)
    {
        LOC_errorStatus = NVIC_WRONGIRQ;
    }
    else
    {
        LOC_errorStatus = NVIC_isOk;
        Bit_NO = IRQn % REGISTER_WIDTH;
        Reg_Shift = IRQn >> REGISTER_GET;
        NVIC->NVIC_ISPR[Reg_Shift] |= SET_BIT << Bit_NO;
    }
    return LOC_errorStatus;        
}

NVIC_errorStatus_t NVIC_ClearPendingIRQ (uint32_t IRQn)
{
    NVIC_errorStatus_t LOC_errorStatus = NVIC_isOk;
    uint32_t Bit_NO=0;
    uint32_t Reg_Shift=0;

    if(IRQn < NVIC_WINDOW_WATCHDOG_TIMER || IRQn > NVIC_SPI4_INTERRUPT)
    {
        LOC_errorStatus = NVIC_WRONGIRQ;
    }
    else
    {
        LOC_errorStatus = NVIC_isOk;
        Bit_NO = IRQn % REGISTER_WIDTH;
        Reg_Shift = IRQn >> REGISTER_GET;
        NVIC->NVIC_ICPR[Reg_Shift] = SET_BIT << Bit_NO;
    }
    return LOC_errorStatus;       
}

NVIC_errorStatus_t NVIC_GetPendingIRQ (uint32_t IRQn, uint32_t* pendingState)
{
    NVIC_errorStatus_t LOC_errorStatus = NVIC_isOk;
    uint32_t Bit_NO=0;
    uint32_t Reg_Shift=0;

    if(IRQn < NVIC_WINDOW_WATCHDOG_TIMER || IRQn > NVIC_SPI4_INTERRUPT)
    {
        LOC_errorStatus = NVIC_WRONGIRQ;
    }
    else if(pendingState == NULL)
    {
        LOC_errorStatus = NVIC_NULLPTR;
    }    
    else
    {
        LOC_errorStatus = NVIC_isOk;
        Bit_NO = IRQn % REGISTER_WIDTH;
        Reg_Shift = IRQn >> REGISTER_GET;
        *pendingState = ((NVIC->NVIC_ISPR[Reg_Shift] >> Bit_NO) & SET_BIT);      
    }
    return LOC_errorStatus;
}

NVIC_errorStatus_t NVIC_GetActive (uint32_t IRQn, uint32_t* activeState)
{
    NVIC_errorStatus_t LOC_errorStatus = NVIC_isOk;
    uint32_t Bit_NO=0;
    uint32_t Reg_Shift=0;

    if(IRQn < NVIC_WINDOW_WATCHDOG_TIMER || IRQn > NVIC_SPI4_INTERRUPT)
    {
        LOC_errorStatus = NVIC_WRONGIRQ;
    }
    else if(activeState == NULL)
    {
        LOC_errorStatus = NVIC_NULLPTR;
    }    
    else
    {
        LOC_errorStatus = NVIC_isOk;
        Bit_NO = IRQn % REGISTER_WIDTH;
        Reg_Shift = IRQn >> REGISTER_GET;
        *activeState = ((NVIC->NVIC_IABR[Reg_Shift] >> Bit_NO) & SET_BIT);      
    }
    return LOC_errorStatus;    
}

NVIC_errorStatus_t NVIC_SetPriority (uint32_t IRQn, uint32_t Group, uint32_t SubGroup)
{
    NVIC_errorStatus_t LOC_errorStatus = NVIC_isOk;
    uint32_t Bit_NO=0;
    uint32_t Reg_Shift=0;
    uint32_t LOC_temp = 0;
    uint32_t LOC_shift = 0;
    if(IRQn < NVIC_WINDOW_WATCHDOG_TIMER || IRQn > NVIC_SPI4_INTERRUPT)
    {
        LOC_errorStatus = NVIC_WRONGIRQ;
    }
    else if(Group < GROUP_0 || Group> GROUP_15)
    {
        LOC_errorStatus = NVIC_WrongGroup;
    }
    else if(Group < SUBGROUP_0 || Group> SUBGROUP_15)
    {
        LOC_errorStatus = NVIC_WrongSubGroup;
    }
    else
    {
        LOC_errorStatus = NVIC_isOk;
        Bit_NO = IRQn % REGISTER_IPR_INT_WIDTH;
        Reg_Shift = IRQn >> REGISTER_IPR_INT_GET;
        LOC_shift = Bit_NO * REGISTER_IPR_PRIORITY_SECTION;
        LOC_temp = NVIC->NVIC_IPR[Reg_Shift];  
        if(GLOBAL_Priority_Level_SHIFT == PRIORITY_NO_GROUP || GLOBAL_Priority_Level_SHIFT == PRIORITY_NO_GROUP)
        {

        }
        else
        {
            LOC_temp &=~(PRIORIOTY_MASK << LOC_shift);
            if(GLOBAL_Priority_Level_SHIFT != PRIORITY_NO_GROUP)
            {

                LOC_temp |= Group << GLOBAL_Priority_Level_SHIFT << LOC_shift;        
            }
            else
            {
            //Do nothing
            }
            
            if(GLOBAL_Priority_Level_SHIFT != PRIORITY_NO_GROUP)
            {
                //LOC_temp &=~(PRIORIOTY_MASK << LOC_shift);
                LOC_temp |= SubGroup << (REGISTER_IPR_PRIORITY_SECTION - GLOBAL_Priority_Level_SHIFT) << LOC_shift;
            }
            else
            {
                //Do nothing
            }
            NVIC->NVIC_IPR[Reg_Shift] = LOC_temp;
        }

    }
    return LOC_errorStatus;
}


NVIC_errorStatus_t NVIC_SetPriorityGrouping(uint32_t priority_grouping)
{
    NVIC_errorStatus_t LOC_errorStatus = NVIC_isOk;
    volatile uint32_t LOC_temp = 0;
    if((priority_grouping != PRIORITY_16_LEVELS) &&
       (priority_grouping != PRIORITY_8_LEVELS)  &&
       (priority_grouping != PRIORITY_4_LEVELS)  &&
       (priority_grouping != PRIORITY_2_LEVELS)  &&
       (priority_grouping != PRIORITY_NO_LEVELS)  )
    {
        LOC_errorStatus = NVIC_WrongPriorityGrouping;
    } 
    else
    {
        LOC_errorStatus = NVIC_isOk;
        LOC_temp = SCB->AIRCR;
        LOC_temp &= ~ PRIORITY_LEVELS_MASK;
        LOC_temp |= priority_grouping;
        LOC_temp = (LOC_temp | 0x05FA0000);
        SCB->AIRCR = LOC_temp;

        // set the global priority LEVEL
        GLOBAL_Priority_Level_SHIFT = ((priority_grouping >> PRIORITY_SHIFT_VALUE) + PRIORITY_SHIFT_OFFSET);
    }

    return LOC_errorStatus;
}


NVIC_errorStatus_t NVIC_GenerateSWINT(uint32_t IRQn)
{
    NVIC_errorStatus_t LOC_errorStatus = NVIC_isOk;

    if(IRQn < NVIC_WINDOW_WATCHDOG_TIMER || IRQn > NVIC_SPI4_INTERRUPT)
    {
        LOC_errorStatus = NVIC_WRONGIRQ;
    }    
    else
    {
        LOC_errorStatus = NVIC_isOk;
        NVIC->NVIC_STIR = IRQn;
    }
    return LOC_errorStatus;
}
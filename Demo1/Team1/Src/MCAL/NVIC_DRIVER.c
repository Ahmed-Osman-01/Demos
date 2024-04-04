/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "MCAL/NVIC_DRIVER.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define NVIC_BASE_ADDRESS 0xE000E100
#define SCB_BASE_ADDRESS 0xE000E008

#define NVIC_REG_THRESHOLD 32
#define NVIC_PRI_REG_THRESHOLD 4
#define SUBGROUPBIT_CLEARFLAG 0xFFFF0700
#define SUBGROUPBIT_GETFLAG 0x03
#define PRIORITY_CLEARFLAG 0x000000FF
#define PRIORITY_GETFLAG 0x000000FF
#define TOTAL_PRIORITY_BITS 8
#define SOC_AIRCR_WRITEENABLE 0x05FA0000

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
typedef struct
{
    volatile u32_t ISER[8];
    volatile u32_t Reserved1[24];
    volatile u32_t ICER[8];
    volatile u32_t Reserved2[24];
    volatile u32_t ISPR[8];
    volatile u32_t Reserved3[24];
    volatile u32_t ICPR[8];
    volatile u32_t Reserved4[24];
    volatile u32_t IABR[8];
    volatile u32_t Reserved5[56];
    volatile u32_t IPR[60];
    volatile u32_t Reserved6[644];
    volatile u32_t STIR;
} NVIC_t;

typedef struct
{
    volatile u32_t ACTLR;
    volatile u32_t Reserved1[829];
    volatile u32_t CPUID;
    volatile u32_t ICSR;
    volatile u32_t VTOR;
    volatile u32_t AIRCR;
    volatile u32_t SCR;
    volatile u32_t CCR;
    volatile u32_t SHPR[3];
    volatile u32_t SHCSR;
    volatile u32_t CFSR;
    volatile u32_t HFSR;
    volatile u32_t Reserved2;
    volatile u32_t MMAR;
    volatile u32_t BFAR;
    volatile u32_t AFSR;

} SCB_t;

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/

NVIC_t *const NVIC = (NVIC_t *)NVIC_BASE_ADDRESS;
SCB_t *const SCB = (SCB_t *)SCB_BASE_ADDRESS;

static u8_t NVIC_SubGroupBits = 0;
/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/

static u8_t power(u8_t num, u8_t pow);

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

Error_Status NVIC_CTRL_EnableIRQ(IRQ_ID_t NVIC_IRQ)
{
    Error_Status LOC_Status = Status_NOK;
    u32_t REG_Index = NVIC_IRQ / NVIC_REG_THRESHOLD;

    if (NVIC_IRQ >= _NVIC_IRQ_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        NVIC->ISER[REG_Index] = (1 << (NVIC_IRQ % NVIC_REG_THRESHOLD));
    }
    return LOC_Status;
}

Error_Status NVIC_CTRL_DisableIRQ(IRQ_ID_t NVIC_IRQ)
{
    Error_Status LOC_Status = Status_NOK;
    u32_t REG_Index = NVIC_IRQ / NVIC_REG_THRESHOLD;

    if (NVIC_IRQ >= _NVIC_IRQ_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        NVIC->ICER[REG_Index] = (1 << (NVIC_IRQ % NVIC_REG_THRESHOLD));
    }
    return LOC_Status;
}

Error_Status NVIC_CTRL_SetIRQPending(IRQ_ID_t NVIC_IRQ)
{
    Error_Status LOC_Status = Status_NOK;
    u32_t REG_Index = NVIC_IRQ / NVIC_REG_THRESHOLD;

    if (NVIC_IRQ >= _NVIC_IRQ_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        NVIC->ISPR[REG_Index] = (1 << (NVIC_IRQ % NVIC_REG_THRESHOLD));
    }
    return LOC_Status;
}

Error_Status NVIC_CTRL_ClearIRQPending(IRQ_ID_t NVIC_IRQ)
{
    Error_Status LOC_Status = Status_NOK;
    u32_t REG_Index = NVIC_IRQ / NVIC_REG_THRESHOLD;

    if (NVIC_IRQ >= _NVIC_IRQ_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        NVIC->ICPR[REG_Index] = (1 << (NVIC_IRQ % NVIC_REG_THRESHOLD));
    }
    return LOC_Status;
}

Error_Status NVIC_CTRL_GenerateSWI(IRQ_ID_t NVIC_IRQ)
{
    Error_Status LOC_Status = Status_NOK;

    if (NVIC_IRQ >= _NVIC_IRQ_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        // SCB->CCR |= 2;  need to be tested
        NVIC->STIR = NVIC_IRQ;
    }
    return LOC_Status;
}

Error_Status NVIC_Get_ActiveStatus(IRQ_ID_t NVIC_IRQ, u32_t *Status)
{
    Error_Status LOC_Status = Status_NOK;
    u32_t REG_Index = NVIC_IRQ / NVIC_REG_THRESHOLD;
    u32_t BIT_Index = NVIC_IRQ % NVIC_REG_THRESHOLD;

    if (NVIC_IRQ >= _NVIC_IRQ_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else if (Status == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else
    {
        LOC_Status = Status_OK;
        *Status = (NVIC->IABR[REG_Index] >> BIT_Index) & 1;
    }
    return LOC_Status;
}

Error_Status NVIC_CFG_SetPriority(IRQ_ID_t NVIC_IRQ, u8_t PreemptPri, u8_t SubGroupPri)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t REG_Index = NVIC_IRQ / NVIC_PRI_REG_THRESHOLD;
    u8_t Tot_Pri_Index = (NVIC_IRQ % NVIC_PRI_REG_THRESHOLD) * TOTAL_PRIORITY_BITS;
    u8_t Imp_Pri_Index = Tot_Pri_Index + (TOTAL_PRIORITY_BITS - IMPLEMENTED_PRIORITY_BITS);
    u32_t LOC_TempPRiority = NVIC->IPR[REG_Index];

    if (NVIC_IRQ >= _NVIC_IRQ_NUM || NVIC_SubGroupBits > IMPLEMENTED_PRIORITY_BITS)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else if (SubGroupPri >= power(2, NVIC_SubGroupBits) || PreemptPri >= power(2, IMPLEMENTED_PRIORITY_BITS - NVIC_SubGroupBits))
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        LOC_TempPRiority &= ~(PRIORITY_CLEARFLAG << Tot_Pri_Index);
        LOC_TempPRiority |= ((PreemptPri << (Imp_Pri_Index + NVIC_SubGroupBits)) | (SubGroupPri << Imp_Pri_Index)) << Tot_Pri_Index;
        NVIC->IPR[REG_Index] = LOC_TempPRiority;
    }
    return LOC_Status;
}

Error_Status NVIC_Get_Priority(IRQ_ID_t NVIC_IRQ, u32_t *Priority)
{
    Error_Status LOC_Status = Status_NOK;
    u32_t REG_Index = NVIC_IRQ / NVIC_REG_THRESHOLD;
    u32_t Tot_Pri_Index = (NVIC_IRQ % NVIC_PRI_REG_THRESHOLD) * NVIC_PRI_REG_THRESHOLD;

    if (NVIC_IRQ >= _NVIC_IRQ_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else if (Priority == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else
    {
        LOC_Status = Status_OK;
        *Priority = (NVIC->IPR[REG_Index] >> Tot_Pri_Index) & PRIORITY_GETFLAG;
    }
    return LOC_Status;
}

Error_Status NVIC_CFG_SetSubGroupBits(u32_t SubGroupBit)
{
    Error_Status LOC_Status = Status_NOK;
    u32_t LOC_AIRCR = SCB->AIRCR;

    if (SubGroupBit > SUBGROUPBIT_FOUR)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        NVIC_SubGroupBits = (SubGroupBit >> SUBGROUPBIT_GETFLAG) ? ((SubGroupBit & SUBGROUPBIT_GETFLAG) + 1) : 0;
        LOC_Status = Status_OK;
        LOC_AIRCR &= ~SUBGROUPBIT_CLEARFLAG;
        LOC_AIRCR |= SubGroupBit | SOC_AIRCR_WRITEENABLE;
        SCB->AIRCR = LOC_AIRCR;
    }
    return LOC_Status;
}

static u8_t power(u8_t num, u8_t pow)
{
    u8_t result = 1;
    u8_t i;

    for (i = 0; i < pow; i++)
    {
        result = result * num;
    }

    return result;
}
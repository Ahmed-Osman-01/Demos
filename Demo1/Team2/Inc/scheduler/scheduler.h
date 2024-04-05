#ifndef SCHEDULER_INCLUDE_SCHEDULER_H_
#define SCHEDULER_INCLUDE_SCHEDULER_H_


/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include <stdint.h>
#include "scheduler_CFG.h"
/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/




/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

typedef void (*Runnable)(void);

typedef struct{
    char* Name;
    uint32_t Priority;
    uint32_t Periodicity;
    Runnable CB;
    uint32_t First_Delay;
}Runnable_USER_t;

typedef enum{
    SCH_isOk,
    SCH_isNotOk,
    SCH_NULLPTR
}SCH_errorStatus_t;

/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

void SCH_init(void);
void SCH_Start(void);

#endif // SCHEDULER_INCLUDE_SCHEDULER_H_
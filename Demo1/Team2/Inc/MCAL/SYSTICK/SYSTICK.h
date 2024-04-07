#ifndef DRIVER_INCLUDE_SYSTICK_H_
#define DRIVER_INCLUDE_SYSTICK_H_

#include <stdint.h>
#include <SYSTICK_CFG.h>

#define AHBDIV8CLK 0
#define AHBCLK     1

typedef void(*SYSTICK_CBF_t)(void);

typedef enum{
    SYSTICK_WRONGCOUNTS,
    SYSTICK_isOk,
    SYSTICK_isNotOk,
    SYSTICK_WrongCLKSOURCE,
    SYSTICK_NULLPTR
}SYSTICK_errorStatus_t;

SYSTICK_errorStatus_t SYSTICK_SetTimeMS(uint32_t Time);
void SYSTICK_Start();
SYSTICK_errorStatus_t SYSTICK_SetCallBack(SYSTICK_CBF_t Function);
SYSTICK_errorStatus_t SYSTICK_CLKSOURCE(uint8_t CLKSOURCE);

#endif // DRIVER_INCLUDE_SYSTICK_H_
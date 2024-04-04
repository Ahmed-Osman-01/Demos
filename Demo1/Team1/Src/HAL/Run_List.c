
#include "Run_List.h"
#include "HAL/SCHEDULER.h"
#include "HAL/Runnables_Prototypes.h"

const Runnable_t Run_List[_Run_Num] =
    {
        [LCD_RUN] = {
            .name = "LCD runnable",
            .periodMs = 2,
            .delay_Ms = 30,
            .CB = LCD_Runnable},
        [LCD_W] = {.name = "LCD Write", .periodMs = 1000, .delay_Ms = 500, .CB = LCD_Write}};
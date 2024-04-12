#include "scheduler.h"

// extern Runnable Runnable_first;
// extern Runnable Runnable_second;
// extern Runnable Runnable_third;
// extern Runnable SW_runnable;
// extern Runnable Runnable_app;
extern Runnable Runnable_app;
extern Runnable LCD_TASK;
extern Runnable Dummy_runnable;
const Runnable_USER_t Runnable_User [_RUNNABLE_NO] =
{
    [Runnable_0] = {
        .Name = "my_first",
        .Priority = 0,
        .Periodicity = 1000,
        .CB = (Runnable)&Dummy_runnable,
        .First_Delay = 200
    },
    [Runnable_1] = {
        .Name = "LCD_TASK",
        .Priority = 0,
        .Periodicity = 1,
        .CB = (Runnable)&LCD_TASK,
        .First_Delay = 0
    }
};
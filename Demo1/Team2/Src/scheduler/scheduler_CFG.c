#include "scheduler/scheduler.h"

// extern Runnable Runnable_first;
// extern Runnable Runnable_second;
// extern Runnable Runnable_third;
// extern Runnable SW_runnable;
// extern Runnable Runnable_app;
extern Runnable DateTime;
extern Runnable displayControl;
extern Runnable stopWatch;
extern Runnable LCD_Update;
extern Runnable Keypad_Runnable;
extern Runnable readSwitches;
const Runnable_USER_t Runnable_User [_RUNNABLE_NO] =
{
    [Runnable_0] = {
        .Name = "Keypad_Runnable",
        .Priority = 0,
        .Periodicity = 6,
        .CB = (Runnable)&Keypad_Runnable,
        .First_Delay = 0
    },
    [Runnable_1] = {
        .Name = "readSwitches",
        .Priority = 0,
        .Periodicity = 30,
        .CB = (Runnable)&readSwitches,
        .First_Delay = 30
    },
    [Runnable_2] = {
        .Name = "LCD_TASK",
        .Priority = 0,
        .Periodicity = 1,
        .CB = (Runnable)&LCD_Update,
        .First_Delay = 0
    },
    [Runnable_3] = {
        .Name = "DateTime",
        .Priority = 0,
        .Periodicity = 100,
        .CB = (Runnable)&DateTime,
        .First_Delay = 200
    },

    [Runnable_4] = {
        .Name = "StopWatch",
        .Priority = 0,
        .Periodicity = 100,
        .CB = (Runnable)&stopWatch,
        .First_Delay = 200
    },
    [Runnable_5] = {
        .Name = "displayControl",
        .Priority = 0,
        .Periodicity = 100,
        .CB = (Runnable)&displayControl,
        .First_Delay = 200
    },
};
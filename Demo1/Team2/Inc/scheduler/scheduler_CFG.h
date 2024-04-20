#ifndef SCHEDULER_INCLUDE_SCHEDULER_CFG_H_
#define SCHEDULER_INCLUDE_SCHEDULER_CFG_H_

typedef enum{
    Runnable_0,
    Runnable_1,
    Runnable_2,
    Runnable_3,
    Runnable_4,
    Runnable_5,
    // Runnable_6, // MASTER TASK
    Runnable_7, // Transfere
    _RUNNABLE_NO
}Runnable_item_t;

#define TICKTIME 2


#endif // SCHEDULER_INCLUDE_SCHEDULER_CFG_H_
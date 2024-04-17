
#ifndef LIN_MASTER_CONFIG_H_
#define LIN_MASTER_CONFIG_H_

#define SCHED_TABLE_MESSAGES_NUM 2

typedef enum
{
    First_MSG,
    Second_MSG,
    _MASTER_MSG_NUM
} Master_MSG_t;

#define MASTER_CALL_PERIOD 100

#endif

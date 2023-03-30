#ifndef INC_LOGICAL_SCHEDULER_SCHEDULERTASK_H_
#define INC_LOGICAL_SCHEDULER_SCHEDULERTASK_H_

#include "main.h"

/*
 * NOTE:
 * Each task should have a associated function, which we defined ask SchedTask_Func
 * The function should have the form "void function_name(void *param)"
 */
typedef void (*SchedulerTask_Callback)(void*);

/*
 * NOTE:
 * The scheduler_task_t is a structure containing all the required infomation
 * to represent a task in our scheduler implementation
 *
 * The priority of a test is determined as according to a rule: compare by priority,
 * then compare by runAtTick next
 */
typedef struct
{
    SchedulerTask_Callback pTask;  // function pointer
    void *pTaskArg;                // argument for task
    uint8_t priority;              // priority for task
    uint32_t runAtTick;            // scheduled to be run at tick
    uint32_t periodTick;           // period for auto-reload task, 0 if not auto-reload
    uint8_t taskID;                // used to identify task
} SchedulerTask;


#endif /* INC_LOGICAL_SCHEDULER_SCHEDULERTASK_H_ */

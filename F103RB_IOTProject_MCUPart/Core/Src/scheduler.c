/*
 * scheduler.c
 *
 *  Created on: Apr 4, 2023
 *      Author: Admin
 */

#include <scheduler.h>

sTasks SCH_tasks_G[SCH_MAX_TASKS];
uint8_t head_index_task = 0;
uint8_t tail_index_task = 0;
uint8_t isFull = 0;
//uint8_t looped = 0;
//int Error_code_G = 0;

void SCH_Delete_Task(uint8_t index)
{
	if (SCH_tasks_G[index].pTask != 0){
		SCH_tasks_G[index].pTask = 0x0000;
		SCH_tasks_G[index].Delay = 0;
		SCH_tasks_G[index].Period = 0;
		SCH_tasks_G[index].RunMe = 0;
		SCH_tasks_G[index].TaskID = 0;
	}
}

void SCH_Init(void)
{
	for (int i = 0; i < SCH_MAX_TASKS; i++){
		SCH_Delete_Task(i);
	}
//	Error_code_G = 0;
	head_index_task = 0;
	tail_index_task = 0;
//	looped = 0;
	isFull = 0;
}

void SCH_Queue_Task ( sTasks newTask, uint32_t DELAY)
{
	int idx = head_index_task;
//	find index to insert task
	while (idx != tail_index_task && SCH_tasks_G[idx].Delay <= DELAY){
		DELAY -= SCH_tasks_G[idx].Delay;
		idx++;
		if (idx >= SCH_MAX_TASKS) idx = 0;
	}
//	subtract newTask's delay from all tasks' delay after index till tail_index_task
//	and move those tasks back 1 slot
	int i = tail_index_task;
	while (i != idx){
		int prev = i - 1;
		if (prev < 0) prev = SCH_MAX_TASKS_1;

		SCH_tasks_G[i] = SCH_tasks_G[prev];
//		SCH_tasks_G[i].Delay -= DELAY;
		if (prev == idx){
			SCH_tasks_G[i].Delay -= DELAY;
		}

		i--;
		if (i < 0) i = SCH_MAX_TASKS_1;
	}

	newTask.Delay = DELAY;
	SCH_tasks_G[idx] = newTask;

	tail_index_task++;
	if (tail_index_task >= SCH_MAX_TASKS){
		tail_index_task = 0;
	}
	if (tail_index_task == head_index_task)
		isFull = 1;
}

void SCH_Add_Task ( void (*pFunction)() ,
					uint32_t DELAY,
					uint32_t PERIOD)
{
	if (!isFull) {
		sTasks newTask;
		newTask.pTask = pFunction;
		newTask.Delay = DELAY / TIMER_CYCLE;
		newTask.Period = PERIOD / TIMER_CYCLE;
		newTask.RunMe = 0;

		newTask.TaskID = tail_index_task;

		SCH_Queue_Task(newTask, newTask.Delay);
	}
}

void SCH_Update(void)
{
	if (SCH_tasks_G[head_index_task].Delay > 0)
	{
		SCH_tasks_G[head_index_task].Delay--;
	}
	else
	{
		//SCH_tasks_G[head_index_task].Delay = SCH_tasks_G[head_index_task].Period;
		SCH_tasks_G[head_index_task].RunMe++;
	}
}

void SCH_Dispatch_Tasks(void){
	if(SCH_tasks_G[head_index_task].RunMe > 0)
	{
		SCH_tasks_G[head_index_task].RunMe--;
		(*SCH_tasks_G[head_index_task].pTask)(); //execute task

		sTasks task = SCH_tasks_G[head_index_task];
		SCH_Delete_Task(head_index_task);
		head_index_task++;
		if (head_index_task >= SCH_MAX_TASKS){
			head_index_task = 0;
		}
		if (task.Period > 0){
			SCH_Queue_Task(task, task.Period);
		}
	}
}

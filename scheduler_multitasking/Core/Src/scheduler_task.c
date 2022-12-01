/*
 * scheduler_task.c
 *
 *  Created on: 1 thg 12, 2022
 *      Author: Guest_demo
 */

#include "scheduler_task.h"

schedTask* make_task(void (*pTask)(uint32_t), uint32_t led_index, const uint32_t DELAY, const uint32_t PERIOD){
	schedTask *task = malloc(sizeof (schedTask));
	task->pTask = pTask;
	task->led_index = led_index;
	task->Delay = DELAY / TICK;
	task->Period = PERIOD / TICK;
	task->RunMe = 0;
	task->nextTask = NULL;
	return task;
}

void delete_task(schedTask *task){
	free(task);
}

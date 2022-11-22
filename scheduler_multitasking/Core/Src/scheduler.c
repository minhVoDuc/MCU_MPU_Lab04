/*
 * scheduler.c
 *
 *  Created on: Nov 8, 2022
 *      Author: Guest_demo
 */

#include "scheduler.h"

sTask SCH_tasks_G[MAX_OF_TASKS];
uint32_t	current_task_id;

void SCH_Init(){ //Initially setting scheduler
	//delete any leftover tasks
	for (uint8_t i = 0; i < MAX_OF_TASKS; i++){
		SCH_Delete_Task(i);
	}
	current_task_id = 0; //set init task id
}

void SCH_Update(){
	for (uint8_t i = 0; i < current_task_id; i++){
		if (SCH_tasks_G[i].Delay > 0){
			SCH_tasks_G[i].Delay--;
		}
		else {
			SCH_tasks_G[i].RunMe = 1;
			SCH_tasks_G[i].Delay = SCH_tasks_G[i].Period;
		}
	}
}

void SCH_Dispatch_Tasks(){
	for (uint8_t i = 0; i < current_task_id; i++){
		if (SCH_tasks_G[i].RunMe){
			(*SCH_tasks_G[i].pTask)(SCH_tasks_G[i].led_index); 	//run task
			SCH_tasks_G[i].RunMe = 0;	//reset RunMe flag
			//if task is one-shot, delete it
			if (SCH_tasks_G[i].Period == 0){
				SCH_Delete_Task(SCH_tasks_G[i].TaskID);
			}
		}
	}
}

uint32_t SCH_Add_Task(void (*pFunc)(uint32_t), uint32_t led_index, const uint32_t DELAY, const uint32_t PERIOD){
	if (current_task_id == MAX_OF_TASKS){ //full of task
		return RETURN_ERROR;
	}
	//add new task
	SCH_tasks_G[current_task_id].TaskID = current_task_id;
	SCH_tasks_G[current_task_id].pTask = pFunc;
	SCH_tasks_G[current_task_id].led_index = led_index;
	SCH_tasks_G[current_task_id].Delay = DELAY / TICK;
	SCH_tasks_G[current_task_id].Period = PERIOD / TICK;
	SCH_tasks_G[current_task_id].RunMe = 0;

	return current_task_id++; //increase current task id
}

uint8_t SCH_Delete_Task(const uint32_t TaskID){
	uint8_t Return_code;
	if (SCH_tasks_G[TaskID].pTask){
		for (uint8_t i = TaskID; i < current_task_id-1; i++){
				SCH_tasks_G[i] = SCH_tasks_G[i+1];
		}
		SCH_tasks_G[current_task_id].TaskID = 0;
		SCH_tasks_G[current_task_id].pTask = 0x0000;
		SCH_tasks_G[current_task_id].Delay = 0;
		SCH_tasks_G[current_task_id].Period = 0;
		SCH_tasks_G[current_task_id].RunMe = 0;
		current_task_id--;
		Return_code = RETURN_NORMAL;
	}
	else {
		Return_code = RETURN_ERROR;
	}
	return Return_code;
}

/*
 * scheduler_ptr.h
 *
 *  Created on: 1 thg 12, 2022
 *      Author: Guest_demo
 */

#ifndef INC_SCHEDULER_PTR_H_
#define INC_SCHEDULER_PTR_H_

#include "scheduler_task.h"

typedef struct {
	uint32_t cur_size;
	schedTask *head;
} schedList;

typedef struct {
	schedTask *head;
	schedTask *tail;
} schedQueue;

#define SCH_MAX_TASKS 	10
#define NO_TASK_ID		0
#define RETURN_ERROR	0
#define RETURN_NORMAL	1

void SCH_Init();

void SCH_Add_To_List(schedTask *task);
schedTask* SCH_Get_From_List();

void SCH_queue_push(schedTask *task);
schedTask* SCH_queue_pop();

void SCH_Dispatch_Tasks(void);
void SCH_Update(void);
void SCH_Report_Status(void);
void SCH_Go_To_Sleep();
void SCH_Add_Task(void (*pTask)(uint32_t), uint32_t led_index, const uint32_t Delay, const uint32_t Period);
uint32_t SCH_Delete_Task();

#endif /* INC_SCHEDULER_PTR_H_ */

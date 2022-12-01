/*
 * scheduler_task.h
 *
 *  Created on: 1 thg 12, 2022
 *      Author: Guest_demo
 */

#ifndef INC_SCHEDULER_TASK_H_
#define INC_SCHEDULER_TASK_H_

#include <stdlib.h>
#include <stdint.h>

#define TICK			10

typedef struct {
	void 	(*pTask)(uint32_t); //pointer to the task (must be a 'void(void)' function)
	uint32_t	led_index; 		// led_id for pTask
	uint32_t 	Delay; 		//delay (ticks) until the function will be first run
	uint32_t	Period; 	//interval (ticks) between subsequent runs
	uint8_t	RunMe; 			//incremented (by scheduler) when task is due to execute

	uint32_t TaskID;
	struct schedTask *nextTask;
} schedTask;

schedTask* make_task(void (*pTask)(uint32_t), uint32_t led_index, const uint32_t DELAY, const uint32_t PERIOD);
void delete_task(schedTask *task);

#endif /* INC_SCHEDULER_TASK_H_ */

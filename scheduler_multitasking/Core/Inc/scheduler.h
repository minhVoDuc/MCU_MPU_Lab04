/*
 * software_timer.h
 *
 *  Created on: Nov 8, 2022
 *      Author: Guest_demo
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include <stdint.h>

#define MAX_OF_TASKS	10
#define	TICK			10
#define RETURN_NORMAL	1
#define RETURN_ERROR	0

typedef struct{
	uint32_t	TaskID;			//id of the task
	void 		(*pTask)(uint32_t);	//function of the task
	uint32_t	led_index; 		// led_id for pTask
	uint32_t	Delay;			//when initally run the task
	uint32_t	Period;			//how long run the task again
	uint8_t		RunMe;			//is the task ready to run?
} sTask;

void archv_SCH_Init();
void archv_SCH_Update();
void archv_SCH_Dispatch_Tasks();
uint32_t archv_SCH_Add_Task(void (*pFunc)(uint32_t), uint32_t led_index, const uint32_t DELAY, const uint32_t PERIOD);
uint8_t archv_SCH_Delete_Task(const uint32_t TaskID);

#endif /* INC_SCHEDULER_H_ */

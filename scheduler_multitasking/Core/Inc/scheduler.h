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

typedef struct{
	uint32_t	TaskID;			//id of the task
	void 		(*pTask)(void);	//function of the task
	uint32_t	Delay;			//when initally run the task
	uint32_t	Period;			//how long run the task again
	uint8_t		RunMe;			//is the task ready to run?
} sTask;

void SCH_Init();
void SCH_Update();
void SCH_Dispatch_Tasks();
void SCH_Add_Task(void (*pFunc)(void), const uint32_t DELAY, const uint32_t PERIOD);
void SCH_Delete_Task(const uint32_t TaskID);

#endif /* INC_SCHEDULER_H_ */

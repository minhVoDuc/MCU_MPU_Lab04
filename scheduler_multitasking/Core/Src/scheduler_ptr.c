/*
 * scheduler_ptr.c
 *
 *  Created on: 1 thg 12, 2022
 *      Author: Guest_demo
 */

#include "scheduler_ptr.h"
#include "main.h"
#include "global.h"

//the array of tasks
schedList SCH_tasks_G;
schedQueue SCH_ready_queue;
uint32_t time;
uint32_t SCH_task_ID;

void SCH_Init(){
	SCH_tasks_G.cur_size = 0;
	SCH_tasks_G.head = NULL;
	time = 0;
	SCH_task_ID = 0;
	SCH_ready_queue.head = NULL;
	SCH_ready_queue.tail = NULL;
}

//------------------------- list method ---------------------------------
void SCH_Add_To_List(schedTask *task){
	if (SCH_tasks_G.cur_size == SCH_MAX_TASKS) return;
	SCH_tasks_G.cur_size++;
	//add first task
	if (SCH_tasks_G.cur_size == 1){
		SCH_tasks_G.head = task;
		return;
	}
	uint32_t time_interval = 0;
	schedTask *currTask = SCH_tasks_G.head,
			  *prevTask = NULL;
	while(currTask) {
		time_interval += currTask->Delay;
		if (task->Delay < time_interval) break;
		prevTask = currTask;
		currTask = currTask->nextTask;
	}
	if (prevTask == NULL) { //before first element of the list
		SCH_tasks_G.head->Delay -= task->Delay;
		task->nextTask = SCH_tasks_G.head;
		SCH_tasks_G.head = task;
		return;
	}
	if (currTask == NULL) { //after last element of the list
		task->Delay -= time_interval;
	}
	else { //added task is between 2 elements of the list
		uint32_t tmp = task->Delay;
		task->Delay -= (time_interval - currTask->Delay);
		currTask->Delay = time_interval - tmp;
	}
	task->nextTask = currTask;
	prevTask->nextTask = task;
}

schedTask* SCH_Get_From_List(){
	schedTask *task = SCH_tasks_G.head; //get first element of the list and soft remove it
	SCH_tasks_G.head = task->nextTask;
	task->nextTask = NULL;
	SCH_tasks_G.cur_size--;
	return task;
}

//------------------------ queue method -------------------------------------
void SCH_queue_push(schedTask *task) {
	if (SCH_ready_queue.head == NULL) {
		SCH_ready_queue.head = task;
	}
	else {
		SCH_ready_queue.tail->nextTask = task;
	}
	SCH_ready_queue.tail = task;
}

schedTask* SCH_queue_pop() {
	if (SCH_ready_queue.head == NULL) return NULL;
	schedTask *task = SCH_ready_queue.head;
	SCH_ready_queue.head = task->nextTask;
	return task;
}

//------------------------- interface method ---------------------------------
void SCH_Update(void){ //update from SCH_tasks_G, if could run -> push to ready queue
	time += TICK; //use for sending to uart
	//only calculate first element of waiting queue
	if (SCH_tasks_G.head){
		if (SCH_tasks_G.head->Delay == 0) {
			//the task is due to run
			SCH_tasks_G.head->RunMe = 1; //active flag 'RunMe'
			if (SCH_tasks_G.head->Period){
				//schedule periodic tasks to run again
				SCH_tasks_G.head->Delay = SCH_tasks_G.head->Period;
			}
			schedTask *runableTask = SCH_Get_From_List();
			SCH_queue_push(runableTask);
		}
		else {
			//not yet ready to run: just decrease the delay
			SCH_tasks_G.head->Delay -= 1;
		}
	}
}

void SCH_Dispatch_Tasks(void){ //get from ready queue
	//dispatching (running) the next task (if one is ready)
	schedTask *runningTask = SCH_queue_pop();
	if (runningTask) {
		uart_send_str(">>>>>>>>");
		uart_send_num("Task running: ", runningTask->TaskID);
		uart_send_num("When start: ", time);
		(*runningTask->pTask)(runningTask->led_index); //run the task
		uart_send_num("When end: ", time);
		//periodic tasks will auto run again
		//if it's a 'oneshot' task, remove it from the array
		if (runningTask->Period != 0){
			runningTask->RunMe = 0; //reset RunMe flag
			//adding task again to waiting queue
			SCH_Add_To_List(runningTask);
		}
		else {
			uart_send_str("Task one-shot die...");
		}
	}
}


void SCH_Add_Task(void (*pTask)(uint32_t), uint32_t led_index, const uint32_t DELAY, const uint32_t PERIOD){
	//check if reaching the end of the list
	if (SCH_tasks_G.cur_size == SCH_MAX_TASKS){
		return;
	}
	//otherwise, index is valid - there's a space in task array
	schedTask *newTask = make_task(pTask, led_index, DELAY, PERIOD);
	newTask->TaskID = SCH_task_ID++;
	//add to list
	SCH_Add_To_List(newTask);
}

uint32_t SCH_Delete_Task(){ //delete first task of list
	uint32_t Return_code;
	if (SCH_tasks_G.head == NULL) { //if list is empty
		Return_code = RETURN_ERROR;
	}
	else {
		Return_code = RETURN_NORMAL;
		schedTask *delTask = SCH_Get_From_List();
		delete_task(delTask);
	}
	return Return_code;
}

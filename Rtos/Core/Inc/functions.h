/*
 * functions.h
 *
 *  Created on: Sep 21, 2026
 *      Author: THE ONE
 */

#ifndef INC_FUNCTIONS_H_
#define INC_FUNCTIONS_H_
#include "stdint.h"
#include "scheduler.h"
int Tcb_Allocation(void);
int Task_Create(void(*fun)(void) ,uint32_t size,const char *task_name, uint8_t task_state, uint8_t task_priority);

int get_Task_priority(TCB_T *Task_handler);
void set_Task_priority(TCB_T *Task_handler, uint8_t priority);
void suspend_task(TCB_T *Task_handler, enum State state_t);
void ready_task(TCB_T *Task_handler);
void task_init(void);
#endif /* INC_FUNCTIONS_H_ */

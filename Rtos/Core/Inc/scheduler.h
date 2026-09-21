/*
 * scheduler.h
 *
 *  Created on: Sep 21, 2026
 *      Author: THE ONE
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "main.h"

extern uint32_t _estack;

#define STACK_START _estack

#define MAX_TASKS 16u
#define MAX_TASK_NAME 50u
#define WORD_ALLOCATION 4u  //if it i s 8 bit allocation give 1 ,16 bit means 2 ,32 bit means 4
typedef struct st
{
	uint32_t psp_value;
	uint32_t Task_Size;  //give with respect to words
	uint8_t Task_name[MAX_TASK_NAME];
	uint8_t Task_State;
	uint8_t Task_pri;
	void (*fun)(void);
}TCB_T;


#endif /* INC_SCHEDULER_H_ */

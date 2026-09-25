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

#define MAX_TASKS         16u
#define MAX_TASK_NAME     50u
#define WORD_ALLOCATION   4u  //if it i s 8 bit allocation give 1 ,16 bit means 2 ,32 bit means 4
#define MAX_TASK_PRI      10u //max value we can keep up to 255
#define EXE_RETURN_NF_TM  0xFFFFFFFD
#define XPSR_THUMB_STATE  0x01000000
typedef struct st
{
	uint32_t psp_value;
	uint32_t Task_Size;  //give with respect to words
	uint32_t MPU_Gaurd;  //for protection if one task is using there stack if enter into anothers task stack
						 //then memmanage error will come
	uint8_t Task_name[MAX_TASK_NAME];
	uint32_t Task_block;
	uint8_t Task_State;
	uint8_t Task_pri;//more value means high priority
	void (*fun)(void);
}TCB_T;
enum State{
	READY=1,
	RUNNING=2,
	BLOCKED=3,
	SUSPEND=4
};

#endif /* INC_SCHEDULER_H_ */

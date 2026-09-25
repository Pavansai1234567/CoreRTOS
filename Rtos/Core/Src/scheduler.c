/*
 * scheduler.c
 *
 *  Created on: Sep 21, 2026
 *      Author: THE ONE
 */
#include "functions.h"
#include "scheduler.h"
#include "string.h"

TCB_T TCB[MAX_TASKS];

uint8_t gu8_curr_task_tcb=0,
		gu8_max_task_created=0;


int Tcb_Allocation(void)
{
	/* if they are creating more than the they declard task in the header file it will return 0*/
	if(gu8_max_task_created>=MAX_TASKS)
		return 0;
	if(gu8_max_task_created==0)
	{
		TCB[gu8_max_task_created].psp_value=STACK_START;
		gu8_max_task_created++;
	}
	else
	{
		/* here next psp value address will start from psp value of before task - task size and
		 * for protection we have done -1 also
		 */
		uint32_t Next_psp_value = ((TCB[gu8_max_task_created-1].psp_value ) -
									((TCB[gu8_max_task_created-1].Task_Size ) * WORD_ALLOCATION) ) ;

		/*
		 * for cortex mx processor is fetch the address in 8 byte so i have given address
		 * in the form of 8 byte before that 8 value
		 */
		Next_psp_value = ((Next_psp_value ) / 8) * 8;

		TCB[gu8_max_task_created].psp_value= Next_psp_value ;

		gu8_max_task_created++;
	}
	return 1;
}
int Task_Create(void(*fun)(void) ,uint32_t size,const char *task_name, uint8_t task_state, uint8_t task_priority)
{
	int local_value=Tcb_Allocation();

	if(local_value==0)
	{
		return 0;//task are more so tcb not created;
	}

	local_value=gu8_max_task_created-1;

	if(local_value<0)
	{
		return 0;
	}

	TCB[local_value].Task_Size=size;

	strcpy((char *)TCB[local_value].Task_name,task_name);

	TCB[local_value].Task_State=task_state;

	if(task_priority>MAX_TASK_PRI)
		task_priority=MAX_TASK_PRI;

	TCB[local_value].Task_State=task_priority;//more value means high priority

	TCB[local_value].fun=fun;

	TCB[local_value].Task_block=0;


	uint32_t gaurd=TCB[local_value].psp_value+((TCB[local_value].Task_Size) *WORD_ALLOCATION);
	gaurd=(uint32_t)(gaurd/32)*32;

	if(gaurd==0)return 0;

	TCB[local_value].MPU_Gaurd=gaurd;

	local_value=0;//for safe side i have assign zero
	gaurd=0;

	return 1;
}
int get_Task_priority(TCB_T *Task_handler)
{
	return (uint8_t)(Task_handler->Task_pri);
}
/*
 * @param priority if you pass mre than this MAX_TASK_PRI macro value
 * it will assign this macro value only
 */
void set_Task_priority(TCB_T *Task_handler, uint8_t priority)
{
	if(priority>MAX_TASK_PRI)
		priority=MAX_TASK_PRI;
	((Task_handler)->Task_pri)=priority;
}
/*
 * @param state_t should be SUSPEND
 * we can chage the task state to suspend only we cant change to
 * block or ready or running or blocked in this funtion
 * after setting to suspend state and we have done context switchin this function
 */
void suspend_task(TCB_T *Task_handler, enum State state_t)
{
	enum State temp=state_t;

	if(temp==SUSPEND)
	{
		((Task_handler)->Task_State)=SUSPEND;

		//here we need to setup for context switching
	}
	else
	{
		return;
	}
}
/*
 * if the task is in suspend state only that task will come to ready state
 * if it is in running or blocked means it will not come to ready
 */
void ready_task(TCB_T *Task_handler)
{
	if((Task_handler->Task_State)==SUSPEND)
		((Task_handler)->Task_State)=READY;
	else
		return;
}
void task_init(void)
{
	uint32_t *l_psp=0;
	for(int i=0;i<gu8_max_task_created;i++)
	{
		l_psp=0;
		l_psp=(uint32_t*)((TCB[i].psp_value/32)*32);

		(*l_psp--)=XPSR_THUMB_STATE;  //adding processor to thumb state in Xpsr register

		(*l_psp--)=(uint32_t)(TCB[i].fun);  //assiging task handler to PC (R15)

		(*l_psp--)=EXE_RETURN_NF_TM;  //LR (R14) is setup for return in threadmode with psp and no floating unit
		/*
		 * R12 R3 R2 R1 R0 R4 R5 R6 R7 R8 R9 R10 R11 =0
		 */
		for(int j=0 ;j<13;j++)
		{
			(*l_psp--)=0;
		}
		TCB[i].psp_value=(uint32_t)l_psp; //and reassiging the currect psp to task handler psp value
	}
}

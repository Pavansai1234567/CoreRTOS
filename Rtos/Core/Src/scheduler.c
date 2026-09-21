/*
 * scheduler.c
 *
 *  Created on: Sep 21, 2026
 *      Author: THE ONE
 */
#include "functions.h"
#include "scheduler.h"

TCB_T TCB[MAX_TASKS];

uint8_t gu8_curr_task_tcb=0,
		gu8_max_task_created=0;


void Tcb_Allocation(void)
{
	/* if they are creating more than the they declard task in the header file it will return 0*/
	if(gu8_max_task_created>=MAX_TASKS)
		return;
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
									((TCB[gu8_max_task_created-1].Task_Size ) * WORD_ALLOCATION) ) -1;

		/*
		 * for cortex mx processor is fetch the address in 8 byte so i have given address
		 * in the form of 8 byte
		 */
		Next_psp_value = ((Next_psp_value ) / 8) * 8;

		TCB[gu8_max_task_created].psp_value= Next_psp_value ;

		gu8_max_task_created++;
	}
}


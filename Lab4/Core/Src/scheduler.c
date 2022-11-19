/*
 * scheduler.c
 *
 *  Created on: Nov 8, 2022
 *      Author: shiba
 */

#include"scheduler.h"

int SCH_CYCLE = 10;

void SCH_Init(void){
	unsigned char i;
	for(i = 0; i < SCH_MAX_TASKS; i++){
		SCH_Delete_Task(i);
	}

	Error_code_G = 0;
//	Timer_init();
//	Watdog_init();
}

void SCH_Update(void){
	unsigned char index;
	for(index = 0; index < SCH_MAX_TASKS; index++){
		// Check if there is a task at this location
		if(SCH_Tasks_G[index].pTask != 0){
			//Task ready to run
			if(SCH_Tasks_G[index].Delay == 0){
				SCH_Tasks_G[index].RunMe++;
				//Schedule periodic tasks to run again
				if(SCH_Tasks_G[index].Period)
					SCH_Tasks_G[index].Delay = SCH_Tasks_G[index].Period;
			}
			else {
				//Not yet ready to run
				SCH_Tasks_G[index].Delay--;
			}
		}
	}
}

unsigned char SCH_Add_Task(void (*pFunction)(), uint32_t Delay, uint32_t Period){
	unsigned char index = 0;
	while((SCH_Tasks_G[index].pTask != 0) && index < SCH_MAX_TASKS){
		index++;
	}
	if(index == SCH_MAX_TASKS){
		Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
		return SCH_MAX_TASKS;
	}
	SCH_Tasks_G[index].pTask = pFunction;
	SCH_Tasks_G[index].Delay = Delay / SCH_CYCLE;
	SCH_Tasks_G[index].Period = Period / SCH_CYCLE;
	SCH_Tasks_G[index].RunMe = 0;
//	SCH_Tasks_G[index].TaskID = index;

	return index;
}

void SCH_Dispatch_Tasks(void){
	unsigned char index;
	for(index = 0; index < SCH_MAX_TASKS; index++){
		if(SCH_Tasks_G[index].RunMe > 0){
			(*SCH_Tasks_G[index].pTask)();
			SCH_Tasks_G[index].RunMe--;
			if(SCH_Tasks_G[index].Period == 0){
				SCH_Delete_Task(index);
			}
		}

	}
//	SCH_Report_Stattus();
	SCH_Go_To_Sleep();
}

void SCH_Go_To_Sleep(void){
	//Entering Sleep Mode
	HAL_SuspendTick();
	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
	HAL_ResumeTick();
}

unsigned char SCH_Delete_Task(uint32_t TaskID){
	unsigned char Return_code;
	if(SCH_Tasks_G[TaskID].pTask == 0){
		Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;
		Return_code = RETURN_ERROR;
	}
	else {
		Return_code = RETURN_NORMAL;
	}
	SCH_Tasks_G[TaskID].pTask = 0x0000;
	SCH_Tasks_G[TaskID].Delay = 0;
	SCH_Tasks_G[TaskID].Period = 0;
	SCH_Tasks_G[TaskID].RunMe = 0;
	return Return_code;
}

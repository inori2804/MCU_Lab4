/*
 * scheduler.h
 *
 *  Created on: Nov 8, 2022
 *      Author: shiba
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include"stdint.h"
#include"main.h"

typedef struct {
	void (*pTask)(void);
	uint32_t Delay;
	uint32_t Period;
	uint8_t RunMe;
	uint32_t TaskID;
} sTask;

#define SCH_MAX_TASKS 	40
#define NO_TASK_ID 		0

#define RETURN_ERROR					1
#define RETURN_NORMAL 					2
#define ERROR_SCH_CANNOT_DELETE_TASK	3
#define ERROR_SCH_TOO_MANY_TASKS		4

unsigned char Error_code_G;
sTask SCH_Tasks_G[SCH_MAX_TASKS];

void SCH_Init(void);
void SCH_Update(void);
unsigned char SCH_Add_Task(void (*pFuntion)(), uint32_t Delay, uint32_t Period);
void SCH_Dispatch_Tasks(void);
unsigned char SCH_Delete_Task(uint32_t TaskID);
void SCH_Go_To_Sleep(void);

#endif /* INC_SCHEDULER_H_ */

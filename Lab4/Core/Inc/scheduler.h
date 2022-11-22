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
#include"linkedList.h"

typedef struct {
	void (*pTask)(void);
	uint32_t Delay;
	uint32_t Period;
	uint8_t RunMe;
	uint32_t TaskID;
} sTask;

#define SCH_MAX_TASKS 	40
#define NO_TASK_ID 		0

#define RETURN_ERROR					0
#define RETURN_NORMAL 					1
#define ERROR_SCH_CANNOT_DELETE_TASK	2
#define ERROR_SCH_TOO_MANY_TASKS		3

unsigned char Error_code_G;
//use to save index of task
int indexTask[SCH_MAX_TASKS];
//queue contain information of tasks
extern struct llist list;

extern int timestamp;
extern int flag_uart;
extern int data;
extern int data_id;

void SCH_Init(void);
void SCH_Update(void);
//Unit of delay and period is "ms"
unsigned char SCH_Add_Task(void (*pFuntion)(), uint32_t Delay, uint32_t Period);
void SCH_Dispatch_Tasks(void);
unsigned char SCH_Delete_Task(uint32_t TaskID);
void SCH_Go_To_Sleep(void);

#endif /* INC_SCHEDULER_H_ */

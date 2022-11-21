/*
 * scheduler.c
 *
 *  Created on: Nov 8, 2022
 *      Author: shiba
 */

#include"scheduler.h"

int SCH_CYCLE = 10;
struct llist list;
int indexTask[SCH_MAX_TASKS];

void SCH_Init(void) {
	//initial list
	list.size = 0;
	list.head = NULL;
	list.tail = NULL;
	//set all value of array index to -1
	for (int i = 0; i < SCH_MAX_TASKS; i++) {
		indexTask[i] = -1;
	}
	Error_code_G = 0;
//	Timer_init();
//	Watdog_init();
}

/*
 * In some special case we have some tasks
 * have same Period and Delay when we create task.
 *
 * So we need check the delay of next task if in
 * current interrupt have one task with delay is 0
 * or head ready to run (flag use for this purpose).
 *
 * When flag is 1 and if Delay of next task is 0 we just do the
 * same thing with next task like previous head.
 *
 * If head not yet ready to run then we wait to go to next Interrupt.
 */
void SCH_Update(void) {
//	int flag = 0; //to check if head have delay == 0
//	//first we check head of list
//	do {
//		if (list.head) {
//			struct item *tmp = list.head;
//			flag = 0;
//			if (tmp->data.Delay == 0) {
//				//delay of head is 0 then we increase RunMe
//				tmp->data.RunMe++;
//				if(tmp->data.Period > 0){
//					//Period > 0 so setup to run task again.
//					//We add task again to list
//					//with Delay is Period and RunMe already increased
//					add(&list, tmp->data.pTask, tmp->data.Period, tmp->data.Period,
//											tmp->data.TaskID, tmp->data.RunMe);
//					//then delete head is contain task have Delay is 0
//					deleteHead();
//					flag = 1;
//				}
//			} else
//				//head not yet ready to run
//				tmp->data.Delay--;
//		}
//	} while (flag && list.head && list.head->data.Delay == 0);
	struct item *tmp = list.head;
	if (tmp && tmp->data.Delay > 0) {
		tmp->data.Delay--;
	}
}

unsigned char SCH_Add_Task(void (*pFunction)(), uint32_t Delay, uint32_t Period) {
	int flag = 0; //use to check if reach to max task can execute
	int i; //index of task will add to list
	for (i = 0; i < SCH_MAX_TASKS; i++) {
		//find position empty
		if (indexTask[i] == -1) {
			indexTask[i] = i;
			flag = 1;
			break;
		}
	}
	if (!flag) {
		//Full so we return Error
		Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
		return SCH_MAX_TASKS;
	}
	//Not Full so we add task into List
	add(&list, pFunction, Delay / SCH_CYCLE, Period / SCH_CYCLE, i, 0);
	//return index of task
	return i;
}

void SCH_Dispatch_Tasks(void) {
	struct item *tmp = list.head;
//	while (tmp) {
//		if (tmp->data.RunMe > 0) {
//			(*tmp->data.pTask)();
//			tmp->data.RunMe--;
//			if(tmp->data.Period == 0 && tmp->data.RunMe == 0){
//				SCH_Delete_Task(tmp->data.TaskID);
//			}
//		}
//		tmp = tmp->next;
//	}

	while (tmp && tmp->data.Delay == 0) {
		(*tmp->data.pTask)();
		if (tmp->data.Period == 0) {
			SCH_Delete_Task(tmp->data.TaskID);
			tmp = list.head;
		} else if (tmp->data.Period > 0) {
			add(&list, tmp->data.pTask, tmp->data.Period, tmp->data.Period,
					tmp->data.TaskID, tmp->data.RunMe);
			//then delete head is contain task have Delay is 0
			deleteHead();
			tmp = list.head;
		} else
			tmp = tmp->next;
	}
//	SCH_Report_Stattus();
	SCH_Go_To_Sleep();
}

/* NOTE:
 * Go to Sleep function I don't know operate true or false
 * and how to test that.
 * Because It is optional and when add into Dispatcher it
 * don't cause Error so I hope it True
 */
void SCH_Go_To_Sleep(void) {
	//To prevent wake-up by Systick Interrupt we use SuspendTick
	HAL_SuspendTick();
	//Entering Sleep Mode
	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
	HAL_ResumeTick();
}

uint8_t SCH_Delete_Task(uint32_t TaskID) {
	uint8_t Return_code = RETURN_NORMAL;
	if (indexTask[TaskID] == -1) {
		//Not yet create task with value TaskID is TaskID (parameter).
		Return_code = RETURN_ERROR;
	} else {
		//set value of array have index is TaskID to -1
		indexTask[TaskID] = -1;
		//then call delete function to delete Task.
		delete(&list, TaskID);
	}
	return Return_code;
}

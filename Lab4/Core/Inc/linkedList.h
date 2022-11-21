/*
 * linkedList.h
 *
 *  Created on: Nov 19, 2022
 *      Author: shiba
 */

#ifndef INC_LINKEDLIST_H_
#define INC_LINKEDLIST_H_

#include"stdio.h"
#include"stdint.h"
#include"stdlib.h"
#include"scheduler.h"

struct sTask {
    void (*pTask)(void);
	uint32_t Delay;
	uint32_t Period;
	uint8_t RunMe;
	uint32_t TaskID;
};

struct item{
    struct sTask data;
    struct item* next;
};

struct llist{
    struct item* head;
    struct item* tail;
    int size;
};

void add(struct llist *list, void (*pFunction)(), uint32_t Delay, uint32_t Period, uint32_t TaskID, uint8_t RunMe);
void delete(struct llist *list, uint32_t taskID); //return index of task delete
void deleteHead(); //delete task head of list

#endif /* INC_LINKEDLIST_H_ */

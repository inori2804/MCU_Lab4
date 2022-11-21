/*
 * linkedList.c
 *
 *  Created on: Nov 19, 2022
 *      Author: shiba
 */


#include"linkedList.h"

void add(struct llist *list, void (*pFunction)(), uint32_t Delay, uint32_t Period, uint32_t TaskID, uint8_t RunMe) {
    struct item* tmp = list->head;
    struct item* new = (struct item*)malloc(sizeof(struct item));
    //assign value
    new->data.Delay = Delay;
    new->data.TaskID = TaskID;
    new->data.Period = Period;
    new->data.pTask = pFunction;
    new->data.RunMe = RunMe;
    new->next = NULL;
    // list have no item
    if(list->size == 0) {
        list->head = new;
        list->tail = new;
        list->size++;
        return;
    }
    // head item delay > new delay
    if(list->head->data.Delay > new->data.Delay) {
        new->next = list->head;
        list->head->data.Delay -= new->data.Delay;
        list->head = new;
        list->size++;
        return; //no error
    }
    //sum variable use to save total delay when loop
    uint32_t sum = list->head->data.Delay;
    struct item *prev = tmp;
    tmp = tmp->next;
    while(tmp) {
        if(new->data.Delay <= sum + tmp->data.Delay) {
        	//new delay less than sum then add new to list on that index
            new->next = tmp;
            prev->next = new;
            new->data.Delay -= sum;
            tmp->data.Delay -= new->data.Delay;
            list->size++;
            return;
        }
        sum += tmp->data.Delay;
        prev = tmp;
        tmp = tmp->next;
    }
    //add to tail of list if task have delay > sum of delay all previous task
    if(!tmp) {
        list->tail->next = new;
        list->tail = new;
        new->data.Delay -= sum;
        list->size++;
    }
}

void delete(struct llist *list, uint32_t taskID){
    if(list->size == 0){
        return; //here return error
    }
    struct item *tmp = list->head;
    struct item *prev = tmp;
    //first item have taskID = parameter taskID
    if(tmp->data.TaskID == taskID){
    	//list have one item
        if(list->size == 1){
            list->head = NULL;
            list->tail = NULL;
            list->size = 0;
            free(tmp);
            return; // no error
        } else { //list have more than one item
            list->head = tmp->next;
            list->head->data.Delay += tmp->data.Delay;
            list->size--;
            free(tmp);
            return; // no error
        }
    } else
        tmp = tmp->next;
    //find item have same taskID with parameter
    while(tmp != NULL){
        if(tmp->data.TaskID == taskID){
        	//find success then delete it
            prev->next = tmp->next;
            if(tmp->next){
                tmp->next->data.Delay += tmp->data.Delay;
            }
            tmp->next = NULL;
            list->size--;
            free(tmp);
            return; //no error
        }
        //move to next item
        prev = tmp;
        tmp = tmp->next;
    }
    //no item have taskID == parameter taskID
    return; //here return error
}
//delete head of list and move head to next if success
void deleteHead(){
	if(list.size == 0){
		return; //error
	}
	struct item* tmp = list.head;
	list.head = list.head->next;
	if(!list.head){
		//list have one item then set pointer tail to NULL
		list.tail = NULL;
	}
	free(tmp);
	list.size--;
}

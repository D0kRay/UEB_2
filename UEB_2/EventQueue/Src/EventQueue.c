/*
 * EventQueue.c
 *
 *  Created on: 26.11.2022
 *      Author: marcwech
 */

#include "EventQueue.h"

EventQueue* EventQueue_Init(){

	EventQueue *queue = malloc(sizeof(EventQueue));
	queue->begin = NULL;
	queue->endof = NULL;
	return queue;
}

void addEvent(EventQueue **queue, Event *evt){

	if(*queue == NULL)
		return;

	if(evt == NULL)
		return;

	EventElement *newElement = malloc (sizeof(EventElement));

	if(newElement != NULL){
		newElement->data = *evt;
		newElement->next = NULL;

		if((*queue)->begin == NULL){
			(*queue)->begin = newElement;
			(*queue)->endof = newElement;
		}else{
			(*queue)->endof->next = newElement;
			(*queue)->endof = newElement;
		}
	}
	return;
}

void getEvent(EventQueue **queue, Event *evt){

	if(queue == NULL)
		return;

	if((*queue)->begin == NULL)
		return;

	*evt = (*queue)->begin->data;

	EventElement *tmp = (*queue)->begin;
	(*queue)->begin = tmp->next;
	free(tmp);

	return;
}

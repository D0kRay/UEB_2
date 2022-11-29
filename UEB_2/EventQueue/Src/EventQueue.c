/*
 * EventQueue.c
 *
 *  Created on: 26.11.2022
 *      Author: marcwech
 */

#include "EventQueue.h"

EventQueue* EventQueue_Init(){

	EventQueue *endofQueue = NULL;

	return endofQueue;
}

void addEvent(EventQueue **endofQueue, Event *evt){

	if(evt == NULL)
		return;

	EventQueue *newElement = malloc (sizeof(EventQueue));

	if(newElement != NULL){
		newElement->data = *evt;
		newElement->next = NULL;
		newElement->prev = *endofQueue;

		*endofQueue = newElement;
	}
	return;
}

Event getEvent(EventQueue *endofQueue){

	Event evt;

	if(endofQueue == NULL){
		evt.class = 0;
		evt.source = 0;
		return evt;
	}

	evt = endofQueue->data;

	if(endofQueue->prev == NULL){
		free(endofQueue);
		endofQueue = NULL;
	}
	else{
		endofQueue = endofQueue->prev;
		free(endofQueue->next);
	}


	return evt;
}

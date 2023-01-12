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
	queue->count = 0;
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

		(*queue)->count++;
	}
	return;
}

void getEvent(EventQueue **queue, Event *evt){

	if(*queue == NULL)
		return;

	if((*queue)->begin == NULL)
		return;

	*evt = (*queue)->begin->data;

	EventElement *tmp = (*queue)->begin;
	(*queue)->begin = tmp->next;
	free(tmp);

	(*queue)->count--;

	return;
}

uint8_t isEventQueued(EventQueue *queue){
	if(queue->begin == NULL)
		return 0;
	else
		return 1;
}

void EventInit(Event *evt){
	evt->class = 0;
	evt->message = 0;
}

uint8_t getEventClass(Event evt){

	if(evt.class == Interrupt){
		return Interrupt;
	}
	if(evt.class == Routine){
		return Routine;
	}
	return NoEvent;
}

uint8_t getEventMessage(Event evt){

	return evt.message;
}

void setEventClass(Event *evt, uint8_t Class){
	evt->class = Class;
}
void setEventMessage(Event *evt, uint8_t Message){
	evt->message = Message;
}

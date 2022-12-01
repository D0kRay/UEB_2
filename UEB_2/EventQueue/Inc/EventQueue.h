/*
 * EventQueue.h
 *
 *  Created on: 29.11.2022
 *      Author: marcwech
 */

#ifndef INC_EVENTQUEUE_H_
#define INC_EVENTQUEUE_H_

#ifdef __cplusplus
 extern "C" {
#endif


#include "stdio.h"
#include "stdlib.h"

typedef struct event{
	char class;
	char source;

}Event;

typedef struct element{
	Event data;
	struct element *next;
}EventElement;

typedef struct queue{
	EventElement *begin;
	EventElement *endof;
}EventQueue;

EventQueue* EventQueue_Init();
void addEvent(EventQueue **queue, Event *evt);
void getEvent(EventQueue **queue, Event *evt);


#ifdef __cplusplus
 }
#endif

#endif /* INC_EVENTQUEUE_H_ */

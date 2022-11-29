/*
 * EventQueue.h
 *
 *  Created on: 26.11.2022
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

typedef struct queue{
	Event data;
	struct queue *next;
	struct queue *prev;
}EventQueue;

EventQueue* EventQueue_Init();
void addEvent(EventQueue **endofQueue, Event *evt);
Event getEvent(EventQueue *endofQueue);


#ifdef __cplusplus
 }
#endif


#endif /* INC_EVENTQUEUE_H_ */

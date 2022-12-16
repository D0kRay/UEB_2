/*
 * DT_id_stack.c
 *
 *  Created on: 16.12.2022
 *      Author: marcwech
 */
#include "DT_id_stack.h"

ID_stack *head_ptr;
uint8_t counter;

ST_err_hand ST_init(){
	counter = 0;
	for(int i = 1; i < maxID;i++){
		if(ST_push(i) == ST_ERR)
			return ST_ERR;
	}
	return ST_NOERR;
}

ST_err_hand ST_push(uint8_t ID){
	ID_stack *newElement = malloc(sizeof(ID_stack));
	if(newElement == NULL)
		return ST_ERR;
	newElement->ID =ID;
	newElement->next = NULL;
	newElement->prev = head_ptr;
	head_ptr = newElement;
	counter++;
	return ST_NOERR;
}

uint8_t ST_pop(){

	if(counter == 0)
		return 0;

	ID_stack *tmp = head_ptr;
	head_ptr = tmp->prev;

	counter--;

	return tmp->ID;
}

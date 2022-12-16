/*
 * DT_id_stack.h
 *
 *  Created on: 16.12.2022
 *      Author: marcwech
 */

#ifndef INC_DT_ID_STACK_H_
#define INC_DT_ID_STACK_H_

#include <stdio.h>
#include <stdlib.h>

#define maxID 255

typedef enum{
	ST_NOERR = 0,
	ST_ERR = 1
}ST_err_hand;

typedef struct stack{
	uint8_t ID;
	struct stack *next;
	struct stack *prev;
}ID_stack;

ST_err_hand ST_init();
ST_err_hand ST_push(uint8_t ID);
uint8_t ST_pop();

#endif /* INC_DT_ID_STACK_H_ */

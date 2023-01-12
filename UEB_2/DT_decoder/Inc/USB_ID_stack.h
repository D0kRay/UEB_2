/*
 * USB_ID_stack.h
 *
 *  Created on: 12.01.2023
 *      Author: pasik
 */

#ifndef INC_USB_ID_STACK_H_
#define INC_USB_ID_STACK_H_

#include <stdio.h>
#include <stdlib.h>

#define maxID 255

typedef enum{
	USB_ST_NOERR = 0,
	USB_ST_ERR = 1
}USB_ST_err_hand;

typedef struct USB_stack{
	uint8_t ID;
	struct USB_stack *next;
	struct USB_stack *prev;
}USB_ID_stack;

USB_ST_err_hand USB_ST_init();
USB_ST_err_hand USB_ST_push(uint8_t ID);
uint8_t USB_ST_pop();

#endif /* INC_USB_ID_STACK_H_ */

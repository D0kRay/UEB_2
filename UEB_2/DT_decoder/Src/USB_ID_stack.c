/*
 * USB_ID_stack.c
 *
 *  Created on: 12.01.2023
 *      Author: pasik
 */

#include "USB_ID_stack.h"

USB_ID_stack *usb_head_ptr;
uint8_t usb_counter;

USB_ST_err_hand USB_ST_init(){
	usb_counter = 0;
	for(int i = 1; i < maxID;i++){
		if(USB_ST_push(i) == USB_ST_ERR)
			return USB_ST_ERR;
	}
	return USB_ST_NOERR;
}

USB_ST_err_hand USB_ST_push(uint8_t ID){
	USB_ID_stack *newElement = malloc(sizeof(USB_ID_stack));
	if(newElement == NULL)
		return USB_ST_ERR;

	newElement->ID = ID;
	newElement->prev = NULL;

	if(usb_head_ptr == NULL){
		newElement->next = NULL;
		usb_head_ptr = newElement;

	}else{
		USB_ID_stack *tmp;
		tmp = usb_head_ptr;
		while(tmp->prev != NULL)
			tmp = tmp->prev;
		newElement->next = tmp;
		tmp->prev = newElement;
	}
	usb_counter++;
	return USB_ST_NOERR;
}

uint8_t USB_ST_pop(){

	if(usb_counter == 0)
		return 0;

	USB_ID_stack *tmp = usb_head_ptr;
	usb_head_ptr = tmp->prev;

	usb_counter--;

	return tmp->ID;
}

/*
 * com_provider.c
 * Interface for the data between Highlevel and Lowlevel Microcontroller Software
 *
 *  Created on: 14.11.2022
 *      Author: pasik
 */
#include "com_provider.h"
#include "usbd_cdc_if.h"


//extern USBD_HandleTypeDef hUsbDeviceFS;

static uint8_t buffer_in_use = 0;
volatile uint8_t receive_pending = 0;
uint8_t once = 0;
uint8_t buffer0status = 0;	//represent the actual status of the buffer (0 = buffer empty and not ready for transmit, 1 = buffer full and ready for transmit)
uint8_t buffer1status = 1;	//
uint8_t buffer0[BUFFERSIZE];
uint8_t buffer1[BUFFERSIZE];

uint8_t is_Receive_Complete()
{
	return CDC_Receive_Complete_FS();
}

uint8_t is_Transmit_Complete()
{
	return CDC_Transmit_Complete_FS();
}

/**
 * @brief
 *
 * @param Buffer
 * @param size
 */
void get_Receive_Message(uint8_t *Buffer, uint32_t size)
{
	if(is_Receive_Complete() != 0) {
		if(size > CDC_RX_BUFFER_SIZE){
			CDC_Receive_Data(Buffer, CDC_RX_BUFFER_SIZE);
		} else {
			CDC_Receive_Data(Buffer, CDC_RX_BUFFER_SIZE);
		}
	}
}

uint8_t whichBufferEmpty()
{
	uint8_t buffernr = 5;
	if(buffer_in_use == 0 && buffer0status == BUFFER_NOT_READY) {
		if(is_Transmit_Complete()) {
			buffer_in_use = 0;
			buffer0status = BUFFER_READY;
			buffernr = 0;
			return buffernr;
		}
	} else if(buffer_in_use == 1 && buffer1status == BUFFER_NOT_READY) {
		if(is_Transmit_Complete()) {
			buffer_in_use = 1;
			buffer0status = BUFFER_READY;
			buffernr = 1;
			return buffernr;
		}
	} else if(buffer_in_use == 1 && buffer0status == BUFFER_READY) {
		buffernr = 0;
		return buffernr;
	} else if(buffer_in_use == 0 && buffer1status == BUFFER_READY) {
		buffernr = 1;
		return buffernr;
	}
	return buffernr;
}

void Transmit_Data()
{
	if(buffer_in_use == 0){
		buffer0status = BUFFER_NOT_READY;
		if(CDC_Transmit_FS(buffer0, BUFFERSIZE) == USBD_OK){
			if(is_Transmit_Complete()) {
				buffer0status = BUFFER_READY;
			}
		} else {	//Capture USB Busy Errors and resent the given buffer the second time
			CDC_Transmit_FS(buffer0, BUFFERSIZE);
		}
	} else if (buffer1status == 1){
		buffer1status = BUFFER_NOT_READY;
		if(CDC_Transmit_FS(buffer1, BUFFERSIZE) == USBD_OK){
			if(is_Transmit_Complete()) {
				buffer1status = BUFFER_READY;
			}
		} else {	//Capture USB Busy Errors and resent the given buffer the second time
			CDC_Transmit_FS(buffer1, BUFFERSIZE);
		}
	}
}

uint8_t TransmitBuffer(uint8_t *data, uint32_t size)
{
	uint8_t transmit_pending = 0;

	switch (whichBufferEmpty()) {

		case 0:
			memset(buffer0, 0x00, sizeof(buffer0));
			memcpy(buffer0, data, size);
			buffer_in_use = 0;
			Transmit_Data();
			break;

		case 1:
			memset(buffer1, 0x00, sizeof(buffer1));
			memcpy(buffer1, data, size);
			buffer_in_use = 1;
			Transmit_Data();
			break;

		default:
			transmit_pending = 1;
			break;
	}

	return transmit_pending;
}





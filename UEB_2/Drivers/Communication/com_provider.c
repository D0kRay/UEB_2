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
uint8_t buffer0status = 0;	//represent the actual status of the buffer (0 = buffer empty and ready for transmit, 1 = buffer full and ready for transmit)
uint8_t buffer1status = 1;	//
uint8_t buffer0[BUFFERSIZE];
uint8_t buffer1[BUFFERSIZE];

uint8_t is_receive_complete()
{
	return CDC_Receive_Complete_FS();
}

uint8_t is_transmit_complete()
{
	return CDC_Transmit_Complete_FS();
}

/**
 * @brief
 *
 * @param Buffer
 * @param size
 */
void get_receive_message(uint8_t *Buffer, uint32_t size)
{
	if(is_receive_complete() != 0) {
		if(size > CDC_RX_BUFFER_SIZE){
			memcpy(Buffer, CDC_Receive_Data(), CDC_RX_BUFFER_SIZE);
		} else {
			memcpy(Buffer, CDC_Receive_Data(), size);
		}
		memset(CDC_Receive_Data(), 0x00, CDC_RX_BUFFER_SIZE);
	}
}

uint8_t isBufferEmpty(uint8_t buffernumber)
{
	uint8_t status = 0;
	if(buffernumber == 1) {
		status = buffer0status;
	} else if (buffernumber == 2) {
		status = buffer1status;
	}
	return status;
}

void setBuffer(uint8_t *data, uint8_t size)
{
	if(buffer_in_use == 0) {
		memset(buffer1, 0x00, sizeof(buffer1));
		memcpy(buffer1, data, size);
		buffer1status = 1;
	} else {
		memset(buffer0, 0x00, sizeof(buffer0));
		memcpy(buffer0, data, size);
		buffer0status = 1;
	}
	transmit_data();
}

void transmit_data()
{
	if(is_transmit_complete()) {
		if(buffer_in_use == 0 && buffer0status == BUFFER_READY){
			if(CDC_Transmit_FS(buffer0, BUFFERSIZE) == USBD_OK){
				buffer_in_use = 1;
				memset(buffer0, '\0', BUFFERSIZE);
				buffer0status = 0;
			} else {	//Capture USB Busy Errors and resent the given buffer the second time
				CDC_Transmit_FS(buffer0, BUFFERSIZE);
			}
		} else if (buffer1status == 1){
			if(CDC_Transmit_FS(buffer1, BUFFERSIZE) == USBD_OK){
				buffer_in_use = 0;
				memset(buffer1, '\0', BUFFERSIZE);
				buffer1status = 0;
			} else {	//Capture USB Busy Errors and resent the given buffer the second time
				CDC_Transmit_FS(buffer1, BUFFERSIZE);
			}
		}
	}
}



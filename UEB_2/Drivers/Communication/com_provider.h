/*
 * com_provider.h
 *
 *  Created on: 14.11.2022
 *      Author: pasik
 */

#ifndef COMMUNICATION_COM_PROVIDER_H_
#define COMMUNICATION_COM_PROVIDER_H_

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_if.h"

/* Defines -------------------------------------------------------------------*/
#define PACKAGE_SIZE	64	//Package size in Bytes
#define PACKAGES_PER_TRANSFER	16	//Max amount of packages per transfer from MC to PC
#define BUFFERSIZE PACKAGES_PER_TRANSFER*PACKAGE_SIZE	//The size of the buffer is the package size multiplied with the max amount of packages per transmission
#define BUFFER_READY	1
#define BUFFER_NOT_READY	0

//typedef struct
//{
//  uint8_t  size;
//  uint8_t  actuallength;
//  uint8_t  usedfortransmit;
//} COMPROV_BufferInfoType;


uint8_t Is_Receive_Complete();
uint8_t IsBufferEmpty(uint8_t buffernumber);

void get_Receive_Message(uint8_t *Buffer, uint32_t size);
void setBuffer(uint8_t *data, uint32_t size);
void Transmit_Data();


#endif /* COMMUNICATION_COM_PROVIDER_H_ */

/*
 * DT_interface.c
 *
 *  Created on: Nov 25, 2022
 *      Author: marcwech
 */

#include "DT_interface.h"

list_t *DT_list;

DT_status DT_Init(void* address, uint32_t size){

	DT_status status = 0;

	if(DT_list == NULL)
		DT_list = list_new();	//No initialization
	if(DT_list == NULL)
		return (DT_status)1100;		// NULL pointer after allocation

	if(address == NULL)
		return (DT_status)1000;		// Illegal address for DATA SET

	Dataset *newDataset = malloc(sizeof(list_node_t));
	if(newDataset == NULL)
		return (DT_status)1200;		// NULL pointer after allocation

	newDataset->ID = (uint8_t)DT_list->len;
	newDataset->Counter = 0;
	newDataset->Address = address;
	newDataset->Size = size;
	if(size%64)
		newDataset->MaxPackages = (size/DT_PACKAGE_DATA_SIZE);
	else
		newDataset->MaxPackages = (size/DT_PACKAGE_DATA_SIZE) - 1;

	list_node_t *newNode = list_node_new(newDataset);

	list_rpush(DT_list, newNode);

	return status;
}

DT_status DT_Start(void *Buffer){

	DT_status status = 0;

	if(DT_list == NULL)
		return (DT_status)1000;
	memcpy(Buffer, DT_fillBuffer(DT_list), DT_BUFFER_SIZE);
//	Buffer = DT_fillBuffer(DT_list);

	return status;
}

uint8_t DT_isError(DT_status status){
	if(status)
		return 1;
	else
		return 0;
}

void* DT_Init_Memory(uint32_t size){

	void* mem = NULL;
	MEM_alloc_params(mem, size);
	return mem;
}




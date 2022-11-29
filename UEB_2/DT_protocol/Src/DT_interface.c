/*
 * DT_interface.c
 *
 *  Created on: Nov 25, 2022
 *      Author: marcwech
 */

#include "DT_interface.h"

Dataset DT_list[5];

DT_status DT_Init(void* address, uint32_t size){

	DT_status status = 0;

	if(address == NULL) return (DT_status)1000;		// Illegal address for DATA SET

	for(int i = 0; i < (sizeof(DT_list)/sizeof(Dataset)); i++){

		if(DT_list[i].Address == NULL){
			DT_list[i].ID = i;
			DT_list[i].Counter = 0;
			DT_list[i].Address = address;
			DT_list[i].Size = size;
			status = 1;
		}
		if(status == 1) break;
	}
	return status;
}

DT_status DT_Start(){

	DT_status status = 1;

	// char* str = (char*)DT_list[0].Address;		//ONLY FOR DEBUGGING

	printf("\nText: %s", (char*)DT_list[0].Address);

	return status;
}

void* DT_Init_Memory(uint32_t size){

	void* mem = NULL;
	MEM_alloc_params(mem, size);
	return mem;
}




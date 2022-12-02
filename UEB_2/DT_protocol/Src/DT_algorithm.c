/*
 * DT_algorithm.c
 *
 *  Created on: 01.12.2022
 *      Author: marcwech
 */

#include "DT_algorithm.h"

uint8_t* DT_fillBuffer(list_t *DT_list){

	if(DT_list == NULL)
		return NULL;

	uint8_t* Buffer = malloc(sizeof(DT_BUFFER_SIZE));
	if(Buffer == NULL)
		return NULL;
	memset(Buffer, '0', DT_BUFFER_SIZE);

	uint8_t StatusByte;
	list_node_t *dt_set;


	int count = 0;
	for(int i = DT_BUFFER_OS2; i <= 1024; i += DT_PACKAGE_SIZE){
		StatusByte = 0;
		dt_set = list_at(DT_list, count);
		if(dt_set == NULL){
			StatusByte &= DT_NO_DATA;
			memcpy(&Buffer[i + StatusFlags_OS], &StatusByte, sizeof(uint8_t));
		}else{
			if(dt_set->next == NULL)
				count = 0;
			else
				count++;

			memcpy(Buffer + i + ID_OS, 			&dt_set->val->ID,			sizeof(uint8_t));
			memcpy(Buffer + i + Count_OS, 		&dt_set->val->Counter,		sizeof(uint8_t));
			memcpy(Buffer + i + MaxPackage_OS, 	&dt_set->val->MaxPackages,	sizeof(uint8_t));

			void* temp = dt_set->val->Address + dt_set->val->Counter * DT_PACKAGE_DATA_SIZE;

			memcpy(Buffer + i + Data_OS, 		temp, 		DT_PACKAGE_DATA_SIZE);

			if(dt_set->val->Counter == dt_set->val->MaxPackages){
				list_remove(DT_list, dt_set);
				StatusByte |= DT_TC;
			}
			if(dt_set->val->Counter < dt_set->val->MaxPackages)
				dt_set->val->Counter++;
			memcpy(Buffer + i + StatusFlags_OS, &StatusByte, sizeof(uint8_t));
		}

	}
	return Buffer;
}

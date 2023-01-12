/*
 * DT_algorithm.c
 *
 *  Created on: 01.12.2022
 *      Author: marcwech
 */

#include "DT_algorithm.h"

void DT_fillBuffer(list_t *DT_list, uint8_t* buf){

	if(DT_list == NULL)
		return;

	uint8_t* Buffer = malloc(DT_BUFFER_SIZE);

	memset(Buffer, 0x00, DT_BUFFER_SIZE);

	uint8_t StatusByte;
	list_node_t *dt_set;


	int count = 0;
	for(int i = DT_BUFFER_OS2; i < 1024; i += DT_PACKAGE_SIZE){ //TODO <=1024???
		StatusByte = 0;
		dt_set = list_at(DT_list, count);
		if(dt_set == NULL){
			StatusByte &= DT_NO_DATA;
			memcpy(&Buffer[i + StatusFlags_OS], &StatusByte, sizeof(uint8_t));
		}else{
			while(dt_set->next != NULL){
				if(dt_set->val->F_SendData == F_ON)
					break;
				dt_set = list_at(DT_list, ++count);

				if(dt_set == NULL)
					return;
			}
			if(dt_set->val->F_SendData == F_ON) {

				if(dt_set->next == NULL)
					count = 0;
				else
					count++;

				memcpy(Buffer + i + ID_OS, 			&dt_set->val->ID,			sizeof(uint8_t));
				memcpy(Buffer + i + Count_OS, 		&dt_set->val->Counter,		sizeof(uint8_t));
				memcpy(Buffer + i + MaxPackage_OS, 	&dt_set->val->MaxPackages,	sizeof(uint8_t));

				void* temp = dt_set->val->Address + dt_set->val->Counter * DT_PACKAGE_DATA_SIZE;

				if(dt_set->val->Counter == dt_set->val->MaxPackages){
					memcpy(Buffer + i + Data_OS, 		temp, 		(dt_set->val->Size - (dt_set->val->MaxPackages) * DT_PACKAGE_DATA_SIZE));
					StatusByte |= DT_TC;
				}
				else
					memcpy(Buffer + i + Data_OS, 		temp, 		DT_PACKAGE_DATA_SIZE);

				memcpy(Buffer + i + StatusFlags_OS, &StatusByte, sizeof(uint8_t));


				if(dt_set->val->Counter == dt_set->val->MaxPackages){
					//ST_push(dt_set->val->ID);
//					list_remove(DT_list, dt_set);
					dt_set->val->F_SendData = F_OFF;
				}


				if(dt_set->val->Counter < dt_set->val->MaxPackages)
					dt_set->val->Counter++;

			}
		}

	}
	memcpy(buf, Buffer, DT_BUFFER_SIZE);
	free(Buffer);
	return;
}

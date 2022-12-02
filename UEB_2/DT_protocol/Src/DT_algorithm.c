/*
 * DT_algorithm.c
 *
 *  Created on: 01.12.2022
 *      Author: marcwech
 */

#include "DT_algorithm.h"

void DT_fillBuffer(list_t *DT_list, void *Buffer){

	if(DT_list == NULL || Buffer == NULL)
		return;

	list_node_t *dt_set;
	int count = 0;
	for(int i = 0; i < 15; i++){
		dt_set = list_at(DT_list, count);
		if(dt_set->next == NULL)
			count = 0;
		else
			count++;

		uint8_t tmp[64];
		if(dt_set->val->Counter == dt_set->val->MaxPackages){
			*tmp = (uint8_t*)(dt_set->val->Address + dt_set->val->Counter * 64);
			(uint8_t*)(Buffer + 64 * i) = tmp;
			list_remove(DT_list, dt_set);
		}else if(dt_set->val->Counter < dt_set->val->MaxPackages ){
			Buffer[64 * i] = (dt_set->val->Address + dt_set->val->Counter * 64);
			dt_set->val->Counter++;
		}
	}
	uint8_t STATUS_PACKAGE[64] = " *** STATUS BYTE ***";
	Buffer[15 * 64] = STATUS_PACKAGE;



}

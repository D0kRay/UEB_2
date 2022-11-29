/*
 * DT_runable.c
 *
 *  Created on: Nov 25, 2022
 *      Author: marcwech
 */

#include "DT_runable.h"


void DT_main(void){

	uint8_t string[50] = "Hello World!";
	uint32_t str_size = sizeof(string);

	DT_status status = DT_Init(string,str_size);

	if(status == 0)
		printf("\n\nError during data transmission!\nError Code: 0");
	if(status == 1000)
		printf("\n\nError during data transmission!\nError Code: 1000");

	status = DT_Start();

	if(status == 0)
		printf("\n\nError during data transmission!\nError Code: 0");
}

void Algorithmus(Dataset *Set, void* buf){

	for(int i = 0; i < 16; i++){
		if(Set[i].Address != NULL){
			*((char*)buf + (i * 64)) = *((char*)Set[i].Address + (Set[i].Counter * 64));


			Set[i].Counter++;

		}
	}
}


/*
 * USB_Packaging.c
 *
 *  Created on: Nov 17, 2022
 *      Author: marcwech
 *  This header file is part of the transmission protocol for the STM32 via USB_OTG_FS.
 *  Further documentation of the transmission protocol can be found in the documentation
 *  according to the "Masterprojektarbeit" of the WS2022 from Marc Wechselberger and Pascal Kirchhoff
 *  at the THU.
 */

#include "USB_Packaging.h"


// 			***** Function declarations for memory control *****

/* Description: Allocation of the memory used for storage temporary
 * transmission data of the according parameter.
 *
 * Parameter List:
 * "mem": address of the parameter for the storage of the allocated memory
 * "size": size of the needed memory in BYTE
 *
*/
void MEM_alloc_params(uint32_t *mem, uint8_t size){

	 if(mem == NULL){
		 mem = malloc(size);
	 }
	 return;
}

/* Description: Free the memory used for storage temporary
 * transmission data of the according parameter.
 *
 * Parameter List:
 * "mem": Memory which can be freed
 *
*/
void MEM_free_params(uint32_t *mem){
	if(mem == NULL) return;
	free(mem);
	return;
}

/* Description: Calculation of the size for the temporary memory
 *
 * Parameter List:
 * resolution: Resolution of the bits for the stored data (e.g. ADC)
 * samplerate: Sample rate of the data collection
 * acquisition_time: Duration of the acquisition
 *
*/
uint8_t MEM_calc_params(uint8_t resolution, uint8_t sample_rate, uint8_t acquistion_time){
	//Size of the memory equals the Resolution of the data times the sample rate and acquisition time.
	return resolution * sample_rate * acquistion_time;
}


int main(){}






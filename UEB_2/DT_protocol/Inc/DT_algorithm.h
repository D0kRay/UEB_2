/*
 * DT_algorithm.h
 *
 *  Created on: 01.12.2022
 *      Author: marcwech
 */

#ifndef INC_DT_ALGORITHM_H_
#define INC_DT_ALGORITHM_H_

#include "DT_list.h"

#define DT_BUFFER_SIZE 1024
#define DT_PACKAGE_SIZE 64
#define DT_PACKAGE_STATUS_SIZE 4
#define DT_PACKAGE_DATA_SIZE 60
#define DT_PACKAGE_NBR 16

#define StatusFlags_OS 0
#define ID_OS 1
#define Count_OS 2
#define MaxPackage_OS 3
#define Data_OS 4

#define DT_BUFFER_OS1 	0
#define DT_BUFFER_OS2 	64
#define DT_BUFFER_OS3 	128
#define DT_BUFFER_OS4 	192
#define DT_BUFFER_OS5 	256
#define DT_BUFFER_OS6 	320
#define DT_BUFFER_OS7 	384
#define DT_BUFFER_OS8 	448
#define DT_BUFFER_OS9 	512
#define DT_BUFFER_OS10 	576
#define DT_BUFFER_OS11 	640
#define DT_BUFFER_OS12 	704
#define DT_BUFFER_OS13 	768
#define DT_BUFFER_OS14 	832
#define DT_BUFFER_OS15 	896
#define DT_BUFFER_OS16 	960

#define DT_NO_DATA		0x40		// No Data for the Buffer
#define DT_TC			0x80		// Transmission Complete

#define DT_complete		0x80

uint8_t* DT_fillBuffer(list_t *DT_list);


#endif /* INC_DT_ALGORITHM_H_ */

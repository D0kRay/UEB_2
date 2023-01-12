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

#define DT_PRE1			224
#define DT_PRE2			225
#define DT_PRE3			226
#define DT_PRE4			227
#define DT_PRE5			228
#define DT_PRE6			229
#define DT_PRE7			230
#define DT_PRE8			231
#define DT_PRE9			232
#define DT_PRE10		233
#define DT_PRE11		234
#define DT_PRE12		235
#define DT_PRE13		236
#define DT_PRE14		237
#define DT_PRE15		238
#define DT_PRE16		239
#define DT_PRE17		240
#define DT_PRE18		241
#define DT_PRE19		242
#define DT_PRE20		243
#define DT_PRE21		244
#define DT_PRE22		245
#define DT_PRE23		246
#define DT_PRE24		247
#define DT_PRE25		248
#define DT_PRE26		249
#define DT_PRE27		250
#define DT_PRE28		251
#define DT_PRE29		252
#define DT_PRE30		253
#define DT_PRE31		254
#define DT_PRE32		255

void DT_fillBuffer(list_t *DT_list, uint8_t *buf);


#endif /* INC_DT_ALGORITHM_H_ */

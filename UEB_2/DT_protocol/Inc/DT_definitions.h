/*
 * DT_definitions.h
 *
 *  Created on: 01.12.2022
 *      Author: marcwech
 */

#ifndef INC_DT_DEFINITIONS_H_
#define INC_DT_DEFINITIONS_H_

#include "stdlib.h"

typedef enum{
	F_ON = 1,
	F_OFF = 0
}SendDataFlags;

typedef struct{
	uint8_t ID;					// 256 IDS for the parameter
	uint8_t Counter;			// Counter for the data package sent
	uint8_t MaxPackages;		// Number of packages to be sent
	void* Address;				// 32 Bit Address
	uint32_t Size;				// Size of the parameter in Byte
	uint8_t F_SendData;			// Flag for sending data: 1 = Send / 0 = not send
}Dataset;

typedef struct package{
	uint8_t P_Status[4];
	uint8_t P_Data[60];
}DataPackage;

#endif /* INC_DT_DEFINITIONS_H_ */

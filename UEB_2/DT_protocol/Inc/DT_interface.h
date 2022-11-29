/*
 * DT_interface.h
 *
 *  Created on: Nov 25, 2022
 *      Author: marcwech
 */

#ifndef INC_DT_INTERFACE_H_
#define INC_DT_INTERFACE_H_

// INCLUDES
#include <DT_includes.h>

#ifdef __cplusplus
 extern "C" {
#endif



// DEFINES

#define MAX_MEMORY_SIZE 1028	// Defines the MAX size of the transmission length

#define Package_Size 64			// Defined Size of a single transmission package in BYTE


// TYPE DEFINITIONS
typedef uint8_t DT_status;


// GLOBAL constructs

/**
 * @brief This is the basic structure for saving the DATA SET information
 * The variables which are saved for transmission are the:
 *
 * 		ID 			: uint8_t	= Defines the unique identifier for the DATA SET
 * 		Counter		: uint8_t	= Defines the counter of packages which are already collected
 * 		Address		: uint32_t	= Defines the first address of the data set
 * 		Size		: uint32_t	= Defines the size of the data set in BYTES
 *
 */
typedef struct{
	uint8_t ID;					// 20 character for the name of the parameter
	uint8_t Counter;			// 256 IDs for the parameter
	void* Address;				// 32 Bit Address
	uint32_t Size;				// Size of the parameter in Byte
}Dataset;

// Function definition

DT_status DT_Init(void* address, uint32_t size);
DT_status DT_Start();
void* DT_Init_Memory(uint32_t size);


#ifdef __cplusplus
 }
#endif





#endif /* INC_DT_INTERFACE_H_ */

/*
 * USB_Packaging.h
 *
 *  Created on: Nov 17, 2022
 *      Author: marcwech
 *
 *  This header file is part of the transmission protocol for the STM32 via USB_OTG_FS.
 *  Further documentation of the transmission protocol can be found in the documentation
 *  according to the "Masterprojektarbeit" of the WS2022 from Marc Wechselberger and Pascal Kirchhoff @THU.
 */

#ifndef USB_PACKAGING_H_
#define USB_PACKAGING_H_


#ifdef __cplusplus
 extern "C" {
#endif


//		*****		INCLUDES		*****

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>



// ***** Function declarations for memory control *****

// $$$ DOCUMENT_KEY TB INSERTED $$$ Transmission Parameter definition
/* This block to provides the functionalities for the memory management
 * for the transmission protocol via USB_OTG_FS of the STM32 controller.
 */


/**
 * @brief Allocation of the memory used for storage temporary
 * transmission data of the according parameter.
 *
 * @param mem:				Address of the parameter for the storage of the allocated memory
 * @param size:				Size of the needed memory in BYTE
 */
void MEM_alloc_params(uint32_t *mem, uint8_t size);


/**
 * @brief Free the memory used for storage temporary
 * transmission data of the according parameter.
 *
 * @param mem:				Memory which can be freed
 */
void MEM_free_params(uint32_t *mem);


/**
 * @brief Calculation of the size for the temporary memory
 *
 * @param resolution: 		Resolution of the bits for the stored data (e.g. ADC)
 * @param sample_rate:		Sample rate of the data collection
 * @param acquistion_time:	Duration of the acquisition
 * @return
 */
uint8_t MEM_calc_params(uint8_t resolution, uint8_t sample_rate, uint8_t acquistion_time);

#ifdef __cplusplus
 }
#endif


#endif /* USB_PACKAGING_H_ */

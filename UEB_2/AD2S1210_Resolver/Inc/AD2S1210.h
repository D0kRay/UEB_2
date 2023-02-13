/*
 * AD2S1210.h
 *
 *  Created on: Jan 30, 2023
 *      Author: pasik
 */

#ifndef INC_AD2S1210_H_
#define INC_AD2S1210_H_

#include "main.h"

typedef struct {
	uint8_t			dataBit;
	uint8_t			reserved;
	uint8_t			phase;
	uint8_t			hysteresis;
	uint8_t			encoderResolution;
	uint8_t			resolution;
} AD2S1210_ControlRegister;

typedef struct {
	uint16_t		GPIO_A0_PIN;				//
	GPIO_TypeDef	*GPIO_A0_PORT;				//
	uint16_t		GPIO_A1_PIN;				//
	GPIO_TypeDef	*GPIO_A1_PORT;				//
	uint16_t		GPIO_WR_FSYNC_PIN;			//
	GPIO_TypeDef	*GPIO_WR_FSYNC_PORT;		//
	uint16_t		GPIO_SAMPLE_PIN;			//
	GPIO_TypeDef	*GPIO_SAMPLE_PORT;			//
	uint8_t			resolverLosThreshold;		//
	uint8_t			resolverDosOverrangeThres;	//
	uint8_t			resolverDosMismatchThres;	//
	uint8_t			resolverDosResetMaxThres;	//
	uint8_t			resolverDosResetMinThres;	//
	uint8_t			resolverLotHighThres;		//
	uint8_t			resolverLotLowThres;		//
	uint8_t			resolverExcitationFreq;		//
	AD2S1210_ControlRegister	resolverControl;//
} AD2S1210_HandleTypeDef;

typedef struct {
	uint32_t		resolverRotationangle;		//
	uint32_t		resolverVelocity;			//
	uint32_t		resolverErrorFlags;			//
} AD2S1210_StatusTypeDef;


//Mode Select
#define RESOLVER_POSITION	0
#define	RESOLVER_VELOCITY	1
#define RESOLVER_CONFIG		2

/*
 * For further information please look at
 * https://www.analog.com/media/en/technical-documentation/data-sheets/ad2s1210.pdf
 * Page 20 and further
 * SPI Output of AD2S1210 is 24bit long word
 */
//Max SPI Clock -> 25 MHz see Page 24

//Resolver SPI Addresses
#define POSITION_D8_D15			0x80	//16bit Read only
#define POSITION_D0_D7			0x81	//16bit Read only
#define VELOCITY_D8_D15			0x82	//16bit Read only
#define VELOCITY_D0_D7			0x83	//16bit Read only
#define LOS_THRESHOLD			0x88	//8bit Read/write
#define DOS_OVERRANGE_THRES		0x89	//8bit Read/write
#define DOS_MISMATCH_THRES		0x8A	//8bit Read/write
#define DOS_RESET_MAX_THRES		0x8B	//8bit Read/write
#define DOS_RESET_MIN_THRES		0x8C	//8bit Read/write
#define LOT_HIGH_THRES			0x8D	//8bit Read/write
#define LOT_LOW_THRES			0x8E	//8bit Read/write
#define EXCITATION_FREQ			0x91	//8bit Read/write
#define CONTROL					0x92	//8bit Read/write
#define SOFT_RESET				0xF0	//8bit Write only
#define FAULT					0xFF	//8bit Read only
#define POSITION_VELOCITY_READ	0x00	//8bit Read only

void AD2S1210Initiate(SPI_HandleTypeDef *spi_handle, DMA_HandleTypeDef *dma_handle, AD2S1210_HandleTypeDef *resolver_handle, AD2S1210_StatusTypeDef *resolver_StatusType);
void AD2S1210SelectMode(uint8_t mode);
void AD2S1210SoftReset(void);
void WriteToAD2S1210(uint8_t address, uint8_t data);
void ReadFromAD2S1210(uint8_t mode, uint8_t address);
uint16_t getAngleOfResolver(void);
int16_t getAngleVelocityOfResolver(void);
uint8_t getErrorOfResolver(void);



#endif /* INC_AD2S1210_H_ */

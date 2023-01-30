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
	uint16_t		GPIO_A0_PIN;		//
	GPIO_TypeDef	*GPIO_A0_PORT;		//
	uint16_t		GPIO_A1_PIN;		//
	GPIO_TypeDef	*GPIO_A1_PORT;		//
	uint16_t		GPIO_WR_FSYNC_PIN;	//
	GPIO_TypeDef	*GPIO_WR_FSYNC_PORT;	//
	uint16_t		GPIO_SAMPLE_PIN;	//
	GPIO_TypeDef	*GPIO_SAMPLE_PORT;	//
} AD2S1210_HandleTypeDef;

typedef struct {
	uint32_t		resolverRotationangle;
	uint32_t		resolverVelocity;
	uint32_t		resolverErrorFlags;
} AD2S1210_StatusTypeDef;

//Mode Select
#define RESOLVER_POSITION	0
#define	RESOLVER_VELOCITY	1
#define RESOLVER_CONFIG		2


//Register Map
#define	POSITIONMSB		0x80
#define	POSITIONLSB		0x81
#define	VELOCITYMSB		0x82
#define	VELOCITYLSB		0x83
#define	LOSTHRES		0x88
#define	DOSORTHRES		0x89
#define	DOSMISTHRES		0x8A
#define	DOSRSTMXTHRES	0x8B
#define	DOSRSTMITHRES	0x8C
#define	LOTHITHRES		0x8D
#define	LOTLOTHRES		0x8E
#define	EXFREQUENCY		0x91
#define	CONTROL			0x92
#define	SOFTRESET		0xF0
#define	FAULT			0xFF
#define POS_VEL			0x00  //void register for normal read address

void AD2S1210Initiate(SPI_HandleTypeDef *spi_handle, DMA_HandleTypeDef *dma_handle, AD2S1210_HandleTypeDef *resolver_handle, AD2S1210_StatusTypeDef *resolver_StatusType);
void AD2S1210SelectMode(unsigned char mode);
void AD2S1210SoftReset(void);
void WriteToAD2S1210(unsigned char address, unsigned char data);
void ReadFromAD2S1210(unsigned char mode, unsigned char address, unsigned char * buf);
//void GetAngleOfResolver();
//void GetVelocityOfResolver();
//void GetErrorOfResolver();



#endif /* INC_AD2S1210_H_ */

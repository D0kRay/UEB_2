/*
 * AD2S1210.c
 *
 *  Created on: Jan 30, 2023
 *      Author: pasik
 */

#include "AD2S1210.h"

SPI_HandleTypeDef *hspi_AD2S1210;
DMA_HandleTypeDef *hdma_spi_AD2S1210;
AD2S1210_HandleTypeDef *resolver_HandleType;
AD2S1210_StatusTypeDef *resolver_StatusType;



void SET_A0(void)
{
	HAL_GPIO_WritePin(resolver_HandleType->GPIO_A0_PORT, resolver_HandleType->GPIO_A0_PIN, GPIO_PIN_SET);
}

void CLR_A0(void)
{
	HAL_GPIO_WritePin(resolver_HandleType->GPIO_A0_PORT, resolver_HandleType->GPIO_A0_PIN, GPIO_PIN_RESET);
}

void SET_A1(void)
{
	HAL_GPIO_WritePin(resolver_HandleType->GPIO_A1_PORT, resolver_HandleType->GPIO_A1_PIN, GPIO_PIN_SET);
}

void CLR_A1(void)
{
	HAL_GPIO_WritePin(resolver_HandleType->GPIO_A1_PORT, resolver_HandleType->GPIO_A1_PIN, GPIO_PIN_RESET);
}

void SET_WR(void)
{
	HAL_GPIO_WritePin(resolver_HandleType->GPIO_WR_FSYNC_PORT, resolver_HandleType->GPIO_WR_FSYNC_PIN, GPIO_PIN_SET);
}

void CLR_WR(void)
{
	HAL_GPIO_WritePin(resolver_HandleType->GPIO_WR_FSYNC_PORT, resolver_HandleType->GPIO_WR_FSYNC_PIN, GPIO_PIN_RESET);
}

//void SET_CS(void)
//{
//	HAL_GPIO_WritePin(resolver_statusType->GPIO_, resolver_statusType->GPIO_A0_PIN, GPIO_PIN_SET);
//}
//
//void CLR_CS(void)
//{
//	HAL_GPIO_WritePin(resolver_statusType->GPIO_A0_PORT, resolver_statusType->GPIO_A0_PIN, GPIO_PIN_RESET);
//}

//void SET_SCLK(void)
//{
//	HAL_GPIO_WritePin(resolver_statusType->GPIO_A0_PORT, resolver_statusType->GPIO_A0_PIN, GPIO_PIN_SET);
//}
//
//void CLR_SCLK(void)
//{
//	HAL_GPIO_WritePin(resolver_statusType->GPIO_A0_PORT, resolver_statusType->GPIO_A0_PIN, GPIO_PIN_RESET);
//}

void SET_SPL(void)
{
	HAL_GPIO_WritePin(resolver_HandleType->GPIO_SAMPLE_PORT, resolver_HandleType->GPIO_SAMPLE_PIN, GPIO_PIN_SET);
}

void CLR_SPL(void)
{
	HAL_GPIO_WritePin(resolver_HandleType->GPIO_SAMPLE_PORT, resolver_HandleType->GPIO_SAMPLE_PIN, GPIO_PIN_RESET);
}

void SPIRead(unsigned char count, unsigned char *buf)
{
	HAL_SPI_Receive_DMA(hspi_AD2S1210, buf, count);
}

void SPIWrite(unsigned char count, unsigned char *buf)
{
	HAL_SPI_Transmit(hspi_AD2S1210, buf, count, 100);
//	HAL_SPI_Transmit_IT(hspi_AD2S1210, buf, count);
}


void AD2S1210Initiate(SPI_HandleTypeDef *spi_handle, DMA_HandleTypeDef *dma_handle, AD2S1210_HandleTypeDef *resolver_handle, AD2S1210_StatusTypeDef *resolver_StatusType)
{
//RESET->0 initially
	hspi_AD2S1210 = spi_handle;
	hdma_spi_AD2S1210 = dma_handle;
	resolver_HandleType = resolver_handle;
//	CLR_RESET();
//	SET_SPL();
//	delay(1);
//	SET_RESET();
//	delay(2);
//	CLR_SPL();
//	delay(1);
//	SET_SPL();
}

void AD2S1210SelectMode(unsigned char mode)
{
	if (mode==RESOLVER_POSITION)
	{
		CLR_A0();
		CLR_A1();
		HAL_Delay(1);		//Normal Mode position output
	}
	else if (mode==RESOLVER_VELOCITY)
	{
		CLR_A0();
		SET_A1();
		HAL_Delay(1);		//Normal Mode velocity output
	}
	else if (mode==RESOLVER_CONFIG)
	{
		SET_A0();
		SET_A1();
		HAL_Delay(1);		//Configuration Mode
	}
}

void WriteToAD2S1210(unsigned char address, unsigned char data)
{
	unsigned	char	buf;

	//write control register address
	buf = address;

//	SET_SCLK();
//	HAL_Delay(1);
//	SET_CS();
//	HAL_Delay(1);
//	CLR_CS();
	HAL_Delay(1);

	SET_WR();
	HAL_Delay(1);
	CLR_WR();
	HAL_Delay(1);

	SPIWrite(1,&buf);

	SET_WR();
	HAL_Delay(1);
//	SET_CS();
	//write control register address

	//write control register data
	buf = data;

//	SET_SCLK();
//	HAL_Delay(1);
//	SET_CS();
//	HAL_Delay(1);
//	CLR_CS();
	HAL_Delay(1);

	SET_WR();
	HAL_Delay(1);
	CLR_WR();
	HAL_Delay(1);

	SPIWrite(1,&buf);

	SET_WR();
	HAL_Delay(1);
//	SET_CS();
	//write control register data
}

void ReadFromAD2S1210(unsigned char mode, unsigned char address, unsigned char * buf)
{

	if (mode==RESOLVER_CONFIG)
	{

		//write control register address
		buf[0] = address;

//		SET_SCLK();
//		HAL_Delay(1);
//		SET_CS();
//		HAL_Delay(1);
//		CLR_CS();
		HAL_Delay(1);

		SET_WR();
		HAL_Delay(1);
		CLR_WR();
		HAL_Delay(1);

		SPIWrite(1,buf);

		SET_WR();
		HAL_Delay(1);
//		SET_CS();
		//write control register address


		//read 1-byte register
//		SET_SCLK();

//		SET_CS();
		SET_WR();
		HAL_Delay(1);

//		CLR_CS();
//		HAL_Delay(1);

//		CLR_SCLK();
//		HAL_Delay(1);

		CLR_WR();
		HAL_Delay(1);

		SPIRead(1,buf);

		SET_WR();
		HAL_Delay(1);

//		SET_CS();
		//read 1-byte register
	}
	else if (mode==RESOLVER_POSITION||mode==RESOLVER_VELOCITY)
	{
		SET_SPL();
		HAL_Delay(1);
		CLR_SPL();
		HAL_Delay(5);

		//read 3-byte register
//		SET_SCLK();

//		SET_CS();
		SET_WR();
		HAL_Delay(1);

//		CLR_CS();
//		HAL_Delay(1);

//		CLR_SCLK();
//		HAL_Delay(1);

		CLR_WR();
		HAL_Delay(1);

		SPIRead(3,buf);		//read data register

		SET_WR();
		HAL_Delay(1);

//		SET_CS();
		//read 3-byte register


	}
}

void AD2S1210SoftReset(void)
{
	unsigned char buf=	SOFTRESET;
	SPIWrite (1,&buf);	  	//soft reset
	HAL_Delay(10);
}

//void GetAngleOfResolver()
//{
//
//}
//void GetVelocityOfResolver()
//{
//
//}
//void GetErrorOfResolver()
//{
//
//}

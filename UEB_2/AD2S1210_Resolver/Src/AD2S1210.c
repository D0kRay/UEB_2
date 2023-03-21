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
uint8_t resolver_buffer[4];


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

void SPIRead(uint8_t count, uint8_t *buf)
{
	HAL_SPI_Receive_DMA(hspi_AD2S1210, buf, count);
}

void SPIWrite(uint8_t count, uint8_t *buf)
{
	HAL_SPI_Transmit(hspi_AD2S1210, buf, count, 10);
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

void AD2S1210SelectMode(uint8_t mode)
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

void WriteToAD2S1210(uint8_t address, uint8_t data)
{
	uint8_t buf;

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

void ReadFromAD2S1210(uint8_t mode, uint8_t address)
{

	if (mode==RESOLVER_CONFIG)
	{
		AD2S1210SelectMode(RESOLVER_CONFIG);
		//write control register address
		resolver_buffer[0] = address;

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

		SPIWrite(1,resolver_buffer);
		resolver_buffer[0] = 0xFF;
		SET_WR();
		HAL_Delay(1);
//		SET_CS();
		//write control register address

//		HAL_SPI_TransmitReceive_DMA(hspi_AD2S1210, &address, buf, address);
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

		SPIRead(1,resolver_buffer);

		SET_WR();
		HAL_Delay(1);

//		SET_CS();
		//read 1-byte register
	}
	else if (mode==RESOLVER_POSITION)
	{
		AD2S1210SelectMode(RESOLVER_POSITION);
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

		SPIRead(3,resolver_buffer);		//read data register

		SET_WR();
		HAL_Delay(1);

//		SET_CS();
		//read 3-byte register


	}
	else if (mode==RESOLVER_VELOCITY)
	{
		AD2S1210SelectMode(RESOLVER_VELOCITY);
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

		SPIRead(3,resolver_buffer);		//read data register

		SET_WR();
		HAL_Delay(1);

//		SET_CS();
		//read 3-byte register


	}
}

void AD2S1210SoftReset(void)
{
	unsigned char buf = SOFT_RESET;
	SPIWrite (1,&buf);	  	//soft reset
	HAL_Delay(10);
}
/*
 * Table 22. Control Register Bit Descriptions
Bit Description
D7 Address/data bit
D6 Reserved; set to 1
D5 Phase lock range
 0 = 360°, 1 = ±44°
D4 0 = disable hysteresis, 1 = enable hysteresis
D3 Set Encoder Resolution EnRES1
D2 Set Encoder Resolution EnRES0
D1 Set Resolution RES1
D0 Set Resolution RES0
 */

uint8_t getAD2S1210ControlRegister(AD2S1210_ControlRegister controlregister)
{
	return (controlregister.dataBit << 7 | controlregister.reserved << 6 | controlregister.phase << 5 | controlregister.hysteresis << 4 | controlregister.encoderResolution << 2 | controlregister.resolution);
}


void AD2S1210Setup(AD2S1210_HandleTypeDef *resolver_handle)
{
	AD2S1210SelectMode(RESOLVER_CONFIG);
	WriteToAD2S1210(LOS_THRESHOLD, resolver_handle->resolverLosThreshold);
//	HAL_Delay(1);
	WriteToAD2S1210(DOS_OVERRANGE_THRES, resolver_handle->resolverDosOverrangeThres);
//	HAL_Delay(1);
	WriteToAD2S1210(DOS_MISMATCH_THRES, resolver_handle->resolverDosMismatchThres);
//	HAL_Delay(1);
	WriteToAD2S1210(DOS_RESET_MAX_THRES, resolver_handle->resolverDosResetMaxThres);
//	HAL_Delay(1);
	WriteToAD2S1210(DOS_RESET_MIN_THRES, resolver_handle->resolverDosResetMinThres);
//	HAL_Delay(1);
	WriteToAD2S1210(LOT_HIGH_THRES, resolver_handle->resolverLotHighThres);
//	HAL_Delay(1);
	WriteToAD2S1210(LOT_LOW_THRES, resolver_handle->resolverLotLowThres);
//	HAL_Delay(1);
	WriteToAD2S1210(EXCITATION_FREQ, resolver_handle->resolverExcitationFreq);
//	HAL_Delay(1);
	uint8_t control = getAD2S1210ControlRegister(resolver_handle->resolverControl);
	WriteToAD2S1210(CONTROL, control);
	AD2S1210SelectMode(RESOLVER_POSITION);
}
/*
	The angular position data format is
	unsigned binary, with all 0s corresponding to 0 degrees and all
	1s corresponding to 360 degrees − l LSB.
	The angular velocity data
	format is twos complement binary, with the MSB representing the
	rotation direction.
	Bit 7 through Bit 0 correspond to the fault
	information.
 */
uint16_t AD2S1210getAngle(void)
{
	AD2S1210SelectMode(RESOLVER_POSITION);
	ReadFromAD2S1210(RESOLVER_POSITION, POSITION_VELOCITY_READ);
	uint16_t angle = (resolver_buffer[0]<<8) & resolver_buffer[1];
	if(resolver_HandleType->resolverControl.hysteresis){
		angle = angle >> (16 - resolver_HandleType->resolverControl.resolution);
	}
	return angle;
}

int16_t AD2S1210getAngleVelocity(void)
{
	AD2S1210SelectMode(RESOLVER_VELOCITY);
	ReadFromAD2S1210(RESOLVER_VELOCITY, POSITION_VELOCITY_READ);
	uint16_t velo = (resolver_buffer[0]<<8) & resolver_buffer[1];
	uint16_t velonegative = resolver_buffer[0] & 0x80;
	velo = velo >> (16 - resolver_HandleType->resolverControl.resolution);
	if(velo & 0x8000) {
		velonegative = (0xFFFF >> resolver_HandleType->resolverControl.resolution) << resolver_HandleType->resolverControl.resolution;
		velo |= velonegative;
	}
	return (int16_t) velo;
}

uint8_t AD2S1210getError(void)
{
	AD2S1210SelectMode(RESOLVER_POSITION);
	ReadFromAD2S1210(RESOLVER_VELOCITY, POSITION_VELOCITY_READ);
	return resolver_buffer[3];
}

void getAD2S1210Data(AD2S1210_StatusTypeDef *resolverStatusVar)
{
	resolverStatusVar->resolverRotationangle = AD2S1210getAngle();
	resolverStatusVar->resolverVelocity = AD2S1210getAngleVelocity();
	resolverStatusVar->resolverErrorFlags = AD2S1210getError();
}





















//switch (chan->type) {
//	case IIO_ANGL:
//		pos = be16_to_cpup((__be16 *)st->rx);
//		if (st->hysteresis)
//			pos >>= 16 - st->resolution;
//		*val = pos;
//		ret = IIO_VAL_INT;
//		break;
//	case IIO_ANGL_VEL:
//		negative = st->rx[0] & 0x80;
//		vel = be16_to_cpup((__be16 *)st->rx);
//		vel >>= 16 - st->resolution;
//		if (vel & 0x8000) {
//			negative = (0xffff >> st->resolution) << st->resolution;
//			vel |= negative;
//		}
//		*val = vel;
//		ret = IIO_VAL_INT;
//		break;

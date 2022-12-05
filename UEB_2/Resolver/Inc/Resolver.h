/*
 * Resolver.h
 *
 *  Created on: Dec 1, 2022
 *      Author: Pascal
 */

#ifndef INC_RESOLVER_H_
#define INC_RESOLVER_H_

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
#define EXCITATIN_FREQ			0x91	//8bit Read/write
#define CONTROL					0x92	//8bit Read/write
#define SOFT_RESET				0xF0	//8bit Write only
#define FAULT					0xFF	//8bit Read only



#endif /* INC_RESOLVER_H_ */

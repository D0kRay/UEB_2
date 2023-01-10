/*
 * Interface_Decoder.h
 *
 *  Created on: Dec 1, 2022
 *      Author: pasik
 */

#ifndef INC_INTERFACE_DECODER_H_
#define INC_INTERFACE_DECODER_H_

#include "DataTransmission_decoder.h"
#include "Peripheral_decoder.h"
#include "UEB_decoder.h"

/*
 * float sind 4 Byte groß auf dem STM32
 * USB Paket hat 64 Byte Speicherplatz
 * Aktuelle Messwerte (U,I) sind 9 stk. -> 9 * 4 = 36 Byte
 */

#define UEB_INIT 0
#define UEB_INIT_FINISH 1
#define UEB_RUN 2
#define UEB_STOP 3

//UEB structs
typedef struct {
  uint8_t	status;				//the actual status of the UEB: 0 = INIT; 1 = RUN; 2 = STOP; 3 = NOPARAM; 4 = ERROR...
  uint8_t	errorstatus;		//see main.c error defines
  float_t	vccvoltage;			//
  float_t	outvoltage;			//
  float_t	frequency;			//
  uint8_t	thirdharmonic;		//
  uint8_t	softstart;			//
  float_t	softstartduration;	//
  float_t	maxcurrent;			//
  uint32_t	averagenum;			//
  float_t	pwmfrequency;		//
  uint8_t	rotationdirection;
} UEB_StatusType;


typedef struct {
  uint8_t	status;				//the actual status of the UEB: 0 = INIT; 1 = RUN; 2 = STOP; 3 = NOPARAM; 4 = ERROR...
  float_t	bufferSum_IHB1;
  float_t	bufferSum_IHB2;
  float_t	bufferSum_IHB3;
  float_t	bufferAverage_IHB1;
  float_t	bufferAverage_IHB2;
  float_t	bufferAverage_IHB3;
  float_t	current_IHB1;
  float_t	current_IHB2;
  float_t	current_IHB3;
  int32_t	rotvelo;
} UEB_MeasuresType;


//Commands to get and set Status of the UEB
static const char UEBREADY[] = "?";								//returns the actual state of the UEB

//Commandtree
/*
 * UEB:--
 * 		|
 * 	   SOFTSTART------------------------>ENAB
 * 		|		   						|
 * 		|								 DUR-->VALUE
 * 		|
 * 	  CONFIG---------------------------->TRDHARM
 * 	  	|								|
 * 	  	|								ROT
 * 	  	|
 *		PARAM--------------------------->VCC-->VALUE
 *		|								|
 *		|								VOUT-->VALUE
 *		|								|
 *		|								FREQ-->VALUE
 *		|								|
 *		|								CURR-->VALUE
 *		|
 *		SYS----------------------------->ENAB
 *
 *	PERI:--
 *		  |
 *		  ADC--------------------------->SYST
 *		  |								|
 *		  |								CH-->FREQ
 *		  |									|
 *		  |									ENAB
 *		  |
 *		  RES--------------------------->DEV-->STAT-->ENAB
 *		  |										|
 *		  |										CONFIG-->THRES
 *		  |												|
 *		  |												LOS
 *		  |												|
 *		  |												DOS-->OVERRANGE
 *		  |												|	 |
 *		  |												|	 MISMATCH
 *		  |												|	 |
 *		  |												|	 RESET-->MIN
 *		  |												|			|
 *		  |												|			MAX
 *		  |												|
 *		  |												LOT-->HIGH
 *		  |												|	 |
 *		  |												|	  LOW
 *		  |												|
 *		  |												EX-->FREQ
 *		  |												|
 *		  |												PLR-->ENAB
 *		  |												|
 *		  |												HYST-->ENAB
 *		  |												|
 *		  |												RES-->ENCOD
 *		  |													 |
 *		  |													  RDC
 *		  |
 *		  INKR
 *
 *	DT TODO TODO
 *
 *
 *
 */
//
static const char DELIMITER_FULLMESSAGE[] = ";";
static const char DELIMITER_PARTMESSAGE[] = ":";
//static const char UEB_VERSION[] = "UEB_Version_1.0";
//
//static const char UEB_IN_CONFIG_MODE[] = "UEB_in_config_mode";
//static const char UEB_CONFIG_SAVED[] = "UEB_config_saved";

//Functions
void createStatusEvent();
void createEvent(EventQueue **queue, EVTClass class, USB_Messages eventMessage);
void getMessage(EventQueue **queue);
void getNewStatus(UEB_StatusType *uebstatus);

void provideStatus(UEB_StatusType *uebstatus);
void provideEventQueues(EventQueue *main_queue, EventQueue *usb_queue, EventQueue *datatransmission_queue);
void setStatus();

#endif /* INC_INTERFACE_DECODER_H_ */

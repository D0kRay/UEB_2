/*
 * Decoder_Interface.c
 *
 *  Created on: Dec 1, 2022
 *      Author: pasik
 */


#include "usb_device.h"
#include "com_provider.h"
#include "Decoder_Interface.h"
#include "EventQueue.h"


char tokenizebuffer[BUFFERSIZE];
char* token;

UEB_StatusType* pUEB_status;
EventQueue* pMainEventQueue;
EventQueue* pUSBEventQueue;
EventQueue* pDataTransmissionEventQueue;

uint8_t receivebuffer[BUFFERSIZE];



float getFloatOfMessage(char *message)
{
//	char *s;
//	s = strstr(message, commandstring);
//	s =  s + strlen(commandstring);
	return (float_t)strtod(message, NULL);
}

uint16_t getIntergerOfFloat(float f)
{
	return truncf(f);
}

uint16_t getDecimalOfFloat(float f)
{
	uint16_t integerPart = 0;
	integerPart = getIntergerOfFloat(f);
	float temp = (f - integerPart) * 100;
	return (uint16_t)temp;
}
//TODO mit strstr stelle des nächsten strichpunkt suchen und message bis dahin abschneiden
void decodeUEBMessage(char* message)
{
	if(strstr(message, UEB) != NULL) {
		message = strstr(message, UEB) + strlen(UEB);
		if(strstr(message, SOFTSTART) != NULL){
			message = strstr(message, SOFTSTART) + strlen(SOFTSTART);
			if(strstr(message, D_ENABLE) != NULL) {
				message = strstr(message, D_ENABLE) + strlen(D_ENABLE);
				message = strstr(message, DELIMITER_PARTMESSAGE) + strlen(DELIMITER_PARTMESSAGE);
				pUEB_status->softstart = getFloatOfMessage(message);
			} else if (strstr(message, DURATION) != NULL) {
				message = message + strlen(DURATION) + 1;
				if (strstr(message, VALUE) != NULL) {
					pUEB_status->softstartduration = getFloatOfMessage(message);
				}

			}
		} else if(strstr(message, CONFIGURATION) != NULL){
			message = message + strlen(CONFIGURATION);
			if(strstr(message, TRDHARMONIC) != NULL) {
				pUEB_status->thirdharmonic = getFloatOfMessage(message);
			} else if (strstr(message, ROTATION) != NULL) {
				pUEB_status->rotationdirection = getFloatOfMessage(message);
			}

		} else if(strstr(message, PARAMETER) != NULL){
			message = message + strlen(PARAMETER);
			if(strstr(message, VCC) != NULL) {
				message = message + strlen(VCC);
				if (strstr(message, VALUE) != NULL) {
					pUEB_status->vccvoltage = getFloatOfMessage(message);
				}

			} else if (strstr(message, VOUT) != NULL) {
				message = message + strlen(VOUT);
				if (strstr(message, VALUE) != NULL) {
					pUEB_status->outvoltage = getFloatOfMessage(message);
				}

			} else if (strstr(message, FREQUENCY) != NULL) {
				message = message + strlen(FREQUENCY);
				if (strstr(message, VALUE) != NULL) {
					pUEB_status->frequency = getFloatOfMessage(message);
				}

			}else if (strstr(message, CURRENT) != NULL) {
				message = message + strlen(CURRENT);
				if (strstr(message, VALUE) != NULL) {
					pUEB_status->maxcurrent = getFloatOfMessage(message);
				}

			}

		} else if(strstr(message, SYSTEM) != NULL){
			message = message + strlen(SYSTEM);
			if(strstr(message, D_ENABLE) != NULL) {
				pUEB_status->status = getFloatOfMessage(message);
				createStatusEvent();
			}
		}
	} else {
		TransmitBuffer((uint8_t*)"Error: Wrong Command\r", strlen("Error: Wrong Command\r"));
	}
}

void decodePeripheralMessage(char* message)
{
	if(strstr(message, PERIPHERAL) != NULL) {
		message = message + strlen(UEB);
		if(strstr(message, ADC) != NULL){
			message = message + strlen(UEB);
			if(strstr(message, SYSTEM) != NULL) {
				message = message + strlen(UEB);
			} else if (strstr(message, CHANNEL) != NULL) {
				message = message + strlen(UEB);
				if (strstr(message, FREQUENCY) != NULL) {

				} else if (strstr(message, D_ENABLE) != NULL) {

				}

			}
		} else if(strstr(message, RES) != NULL){
			message = message + strlen(UEB);
			if(strstr(message, DEVICE) != NULL) {
				message = message + strlen(UEB);
				if (strstr(message, STAT) != NULL) {
					message = message + strlen(UEB);
					if (strstr(message, D_ENABLE) != NULL) {

					}

				} else if (strstr(message , CONFIG) != NULL) {
					message = message + strlen(UEB);
					if (strstr(message, THRES) != NULL) {
						message = message + strlen(UEB);
					} else if (strstr(message, LOS) != NULL) {

					} else if (strstr(message, DOS) != NULL) {
						message = message + strlen(UEB);
						if (strstr(message, OVERRANGE) != NULL) {

						} else if (strstr(message, MISMATCH) != NULL) {

						} else if (strstr(message, D_RESET) != NULL) {
							message = message + strlen(UEB);
							if (strstr(message, MIN) != NULL) {

							} else if (strstr(message, MAX) != NULL) {

							}
						}
					} else if (strstr(message, LOT) != NULL) {
						message = message + strlen(UEB);
						if (strstr(message, MIN) != NULL) {

						} else if (strstr(message, MAX) != NULL) {

						}

					} else if (strstr(message, EXCITATION) != NULL) {
						message = message + strlen(UEB);
						if (strstr(message, FREQUENCY) != NULL) {

						}
					} else if (strstr(message, PHASELOCKRANGE) != NULL) {
						message = message + strlen(UEB);
						if (strstr(message, D_ENABLE) != NULL) {

						}
					} else if (strstr(message, HYSTERESIS) != NULL) {
						message = message + strlen(UEB);
						if (strstr(message, D_ENABLE) != NULL) {

						}
					} else if (strstr(message, RES) != NULL) {
						message = message + strlen(UEB);
						if (strstr(message, ENCODER) != NULL) {

						} else if (strstr(message, RDC) != NULL) {

						}

					}
				}
			}
		} else if(strstr(message, INKREMENTAL) != NULL){
			message = message + strlen(UEB);
			if(strstr(message, VCC) != NULL) {
				message = message + strlen(UEB);
				if (strstr(message, VALUE) != NULL) {

				}
			} else if (strstr(message, VOUT) != NULL) {
				message = message + strlen(UEB);
				if (strstr(message, VALUE) != NULL) {

				}
			} else if (strstr(message, FREQUENCY) != NULL) {
				message = message + strlen(UEB);
				if (strstr(message, VALUE) != NULL) {

				}
			}
		}
	} else {
		TransmitBuffer((uint8_t*)"Error: Wrong Command\r", strlen("Error: Wrong Command\r"));
	}
}

void decodeDataTransmissionMessage(char* message)
{
	if(strstr(message, UEB) != NULL) {
		message = message + strlen(UEB);
		if(strstr(message, SOFTSTART) != NULL){
			message = message + strlen(UEB);
			if(strstr(message, D_ENABLE) != NULL) {

			} else if (strstr(message, FREQUENCY) != NULL) {
				if (strstr(message, VALUE) != NULL) {

				}
			}
		} else if(strstr(message, CONFIGURATION) != NULL){
			message = message + strlen(UEB);
			if(strstr(message, TRDHARMONIC) != NULL) {

			} else if (strstr(message, ROTATION) != NULL) {

			}
		} else if(strstr(message, PARAMETER) != NULL){
			message = message + strlen(UEB);
			if(strstr(message, VCC) != NULL) {
				message = message + strlen(UEB);
				if (strstr(message, VALUE) != NULL) {

				}
			} else if (strstr(message, VOUT) != NULL) {
				message = message + strlen(UEB);
				if (strstr(message, VALUE) != NULL) {

				}
			} else if (strstr(message, FREQUENCY) != NULL) {
				message = message + strlen(UEB);
				if (strstr(message, VALUE) != NULL) {

				}
			}
		}
	} else {
		TransmitBuffer((uint8_t*)"Error: Wrong Command\r", strlen("Error: Wrong Command\r"));
	}
}

void transmit_info()
{
	uint8_t* string;
	string = malloc(1024);
	memset(string, '\0', 1024);

	snprintf((char*)string, 1024, "%s= %d;%s= %d.%d;%s= %d.%d;%s= %d.%d;%s= %d;%s= %d;%s= %d;%s= %d.%d;%s= %d.%d\r",
			STAT, pUEB_status->status, VCC, getIntergerOfFloat(pUEB_status->vccvoltage), getDecimalOfFloat(pUEB_status->vccvoltage),
			VOUT, getIntergerOfFloat(pUEB_status->outvoltage), getDecimalOfFloat(pUEB_status->outvoltage),
			FREQUENCY, getIntergerOfFloat(pUEB_status->frequency), getDecimalOfFloat(pUEB_status->frequency),
			ROTATION, pUEB_status->rotationdirection, TRDHARMONIC, pUEB_status->thirdharmonic, SOFTSTART, pUEB_status->softstart,
			DURATION, getIntergerOfFloat(pUEB_status->softstartduration), getDecimalOfFloat(pUEB_status->softstartduration),
			CURRENT, getIntergerOfFloat(pUEB_status->maxcurrent), getDecimalOfFloat(pUEB_status->maxcurrent));
	TransmitBuffer(string, strlen((char*)string));
	free(string);
}

void decodeMessage(char *message)
{
	if (strstr(message, UEB) != NULL) {
		decodeUEBMessage(message);
	} else if (strstr(message, PARAMETER) != NULL) {
		decodePeripheralMessage(message);
	} else if (strstr(message, DATATRANSMISSION) != NULL) {
		createInfoEvent();
		//decodeDataTransmissionMessage(message);
	}  else if (strstr(message, UEBREADY) != NULL) {
		transmit_info();
	} else {
		TransmitBuffer((uint8_t*)"Error: Wrong Command\r", strlen("Error: Wrong Command\r"));
	}

}

void getMessage()
{
	if(is_Receive_Complete()) {
		get_Receive_Message(receivebuffer, BUFFERSIZE);
		strcpy(tokenizebuffer, (const char *)receivebuffer);
		uint8_t i = 0;
		token = strtok(tokenizebuffer, DELIMITER_FULLMESSAGE);
		while (token != NULL) {
			decodeMessage(token);
			token = strtok(NULL, DELIMITER_FULLMESSAGE);
			i++;
		}
	}
}

void provideStatus(UEB_StatusType *uebstatus)
{
	pUEB_status = uebstatus;
}

void provideEventQueues(EventQueue *main_queue, EventQueue *usb_queue, EventQueue *datatransmission_queue)
{
	pMainEventQueue = main_queue;
	pUSBEventQueue = usb_queue;
	pDataTransmissionEventQueue = datatransmission_queue;
}

void createStatusEvent()
{
	Event *evt = malloc (sizeof(Event));
	(*evt).class = 0;
	(*evt).message = 0;
	addEvent(&pMainEventQueue, evt);
}

void createInfoEvent()
{
	Event *evt = malloc (sizeof(Event));
	(*evt).class = 0;
	(*evt).message = 1;
	addEvent(&pDataTransmissionEventQueue, evt);
}



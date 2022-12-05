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



void getFloatOfMessage(char *message, char *commandstring, float value)
{
	char *s;
	s = strstr(message, commandstring);      // search for searchedstring in message
	s =  s + strlen(commandstring);// index of s in buff can be found by pointer subtraction
	value = (float_t)strtod(s, NULL);
}

void decodeUEBMessage(char* message)
{
	if(strstr(message, UEB) != NULL) {
		message = message + strlen(UEB)+1;
		if(strstr(message, SOFTSTART) != NULL){
			message = message + strlen(SOFTSTART)+1;
			if(strstr(message, D_ENABLE) != NULL) {
				getFloatOfMessage(message, (char *)D_ENABLE, pUEB_status->softstart);
			} else if (strstr(message, DURATION) != NULL) {
				if (strstr(message, VALUE) != NULL) {
					getFloatOfMessage(message, (char *)VALUE, pUEB_status->softstartduration);
				}

			}
		} else if(strstr(message, CONFIGURATION) != NULL){
			message = message + strlen(CONFIGURATION);
			if(strstr(message, TRDHARMONIC) != NULL) {
				getFloatOfMessage(message, (char *)TRDHARMONIC, pUEB_status->thirdharmonic);
			} else if (strstr(message, ROTATION) != NULL) {
				getFloatOfMessage(message, (char *)ROTATION, pUEB_status->rotationdirection);
			}

		} else if(strstr(message, PARAMETER) != NULL){
			message = message + strlen(PARAMETER);
			if(strstr(message, VCC) != NULL) {
				message = message + strlen(VCC);
				if (strstr(message, VALUE) != NULL) {
					getFloatOfMessage(message, (char *)VALUE, pUEB_status->vccvoltage);
				}

			} else if (strstr(message, VOUT) != NULL) {
				message = message + strlen(VOUT);
				if (strstr(message, VALUE) != NULL) {
					getFloatOfMessage(message, (char *)VALUE, pUEB_status->outvoltage);
				}

			} else if (strstr(message, FREQUENCY) != NULL) {
				message = message + strlen(FREQUENCY);
				if (strstr(message, VALUE) != NULL) {
					getFloatOfMessage(message, (char *)VALUE, pUEB_status->frequency);
				}

			}else if (strstr(message, CURRENT) != NULL) {
				message = message + strlen(CURRENT);
				if (strstr(message, VALUE) != NULL) {
					getFloatOfMessage(message, (char *)VALUE, pUEB_status->maxcurrent);
				}

			}

		} else if(strstr(message, SYSTEM) != NULL){
			message = message + strlen(SYSTEM);
			if(strstr(message, D_ENABLE) != NULL) {
				getFloatOfMessage(message, (char *)D_ENABLE, pUEB_status->status);
				createStatusEvent();
			}
		}
	} else {
		setBuffer((uint8_t*)"Error: Wrong Command\r", strlen("Error: Wrong Command\r"));
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
		setBuffer((uint8_t*)"Error: Wrong Command\r", strlen("Error: Wrong Command\r"));
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
		setBuffer((uint8_t*)"Error: Wrong Command\r", strlen("Error: Wrong Command\r"));
	}
}

void transmit_info()
{
	uint8_t* string;
	string = malloc(1024);
	memset(string, '\0', 1024);

	snprintf((char*)string, 1024, "%s%d;%s%d;%s%d;%s%d;%s%d;%s%d;%s%d;%s%d;%s%d\r",
			STAT, (uint8_t)pUEB_status->status, VCC, (uint8_t)pUEB_status->vccvoltage,
			VOUT, (uint8_t)pUEB_status->outvoltage, FREQUENCY, (uint8_t)pUEB_status->frequency,
			ROTATION, (uint8_t)pUEB_status->rotationdirection, TRDHARMONIC, (uint8_t)pUEB_status->thirdharmonic,
			SOFTSTART, (uint8_t)pUEB_status->softstart, DURATION, (uint8_t)pUEB_status->softstartduration,
			CURRENT, (uint8_t)pUEB_status->maxcurrent);
	setBuffer(string, strlen((char*)string));
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
	*pUEB_status = *uebstatus;
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
	(*evt).source = 0;
	addEvent(&pMainEventQueue, evt);
}

void createInfoEvent()
{
	Event *evt = malloc (sizeof(Event));
	(*evt).class = 0;
	(*evt).source = 1;
	addEvent(&pDataTransmissionEventQueue, evt);
}



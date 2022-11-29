/*
 * communication.c
 *
 *  Created on: 19.11.2022
 *      Author: pasik
 */

#include "usb_device.h"
#include "com_provider.h"
#include "communication.h"

char tokenizebuffer[BUFFERSIZE];
char* token;

UEB_StatusType pUEB_status;

uint8_t receivebuffer[BUFFERSIZE];




void getFloatOfMessage(char *message, char *commandstring, float value)
{
	char *s;
	s = strstr(message, commandstring);      // search for searchedstring in message
	s =  s + strlen(commandstring);// index of s in buff can be found by pointer subtraction
	value = (float_t)strtod(s, NULL);
}

void setUEBValues(char *message)
{
	if(strstr(message, SETVCCVOLTAGE) != NULL){
		getFloatOfMessage(message, (char *)SETVCCVOLTAGE, pUEB_status.vccvoltage);
	} else if(strstr(message, SETOUTVOLTAGE) != NULL){
		getFloatOfMessage(message, (char *)SETOUTVOLTAGE, pUEB_status.outvoltage);
	} else if(strstr(message, SETFREQUENCY) != NULL){
		getFloatOfMessage(message, (char *)SETFREQUENCY, pUEB_status.frequency);
	} else if(strstr(message, SETROTATION) != NULL){
		getFloatOfMessage(message, (char *)SETROTATION, pUEB_status.rotationdirection);
	} else if(strstr(message, SETTHIRDHARMONIC) != NULL){
		getFloatOfMessage(message, (char *)SETTHIRDHARMONIC, pUEB_status.thirdharmonic);
	} else if(strstr(message, SETSOFTSTART) != NULL){
		getFloatOfMessage(message, (char *)SETSOFTSTART, pUEB_status.softstart);
	} else if(strstr(message, SETSOFTSTARTDURATION) != NULL){
		getFloatOfMessage(message, (char *)SETSOFTSTARTDURATION, pUEB_status.softstartduration);
	} else if(strstr(message, SETMAXCURRENT) != NULL){
		getFloatOfMessage(message, (char *)SETMAXCURRENT, pUEB_status.maxcurrent);
	} else if(strstr(message, SETNUMAVERAGED) != NULL){
		getFloatOfMessage(message, (char *)SETNUMAVERAGED, pUEB_status.averagenum);
	} else {
		setBuffer((uint8_t*)"Error: Wrong Command\r", strlen("Error: Wrong Command\r"));
	}
}

//void getUint32OfMessage(char *message, char *commandstring, uint32_t value)
//{
//	char *s;
//	s = strstr(message, commandstring);
//	s = s + strlen(commandstring);
//	value = (uint32_t)strtod(s, NULL);
//}

void decodeMessage(char *message)
{
	if (strstr(message, UEBREADY) != NULL) {
		setBuffer((uint8_t*)UEB_VERSION, strlen(UEB_VERSION));
	} else if(strstr(message, GETMEASURES) != NULL){
		pUEB_status.status = UEB_RUN;
	} else if(strstr(message, STOPMEASURES) != NULL){
		pUEB_status.status = UEB_STOP;
	} else if(strstr(message, GETPARAMETERS) != NULL){
		uint8_t* string;
		string = malloc(1024);

		snprintf((char*)string, 1024, "%s%d;%s%d;%s%d;%s%d;%s%d;%s%d;%s%d;%s%d;%s%d;%s%d\r",
				GETSTATUS, (uint8_t)pUEB_status.status, SETVCCVOLTAGE, (uint8_t)pUEB_status.vccvoltage,
				SETOUTVOLTAGE, (uint8_t)pUEB_status.outvoltage, SETFREQUENCY, (uint8_t)pUEB_status.frequency,
				SETROTATION, (uint8_t)pUEB_status.rotationdirection, SETTHIRDHARMONIC, (uint8_t)pUEB_status.thirdharmonic,
				SETSOFTSTART, (uint8_t)pUEB_status.softstart, SETSOFTSTARTDURATION, (uint8_t)pUEB_status.softstartduration,
				SETMAXCURRENT, (uint8_t)pUEB_status.maxcurrent, SETNUMAVERAGED, (uint8_t)pUEB_status.averagenum);
		setBuffer(string, strlen((char*)string));
		free(string);
	} else if(strstr(message, SETPARAMETERS) != NULL){
		switch (pUEB_status.status) {
			case UEB_INIT:
				pUEB_status.status = UEB_INIT_FINISH;
				setBuffer((uint8_t*)UEB_CONFIG_SAVED, strlen(UEB_CONFIG_SAVED));
				break;
			case UEB_STOP:
				pUEB_status.status = UEB_INIT;
				setBuffer((uint8_t*)UEB_IN_CONFIG_MODE, strlen(UEB_IN_CONFIG_MODE));
				break;
			default:
				break;
		}
	} else {
		setUEBValues(message);
	}
}

void divideMessage(uint8_t *USBbuffer, const char *delimiter)
{
	strcpy(tokenizebuffer, (const char *)USBbuffer);
	uint8_t i = 0;
	token = strtok(tokenizebuffer, delimiter);
	while (token != NULL) {
		decodeMessage(token);
		token = strtok(NULL, delimiter);
		i++;
	}
}

void getNewStatus(UEB_StatusType *uebstatus)
{
	pUEB_status = *uebstatus;
	get_receive_message(receivebuffer, BUFFERSIZE);
	if(receivebuffer[0] != '\0') {
		divideMessage(receivebuffer, DELIMITER_FULLMESSAGE);
		memset(receivebuffer, '\0', BUFFERSIZE);
	}
}
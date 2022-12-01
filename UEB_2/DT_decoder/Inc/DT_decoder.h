/*
 * DT_decoder.h
 *
 *  Created on: 01.12.2022
 *      Author: pasik
 */

#ifndef INC_DT_DECODER_H_
#define INC_DT_DECODER_H_

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

/*
float 			frequency = 120;				//enter frequency of the 3-phase sine (range of values: 0.1Hz to 200Hz)
float 			v_bridge_uf = 11;			//enter the voltage that applies at "u_brueke_uf" (range of values: 10V to 60V)
float 			voltage_ref = 6;				//enter your preferred voltage for the amplitude of the sine (range of Values: 1V to 0.95*"v_bridge_uf")
bool 			rotationDirectionCW = false;		//enter the direction of rotation, true -> clockwise; false -> counterclockwise
bool 			enableThirdHarmonic = true;	    //enter true, if you want to enable the third harmonic mode
bool 			enableSoftstarter = false;		//enter true, if you want the motor to start slowly
float			softstarterDuration = 1;		//enter duration of the softstarter ramp in seconds
const float		overCurrentThreshold = 10; 		//enter the allowed current in ampere (range of values: 0 to 10 Ampere)
uint32_t		numberOfAveragedValues = 10;    //enter by how many current values you want to calculate the average
*/
typedef struct
{
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

/*
float 			amperePerDigits = 0.0007575757; //(0.05mV/Digits)/(66mV/A)=0.0007575757A/Digit
float 			bufferSum_IHB1 = 0;
float 			bufferSum_IHB2 = 0;
float 			bufferSum_IHB3 = 0;
float 			bufferAverage_IHB1 = 0;
float 			bufferAverage_IHB2 = 0;
float 			bufferAverage_IHB3 = 0;
float 			bufferCalibrated1 = -33900;
float 			bufferCalibrated2 = -33900;
float 			bufferCalibrated3 = -33900;
float 			current_IHB1 = 0;
float 			current_IHB2 = 0;
float 			current_IHB3 = 0;
*/
typedef struct
{
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
 * 	   CONF-->
 * 		|
 * 		|
 * 		|
 * 	  STATUS-->
 * 	  	|
 * 	  	|
 * 	  	|
 *
 *
 *
 *
 */
static const char GETMEASURES[] = "UEB_start_measure";			//starts the measure and pushes all data to the serial port
static const char STOPMEASURES[] = "UEB_stop_measure";			//stops pushing data to the serial port

//Command to get Parameters of the UEB
static const char GETPARAMETERS[] = "UEB_get_params";
static const char GETSTATUS[] = "UEB_status:";

//Commands to set Parameters on the UEB
static const char SETPARAMETERS[] = "UEB_set_params";			//
//static const char SAVEPARAMETERS[] = "UEB_save_params";
static const char SETVCCVOLTAGE[] = "UEB_Vcc:";				//The value must be in
static const char SETOUTVOLTAGE[] = "UEB_Vout:";

static const char SETFREQUENCY[] = "UEB_freq:";
static const char SETROTATION[] = "UEB_rot:";
static const char SETTHIRDHARMONIC[] = "UEB_thirdharm:";
static const char SETSOFTSTART[] = "UEB_softstart:";
static const char SETSOFTSTARTDURATION[] = "UEB_softdur:";
static const char SETMAXCURRENT[] = "UEB_maxCurr:";
static const char SETNUMAVERAGED[] = "UEB_averagednum:";

static const char DELIMITER_FULLMESSAGE[] = ";";
static const char DELIMITER_PARTMESSAGE[] = ":";
static const char UEB_VERSION[] = "UEB_Version_1.0";

static const char UEB_IN_CONFIG_MODE[] = "UEB_in_config_mode";
static const char UEB_CONFIG_SAVED[] = "UEB_config_saved";
//Functions

void getNewStatus(UEB_StatusType *uebstatus);


#endif /* INC_DT_DECODER_H_ */
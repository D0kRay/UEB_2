/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "math.h"
#include "stm32h743xx.h"
#include "Decoder_Interface.h"
#include "com_provider.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define __TESTVAR_H
#define T4R				    		(TIM4->CR1 |= TIM_CR1_CEN)
#define T4S 				  		(TIM4->CR1 &= ~TIM_CR1_CEN)
#define PERIOD4						10000
#define clockFrequency				120000000

//Errors
#define no_Error					0
#define invalidValues_Error			1
#define overcurrent_Error			2
#define watchdog_Error				3
#define opticalFiber_Error			4
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

IWDG_HandleTypeDef hiwdg1;

SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;

/* USER CODE BEGIN PV */


float 			frequency = 120;				//enter frequency of the 3-phase sine (range of values: 0.1Hz to 200Hz)
float 			v_bridge_uf = 11;			//enter the voltage that applies at "u_brueke_uf" (range of values: 10V to 60V)
float 			voltage_ref = 6;				//enter your preferred voltage for the amplitude of the sine (range of Values: 1V to 0.95*"v_bridge_uf")
bool 			rotationDirectionCW = false;		//enter the direction of rotation, true -> clockwise; false -> counterclockwise
bool 			enableThirdHarmonic = true;	    //enter true, if you want to enable the third harmonic mode
bool 			enableSoftstarter = false;		//enter true, if you want the motor to start slowly
float			softstarterDuration = 1;		//enter duration of the softstarter ramp in seconds
const float		overCurrentThreshold = 10; 		//enter the allowed current in ampere (range of values: 0 to 10 Ampere)
uint32_t		numberOfAveragedValues = 10;    //enter by how many current values you want to calculate the average


float			maxTensionRelationship = 0.85;  //the max "voltage_ref" = "v_bridge_uf" * "maxTensionRelationship". This value is determined through attempts



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

uint32_t 		WDHTR = 0;

const float 	pwmFrequency = 20000; 			//enter PWM-Frequency for the sine modulation
const uint32_t 	numberOfSineValues = 1800; 		//how many Values are entered in the "pulseWidth" Array
const uint32_t 	rangeOfSineValues = 1800;  		//the highest value in the "pulseWidth" Array

uint32_t 		counterperiod_TIM1 =   (clockFrequency/pwmFrequency)/2;				        //120MHZ/20kHz/2 = 3000
uint32_t 		counterperiod_TIM2 =    clockFrequency/pwmFrequency-1;				        //120MHz/20kHz-1 = 5999
uint32_t		counterperiod_TIM3 = clockFrequency/pwmFrequency*10-1;
uint32_t		counterperiod_TIM4 =  600000-1;
float 			pwmPeriodConversion = ((clockFrequency/pwmFrequency)/2)/rangeOfSineValues;//((120MHz/20kHz)/2)/1800 = 1.6666666666

uint8_t 		error = no_Error;


unsigned 		bufferFlag;
unsigned 		seqFlag;
uint32_t		convres;
uint32_t     	buffer[100][3];	//into this buffer, the DMA writes the measured current values

int				rotVelo = 0;	//rotation velocity of the motor in rpm

//Wechselberger, Kirchhoff USB
uint8_t			transmiton = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM4_Init(void);
static void MX_ADC1_Init(void);
static void MX_IWDG1_Init(void);
static void MX_TIM5_Init(void);
static void MX_TIM3_Init(void);
static void MX_SPI2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void LED_test(void)
{
	//toggle of each LED for testing
	HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_0);
	HAL_Delay(500);
	HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_1);
	HAL_Delay(500);
	HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_2);
	HAL_Delay(500);
	HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_3);
	HAL_Delay(500);
	HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_4);
	HAL_Delay(500);
	HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_5);
	HAL_Delay(500);
	HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_6);
	HAL_Delay(500);
	HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_7);
	HAL_Delay(500);

}
void DIP_test(void)
{
	//Activates LED for each Dip Switch
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_8));
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_9));
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_2, HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_10));
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_3, HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_11));
}
void Transmitter_Receiver_test(void)
{
	//Sends a toggling signal to the transmitter.
	//With the optical fiber, the Signal is fed back to receiver.
	//the LEDs show, which Receiver the toggling signal received.
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_1);
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1));
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2));
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_5, HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3));
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_4, HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4));
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_3, HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_5));
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_2, HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_6));
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_7));
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_9));
}
void Enable_Bridge_test (void)
{
	//If Dip Switch 1 is HIGH, the Bridge gets enabled
	if(HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_8))
	{
		HAL_GPIO_WritePin(EN_BRUECKE_1_GPIO_Port, EN_BRUECKE_1_Pin, 1); 	//bridge gets enabled
	}
}


void ADC_Start (void)
{
	while (!(ADC1->ISR & ADC_ISR_ADRDY)) ;
	seqFlag = 0;
	bufferFlag = 0;
	ADC1->CR |= ADC_CR_ADSTART;										// cleared by Hardware after Start
}

void ADCDMA_Init (void)
 {
 RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;								// Channel0
 DMA1_Stream0->CR &= ~DMA_SxCR_EN;
 DMA1_Stream0->CR &= ~DMA_SxCR_MBURST;								// DMA_MemoryBurst = DMA_MemoryBurst_Single
 DMA1_Stream0->CR &= ~DMA_SxCR_MSIZE;
 DMA1_Stream0->CR |= DMA_SxCR_MSIZE_1;								// 10 DMA_MemoryDataSize 32 bit
 DMA1_Stream0->CR |= DMA_SxCR_MINC;									// DMA_MemoryIncrement enable
 DMA1_Stream0->CR &= ~DMA_SxCR_CIRC;								// DMA_Mode = DMA_Mode_Normal
 DMA1_Stream0->PAR = (uint32_t)(&(ADC1->DR));						// DMA_PeripheralBaseAddr
 DMA1_Stream0->CR &= ~DMA_SxCR_PBURST;								// DMA_PeripheralBurst = DMA_PeripheralBurst_Single
 DMA1_Stream0->CR &= ~DMA_SxCR_PSIZE;
 DMA1_Stream0->CR |= DMA_SxCR_PSIZE_1;								// 10 DMA_PeripheralDataSize 32 bit
 DMA1_Stream0->CR &= ~DMA_SxCR_PINC;								// DMA_PeripheralIncrement disable
 DMA1_Stream0->CR &= ~DMA_SxCR_PL;									// priority 0 nur eine DMA
 DMA1_Stream0->CR &= ~DMA_SxCR_DIR;									// DMA_DIR = DMA_DIR_PeripheralToMemory
 DMA1_Stream0->CR |= DMA_SxCR_TCIE;									// transfer complete Interrupt enable
 DMAMUX1_Channel0->CCR = 0x00000009;								// Channel0 ADC1
 //DMA1_Stream0->NDTR = 3;											// DMA_BufferSize for every sequence
 DMA1_Stream0->NDTR = 300;	 										// DMA_BufferSize for full Buffer
 DMA1_Stream0->M0AR = (uint32_t)(&(buffer[0][0]));					// DMA_Memory0BaseAddr
 DMA1->LIFCR = DMA_LIFCR_CTCIF0;
 DMA1_Stream0->CR |= DMA_SxCR_EN; 									// Enable the DMA
 HAL_NVIC_EnableIRQ (DMA1_Stream0_IRQn);
 }

void ADC_Init (void)
{
	ADCDMA_Init ();
	ADC1->CR &= ~ADC_CR_ADEN;
	RCC->AHB4ENR |= RCC_AHB4ENR_GPIOAEN;							// enable clock for GPIO PA4 und PA6 ADC1 Channel 3 und 18
	RCC->AHB4ENR |= RCC_AHB4ENR_GPIOCEN;							// enable clock for GPIO PC4 ADC1 Channel 4
	RCC->AHB1ENR |= RCC_AHB1ENR_ADC12EN;              				// enable clock for ADC1
	GPIOA->MODER |= 0x00003300;										// pin PA4 und PA6 analog Input
	GPIOA->PUPDR &= 0xFFFF00FF;        								// pin PA4 und PA6 no pull-up, no pull-down
	GPIOC->MODER |= 0x00000300;                       				// pin PC4 analog Input
	GPIOC->PUPDR &= 0xFFFFF0FF;        								// pin PC4 no pull-up, no pull-down
	ADC1->CFGR &= ~ADC_CFGR_RES;									// 00: 16 bit resolution
	//ADC1->CFGR |= ADC_CFGR_DISCEN;								// 1: discontinuous regular mode
	ADC1->CFGR &= ~ADC_CFGR_DISCEN;
	//ADC1->CFGR &= ~ADC_CFGR_CONT;
	ADC1->CFGR |= ADC_CFGR_CONT;									// continuous mode
	//ADC1->IER |= ADC_IER_EOCIE;									// Interrupt conversion is complete
	//ADC1->IER |= ADC_IER_EOSIE;									// Interrupt sequence is complete
	ADC1->IER |= ADC_IER_AWD1IE;	 								// Watchdog 1 interrupt enable

	WDHTR = overCurrentThreshold/amperePerDigits-bufferCalibrated1;

	ADC1->HTR1 = WDHTR;	 											// Watchdog 1 high threshold
	ADC1->CFGR |= ADC_CFGR_AWD1EN;									// Watchdog 1 enable
	ADC1->CFGR &= ~ADC_CFGR_EXTEN;									// 00: extern Trigger disabled
	//ADC1->CFGR &= ~ADC_CFGR_DMNGT;								// 00: no DMA
	ADC1->CFGR |= ADC_CFGR_DMNGT;									// 11: DMA cicular mode
	//ADC1->CFGR |= ADC_CFGR_DMNGT_0;								// 01: DMA one shot
	ADC1->CFGR |= ADC_CFGR_OVRMOD;									// 1: last conversation
	ADC1->IER &= ~ADC_IER_OVRIE;									// overrun Interrupt disabled
	ADC1->SMPR2 = 0x00004800;										// 100: 32 cycles sampling time 32 cicles for Channel 3 und 4
	ADC1->SMPR2 |= 0x04000000;										// 100: 32 cycles sampling time 32 cicles for Channel 18
	//ADC1->PCSEL = 0x00000010;
	ADC1->PCSEL = 0x00040018;										// preselect Channel 4, 3, 18
	//ADC1->SQR1 = 0x00000100;
	ADC1->SQR1 = 0x00483102;										// Channel sequence assign to Channel 4, 3, 18
	ADC12_COMMON->CCR &= ~ADC_CCR_PRESC;							// ADCLK not divided
	ADC12_COMMON->CCR |= 0x002C0000;								// ADCLK Clock div 256
	//ADC12_COMMON->CCR |= 0x00240000;								// ADCLK Clock div 64
	ADC1->CR &= ~ADC_CR_DEEPPWD;									// no ADC power down
	ADC1->CR |= ADC_CR_ADVREGEN;
	while (!(ADC1->ISR & 0x00001000)) ;
	ADC12_COMMON->CCR |= ADC_CCR_VREFEN;
	ADC1->CR |= ADC_CR_ADEN;										// AD Converter on
	HAL_NVIC_EnableIRQ (ADC_IRQn);
}


void setParameters(UEB_StatusType *ueb_status)
{
	if(ueb_status->status == UEB_INIT_FINISH) {
		frequency = ueb_status->frequency;				//enter frequency of the 3-phase sine (range of values: 0.1Hz to 200Hz)
		v_bridge_uf = ueb_status->vccvoltage;			//enter the voltage that applies at "u_brueke_uf" (range of values: 10V to 60V)
		voltage_ref = ueb_status->outvoltage;				//enter your preferred voltage for the amplitude of the sine (range of Values: 1V to 0.95*"v_bridge_uf")
		rotationDirectionCW = ueb_status->rotationdirection;		//enter the direction of rotation, true -> clockwise; false -> counterclockwise
		enableThirdHarmonic = ueb_status->thirdharmonic;	    //enter true, if you want to enable the third harmonic mode
		enableSoftstarter = ueb_status->softstart;		//enter true, if you want the motor to start slowly
		softstarterDuration = ueb_status->softstartduration;		//enter duration of the softstarter ramp in seconds
	//	overCurrentThreshold = ueb_status->maxcurrent; 		//enter the allowed current in ampere (range of values: 0 to 10 Ampere)
		numberOfAveragedValues = ueb_status->averagenum;    //enter by how many current values you want to calculate the average
	//	pwmFrequency = ueb_status->pwmfrequency;
	}
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */



  uint8_t DT_TransmissionBuffer[1024];
  //char DT_TestString[] = "This is the Test String for the Data Transmission via the Command fields of the Computer.�?�nThis Text will be transmitted via multiple Data packages!";

  char DT_TestString[] = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi. Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat. Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi.Nam liber tempor cum soluta nobis eleifend option congue nihil imperdiet doming id quod mazim placerat facer possim assum. Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat. Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis.At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, At accusam aliquyam diam diam dolore dolores duo eirmod eos erat, et nonumy sed tempor et et invidunt justo labore Stet clita ea et gubergren, kasd magna no rebum. sanctus sea sed takimata ut vero voluptua. est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit ame:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi. Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat. Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi.Nam liber tempor cum soluta nobis eleifend option congue nihil imperdiet doming id quod mazim placerat facer possim assum. Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat. Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis.At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, At accusam aliquyam diam diam dolore dolores duo eirmod eos erat, et nonumy sed tempor et et invidunt justo labore Stet clita ea et gubergren, kasd magna no rebum. sanctus sea sed takimata ut vero voluptua. est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit ame";


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_ADC1_Init();
  MX_IWDG1_Init();
  MX_TIM5_Init();
  MX_TIM3_Init();
  MX_USB_DEVICE_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, 1);	//LED 3 turned on to see system resets in case of IWDG1(Watchdog)
  error = 3;
  HAL_TIM_Base_Start_IT(&htim4);					//Start TIM4


  HAL_TIM_Base_Start_IT(&htim2);					//Start TIM2

  HAL_TIM_Base_Start_IT(&htim3);					//Start TIM3


//  for ( int i = 0;  i < 50; ++ i) //1195		//Wait for a certain amount of microseconds
//  {
//	  //empty loop, to adjust the phase of Tim1 to Tim2
//  }
  //Starting Tim1 means starting the PWM Timer
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);		//Start TIM1_Channel1_positive
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);	//Start TIM1_Channel1_negative
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);		//Start TIM1_Channel2_positive
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);  //Start TIM1_Channel2_negative
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);		//Start TIM1_Channel3_positive
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);  //Start TIM1_Channel3_negative


  ADC_Init ();									//Initialise own ADC
  ADC_Start();									//Start own ADC


  HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, 0); //LED 3 gets turned off again to see whether the system resets in case of IWDG1(Watchdog)
  error = 0;

  HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);

  //Wechselberger Kirchhoff below
  UEB_StatusType uebstatus = {
		  .status = UEB_STOP,
		  .vccvoltage = 10.45,
		  .outvoltage = 1.545,
		  .frequency = 1000.55,
		  .rotationdirection = 0,
		  .thirdharmonic = 0,
		  .softstart = 1,
		  .softstartduration = 5.5,
		  .maxcurrent = 4.53,
		  .averagenum = 24
  };


  // Create EventQueues for priority run of the application
    EventQueue* Q_USB 				= EventQueue_Init();
    EventQueue* Q_DataTransmission 	= EventQueue_Init();
    EventQueue* Q_Main 				= EventQueue_Init();
    ST_init();

  //UEB_MeasuresType uebmeasure;
  provideStatus(&uebstatus);
  uint16_t counterM = 0;

  Event *evt1 = malloc (sizeof(Event));
  setEventClass(evt1,Routine);
  setEventMessage(evt1,DataTransmissionComplete);
  addEvent(&Q_DataTransmission,evt1);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
	  // Main Queue
	  if(isEventQueued(Q_Main)){
		  Event evt;
		  getEvent(&Q_Main, &evt);
		  switch(getEventClass(evt)){

		  case Interrupt:

			  switch (getEventMessage(evt)) {

				case UEB_params_set:
					setParameters(&uebstatus);
					break;
				/*
				case value:

					break;
				*/

				default:
					break;
			}
			  break;

		  case Routine:

			  switch (getEventMessage(evt)) {
			  /*
				case value:

					break;
			  */

				default:
					break;
			}
			  break;

		  default:
			  break;
		  }
	  }


	  // USB Queue
	  if(isEventQueued(Q_USB)){
		  Event evt;
		  getEvent(&Q_USB, &evt);
		  switch(getEventClass(evt)){

		  case Interrupt:	//Transmit data from DT_Transmission over USB

			  switch(getEventMessage(evt)){	//Interrupt Events
			  case 0:
				  if(counterM < 512) {
					  TransmitBuffer(DT_TransmissionBuffer,sizeof(DT_TransmissionBuffer));
					  counterM++;
					  Event *evt = malloc (sizeof(Event));

					  (*evt).class = 0;
					  (*evt).message = 1;

					  addEvent(&Q_DataTransmission, evt);
				  }
				  break;

			  case StatusInfoReceived:

				  getMessage(&Q_USB);		// TODO receive message entweder in decoder oder hier????

				  break;

			  case StatusCommandReceived:

//				  getMessage(&Q_USB);		// TODO receive message entweder in decoder oder hier????
				  setEventClass(&evt,Interrupt);
				  setEventMessage(&evt,1);
				  addEvent(&Q_DataTransmission,&evt);
				  setEventClass(&evt,Interrupt);
				  setEventMessage(&evt,2);
				  addEvent(&Q_DataTransmission,&evt);

				  break;

			  default:
				  break;
			  }
			  break;

		  case Routine:

			  switch(getEventMessage(evt)){	//Routine Events

			  case DTBufferReady:	//Send buffer if ready
				  if(is_Transmit_Complete()){
					  TransmitBuffer(DT_TransmissionBuffer,sizeof(DT_TransmissionBuffer));
					  setEventClass(&evt,Routine);
					  setEventMessage(&evt,DataTransmissionComplete);
					  addEvent(&Q_DataTransmission,&evt);
				  } else {
					  setEventClass(&evt,Routine);
					  setEventMessage(&evt,DTBufferReady);
					  addEvent(&Q_USB,&evt);
				  }
				  break;


			  default:
				  break;
			  }
			  break;

		  default:
			  break;
		  }
	  }

	  // Data Transmission Queue
	  if(isEventQueued(Q_DataTransmission)){
		  Event evt;
		  getEvent(&Q_DataTransmission, &evt);
		  DT_status status;
		  uint8_t ID;
		  switch(getEventClass(evt)){
		  case Interrupt:

			  switch(getEventMessage(evt)){	//Interrupt Events
			  case 0:
				  strcpy((char*)DT_TransmissionBuffer,"Hello World!");
				  setEventClass(&evt,Routine);
				  setEventMessage(&evt,DTBufferReady);
				  addEvent(&Q_USB,&evt);
				  break;

			  case 1:
				 status = DT_Init(&ID, DT_TestString, sizeof(DT_TestString));
				 if(DT_isError(status))
					 break;
				 break;

			  case 2:
				 status = DT_Start(ID);
				 if(DT_isError(status))
					 break;
				 break;

			  default:
				  break;
			  }
			  break;

		  case Routine:

			  switch(getEventMessage(evt)){	//Routine Events
			  case DataTransmissionComplete:
				  if(DT_activeData()){
					  DT_TransmitData(DT_TransmissionBuffer);
					  setEventClass(&evt,Routine);
					  setEventMessage(&evt,DTBufferReady);
					  addEvent(&Q_USB,&evt);
				  }else{
					  setEventClass(&evt,Routine);
					  setEventMessage(&evt,DataTransmissionComplete);
					  addEvent(&Q_DataTransmission,&evt);
				  }
				  break;


			  default:
				  break;
			  }
			  break;

		  default:
			  break;
		  }
	  }

	  // if button pressed run dataset
	  if(is_Receive_Complete()){
		  Event *evt = malloc (sizeof(Event));
		  //TODO (Event*)&evt durch evt ersetzt, da evt schon ein Pointer ist -> sonst HardFaultHandler
		  setEventClass(evt,Interrupt);
		  setEventMessage(evt,StatusInfoReceived);
		  addEvent(&Q_USB, evt);
	  	  }
	  if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET){
		  Event *evt = malloc (sizeof(Event));
		  setEventClass(evt,Interrupt);
		  setEventMessage(evt,1);
		  addEvent(&Q_DataTransmission, evt);
	  }

	  //ADC Abtastfunktionen
	  /*
	   * 1. ADC Frequenz ändern
	   * 2. ADC Kanäle ändern
	   * 3.	ADC Buffergröße ändern und allokieren
	   * 4.	ADC Auflösung einstellen
	   * 5.	ADC Abtastrate für einzelne Kanäle wählen
	   */
  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//  void ADC_ChangeFrequency(uint32_t frequency)
//  {
//
//  }
//
//  void ADC_SetChannels(uint32_t channel, uint32_t sampling_rate)
//  {
//
//  }
//
//  void ADC_AllocateBuffer(ADC_ConfigType adc_config)
//  {
//
//  }
//
//  void ADC_SetResolution(uint32_t resolution)
//  {
//
//  }
//
//  void ADC_SetSamplingRate(uint32_t channel, uint32_t sampling_rate)
//  {
//
//  }
//
//  void ADC_GetBuffer(void* buffer)
//  {
//
//  }
//
//  void ADC_WriteToBuffer()
//  {
//
//  }
//
//  uint32_t generate_Timestamp()
//  {
//	  uint32_t timestamp = HAL_GetTick();	//Generate Timestamp in ms from Controller Startup
//	  //TODO Timestamp als Echtzeit mittels RTC
//  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSI);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSI
                              |RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 30;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI2|RCC_PERIPHCLK_ADC
                              |RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.PLL2.PLL2M = 4;
  PeriphClkInitStruct.PLL2.PLL2N = 10;
  PeriphClkInitStruct.PLL2.PLL2P = 1;
  PeriphClkInitStruct.PLL2.PLL2Q = 2;
  PeriphClkInitStruct.PLL2.PLL2R = 2;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_3;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOMEDIUM;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
  PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable USB Voltage detector
  */
  HAL_PWREx_EnableUSBVoltageDetector();
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV8;
  hadc1.Init.Resolution = ADC_RESOLUTION_16B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  sConfig.OffsetSignedSaturation = DISABLE;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief IWDG1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_IWDG1_Init(void)
{

  /* USER CODE BEGIN IWDG1_Init 0 */

  /* USER CODE END IWDG1_Init 0 */

  /* USER CODE BEGIN IWDG1_Init 1 */

  /* USER CODE END IWDG1_Init 1 */
  hiwdg1.Instance = IWDG1;
  hiwdg1.Init.Prescaler = IWDG_PRESCALER_4;
  hiwdg1.Init.Window = 799;
  hiwdg1.Init.Reload = 799;
  if (HAL_IWDG_Init(&hiwdg1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG1_Init 2 */

  /* USER CODE END IWDG1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 0x0;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi2.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi2.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi2.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi2.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi2.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi2.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi2.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi2.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi2.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
  htim1.Init.Period = counterperiod_TIM1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */
  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = counterperiod_TIM2;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = counterperiod_TIM3;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = counterperiod_TIM4;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 0;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 4096;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 10;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 10;
  if (HAL_TIM_Encoder_Init(&htim5, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, Test_pulse_Pin|EN_BRUECKE_1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, LED1_Pin|LED2_Pin|LED3_Pin|LED4_Pin
                          |LED5_Pin|LED6_Pin|LED7_Pin|LED8_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOH, R_F_W_UC_Pin|R_F_V_UC_Pin|R_F_U_UC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TRANSMITTER1_GPIO_Port, TRANSMITTER1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : Test_pulse_Pin EN_BRUECKE_1_Pin */
  GPIO_InitStruct.Pin = Test_pulse_Pin|EN_BRUECKE_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : DIPSW1_Pin DIPSW2_Pin DIPSW3_Pin DIPSW4_Pin */
  GPIO_InitStruct.Pin = DIPSW1_Pin|DIPSW2_Pin|DIPSW3_Pin|DIPSW4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LED1_Pin LED2_Pin LED3_Pin LED4_Pin
                           LED5_Pin LED6_Pin LED7_Pin LED8_Pin */
  GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin|LED3_Pin|LED4_Pin
                          |LED5_Pin|LED6_Pin|LED7_Pin|LED8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : Reference_Signal_Pin */
  GPIO_InitStruct.Pin = Reference_Signal_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(Reference_Signal_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : R_F_W_UC_Pin R_F_V_UC_Pin R_F_U_UC_Pin */
  GPIO_InitStruct.Pin = R_F_W_UC_Pin|R_F_V_UC_Pin|R_F_U_UC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pins : BRKIN_W_Pin BRKIN_V_Pin BRKIN_U_Pin */
  GPIO_InitStruct.Pin = BRKIN_W_Pin|BRKIN_V_Pin|BRKIN_U_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pin : TRANSMITTER1_Pin */
  GPIO_InitStruct.Pin = TRANSMITTER1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TRANSMITTER1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RECIVER7_Pin RECIVER6_Pin RECIVER5_Pin RECIVER4_Pin
                           RECIVER3_Pin RECIVER2_Pin */
  GPIO_InitStruct.Pin = RECIVER7_Pin|RECIVER6_Pin|RECIVER5_Pin|RECIVER4_Pin
                          |RECIVER3_Pin|RECIVER2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : RECIVER1_Pin */
  GPIO_InitStruct.Pin = RECIVER1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(RECIVER1_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

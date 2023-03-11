/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
#include "DT_interface.h"
#include "EventQueue.h"
#include "AD2S1210.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
typedef enum{
	UEB_params_set = 0,
	Resolver_params_set = 1,
	Resolver_read_data = 2
}Main_Messages;

typedef enum{
	DataTransmissionComplete = 0,
	DT_Test1 = 1,
	DT_Test2 = 2,
	GUIInitDataset_NEW = 3,
	GUIInitDataset_PRE = 4,
	GUIFeedbackComplete = 5
}DT_Messages;

typedef enum{
	DTBufferReady = 0,
	StatusInfoReceived = 1,
	StatusCommandReceived = 2,
	SetStatusReceived = 3,
	DTTransmissionComplete = 4,
	DTTransmissionInit = 5,
	DTTransmissionNEW = 6
}USB_Messages;

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Test_pulse_Pin GPIO_PIN_5
#define Test_pulse_GPIO_Port GPIOE
#define DIPSW1_Pin GPIO_PIN_8
#define DIPSW1_GPIO_Port GPIOI
#define DIPSW2_Pin GPIO_PIN_9
#define DIPSW2_GPIO_Port GPIOI
#define DIPSW3_Pin GPIO_PIN_10
#define DIPSW3_GPIO_Port GPIOI
#define DIPSW4_Pin GPIO_PIN_11
#define DIPSW4_GPIO_Port GPIOI
#define LED1_Pin GPIO_PIN_0
#define LED1_GPIO_Port GPIOF
#define LED2_Pin GPIO_PIN_1
#define LED2_GPIO_Port GPIOF
#define LED3_Pin GPIO_PIN_2
#define LED3_GPIO_Port GPIOF
#define LED4_Pin GPIO_PIN_3
#define LED4_GPIO_Port GPIOF
#define LED5_Pin GPIO_PIN_4
#define LED5_GPIO_Port GPIOF
#define LED6_Pin GPIO_PIN_5
#define LED6_GPIO_Port GPIOF
#define LED7_Pin GPIO_PIN_6
#define LED7_GPIO_Port GPIOF
#define LED8_Pin GPIO_PIN_7
#define LED8_GPIO_Port GPIOF
#define RESOLVER_A1_Pin GPIO_PIN_8
#define RESOLVER_A1_GPIO_Port GPIOF
#define RESOLVER_A0_Pin GPIO_PIN_9
#define RESOLVER_A0_GPIO_Port GPIOF
#define RESOLVER_WR_FSYNC_Pin GPIO_PIN_10
#define RESOLVER_WR_FSYNC_GPIO_Port GPIOF
#define RESOLVER_SPI_SDI_Pin GPIO_PIN_1
#define RESOLVER_SPI_SDI_GPIO_Port GPIOC
#define RESOLVER_SPI_SDO_Pin GPIO_PIN_2
#define RESOLVER_SPI_SDO_GPIO_Port GPIOC
#define Reference_Signal_Pin GPIO_PIN_2
#define Reference_Signal_GPIO_Port GPIOA
#define RESOLVER_SAMPLE_Pin GPIO_PIN_2
#define RESOLVER_SAMPLE_GPIO_Port GPIOH
#define IHB1_UC_Pin GPIO_PIN_4
#define IHB1_UC_GPIO_Port GPIOA
#define IHB2_UC_Pin GPIO_PIN_6
#define IHB2_UC_GPIO_Port GPIOA
#define IHB3_UC_Pin GPIO_PIN_4
#define IHB3_UC_GPIO_Port GPIOC
#define HB2L_1_Pin GPIO_PIN_0
#define HB2L_1_GPIO_Port GPIOB
#define HB3L_1_Pin GPIO_PIN_1
#define HB3L_1_GPIO_Port GPIOB
#define HB1L_1_Pin GPIO_PIN_8
#define HB1L_1_GPIO_Port GPIOE
#define HB1H_1_Pin GPIO_PIN_9
#define HB1H_1_GPIO_Port GPIOE
#define HB2H_1_Pin GPIO_PIN_11
#define HB2H_1_GPIO_Port GPIOE
#define HB3H_1_Pin GPIO_PIN_13
#define HB3H_1_GPIO_Port GPIOE
#define EN_BRUECKE_1_Pin GPIO_PIN_15
#define EN_BRUECKE_1_GPIO_Port GPIOE
#define RESOLVER_SPI_CLK_Pin GPIO_PIN_10
#define RESOLVER_SPI_CLK_GPIO_Port GPIOB
#define R_F_W_UC_Pin GPIO_PIN_6
#define R_F_W_UC_GPIO_Port GPIOH
#define R_F_V_UC_Pin GPIO_PIN_7
#define R_F_V_UC_GPIO_Port GPIOH
#define R_F_U_UC_Pin GPIO_PIN_8
#define R_F_U_UC_GPIO_Port GPIOH
#define BRKIN_W_Pin GPIO_PIN_9
#define BRKIN_W_GPIO_Port GPIOH
#define BRKIN_W_EXTI_IRQn EXTI9_5_IRQn
#define BRKIN_V_Pin GPIO_PIN_10
#define BRKIN_V_GPIO_Port GPIOH
#define BRKIN_V_EXTI_IRQn EXTI15_10_IRQn
#define BRKIN_U_Pin GPIO_PIN_11
#define BRKIN_U_GPIO_Port GPIOH
#define BRKIN_U_EXTI_IRQn EXTI15_10_IRQn
#define TRANSMITTER1_Pin GPIO_PIN_1
#define TRANSMITTER1_GPIO_Port GPIOD
#define RECIVER7_Pin GPIO_PIN_2
#define RECIVER7_GPIO_Port GPIOD
#define RECIVER6_Pin GPIO_PIN_3
#define RECIVER6_GPIO_Port GPIOD
#define RECIVER5_Pin GPIO_PIN_4
#define RECIVER5_GPIO_Port GPIOD
#define RECIVER4_Pin GPIO_PIN_5
#define RECIVER4_GPIO_Port GPIOD
#define RECIVER3_Pin GPIO_PIN_6
#define RECIVER3_GPIO_Port GPIOD
#define RECIVER2_Pin GPIO_PIN_7
#define RECIVER2_GPIO_Port GPIOD
#define RECIVER1_Pin GPIO_PIN_9
#define RECIVER1_GPIO_Port GPIOG
/* USER CODE BEGIN Private defines */

#define UEB_CURRBUF_SIZE	400
#define UEB_CURRBUF_OS1		0
#define UEB_CURRBUF_OS2		400
#define UEB_CURRBUF_OS3		800

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

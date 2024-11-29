/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define P_Sensor_Pin GPIO_PIN_0
#define P_Sensor_GPIO_Port GPIOA
#define PASS_SW_Pin GPIO_PIN_1
#define PASS_SW_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define BL1_Pin GPIO_PIN_4
#define BL1_GPIO_Port GPIOC
#define ADDR4_Pin GPIO_PIN_2
#define ADDR4_GPIO_Port GPIOB
#define D32_Pin GPIO_PIN_12
#define D32_GPIO_Port GPIOB
#define BL2_Pin GPIO_PIN_13
#define BL2_GPIO_Port GPIOB
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define Hirata_NG_Pin GPIO_PIN_15
#define Hirata_NG_GPIO_Port GPIOA
#define ST_BLUE_Pin GPIO_PIN_10
#define ST_BLUE_GPIO_Port GPIOC
#define ST_GREEN_Pin GPIO_PIN_11
#define ST_GREEN_GPIO_Port GPIOC
#define ST_YELLOW_Pin GPIO_PIN_12
#define ST_YELLOW_GPIO_Port GPIOC
#define ST_RED_Pin GPIO_PIN_2
#define ST_RED_GPIO_Port GPIOD
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define ADDR3_Pin GPIO_PIN_4
#define ADDR3_GPIO_Port GPIOB
#define ADDR2_Pin GPIO_PIN_5
#define ADDR2_GPIO_Port GPIOB
#define ADDR1_Pin GPIO_PIN_8
#define ADDR1_GPIO_Port GPIOB
#define ADDR0_Pin GPIO_PIN_9
#define ADDR0_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
extern uint8_t send_buffer[];
extern uint8_t slave_addr[2];
extern uint8_t rec_data[14];		//dane odczytane z karty
extern uint8_t rx_data[64];
extern uint8_t ReceivedData[64]; // A buffer for parsing
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

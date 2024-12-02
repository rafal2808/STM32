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
#include "lwip.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tcpServerRAW.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include <stdio.h>
#include "ring_buffer.h"
#include "parser_simple.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t data[4];
uint8_t rec_d[17];

uint8_t rx_data[32];
uint8_t rx_data_temp;
uint8_t send_buffer[] = {"TS_00_00\n"};
uint8_t ReceviedLines; // Complete lines counter
uint8_t ReceivedData[32]; // A buffer for parsing
uint8_t read_data[25]; // A buffer for parsing

uint8_t ST_RFID_00[25]={"NULL"};
uint8_t ST_RFID_01[25] ={"NULL"};
uint8_t ST_RFID_02[25]={"NULL"};
uint8_t ST_RFID_03[25]={"NULL"};
uint8_t ST_RFID_04[25]={"NULL"};
uint8_t ST_RFID_05[25]={"NULL"};
uint8_t ST_RFID_06[25]={"NULL"};
uint8_t ST_RFID_07[25]={"NULL"};
uint8_t ST_RFID_08[25]={"NULL"};
uint8_t ST_RFID_09[25]={"NULL"};
uint8_t ST_RFID_10[25]={"NULL"};
uint8_t ST_RFID_11[25]={"NULL"};
uint8_t ST_RFID_12[25]={"NULL"};
uint8_t ST_RFID_13[25]={"NULL"};
uint8_t ST_RFID_14[25]={"NULL"};
uint8_t ST_RFID_15[25]={"NULL"};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void RS_Transmit(char* Message);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void UART_Log(char* Message1);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

extern struct netif gnetif;



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	  /* Enable I-Cache---------------------------------------------------------*/
//	  SCB_EnableICache();

	  /* Enable D-Cache---------------------------------------------------------*/
//	  SCB_EnableDCache();
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
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  HAL_Delay(3000);
  MX_USART2_UART_Init();
  MX_USART6_UART_Init();
  MX_LWIP_Init();
  /* USER CODE BEGIN 2 */
  for(size_t i = 0; i < sizeof read_data; ++i)	//clearread data
  	  read_data[i] = 0;

  tcp_server_init();
  UART_Log("INIT OK...\n\r");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  ethernetif_input(&gnetif);
	  sys_check_timeouts();


		 send_buffer[3] = '0';	//adres 00
		 send_buffer[4] = '0';

		 send_buffer[6] = '0';	//commend 00
		 send_buffer[7] = '0';

		 HAL_UART_Receive_IT(&huart6, read_data, 25); //read
		 RS_Transmit(send_buffer);	//send commend

		 HAL_Delay(100);
		 memcpy(ST_RFID_00, read_data, 25);
		 UART_Log(ST_RFID_00);
		 UART_Log("\n");

		 for(size_t i = 0; i < sizeof read_data; ++i)	//clearread data
		  read_data[i] = 0;

		 ethernetif_input(&gnetif);
		 	  sys_check_timeouts();
	/*-------------------------------------------------------------------------------*/

		 send_buffer[3] = '0';	//adres 01
		 send_buffer[4] = '1';

		 send_buffer[6] = '0';	//commend 00
		 send_buffer[7] = '0';

		 HAL_UART_Receive_IT(&huart6, read_data, 25); //read
		 RS_Transmit(send_buffer);	//send commend

		 HAL_Delay(100);
		 memcpy(ST_RFID_01, read_data, 25);
		 UART_Log(ST_RFID_01);

		 for(size_t i = 0; i < sizeof read_data; ++i)	//clearread data
		  read_data[i] = 0;

		 ethernetif_input(&gnetif);
		 	  sys_check_timeouts();
	/*-------------------------------------------------------------------------------*/
		 send_buffer[3] = '0';	//adres 02
		 send_buffer[4] = '2';

		 send_buffer[6] = '0';	//commend 00
		 send_buffer[7] = '0';

		 HAL_UART_Receive_IT(&huart6, read_data, 25); //read
		 RS_Transmit(send_buffer);	//send commend

		 HAL_Delay(100);
		 memcpy(ST_RFID_02, read_data, 25);
		 UART_Log(ST_RFID_02);

		 for(size_t i = 0; i < sizeof read_data; ++i)	//clearread data
		  read_data[i] = 0;
	/*-------------------------------------------------------------------------------*/
		 send_buffer[3] = '0';	//adres 03
		 send_buffer[4] = '3';

		 send_buffer[6] = '0';	//commend 00
		 send_buffer[7] = '0';

		 HAL_UART_Receive_IT(&huart6, read_data, 25); //read
		 RS_Transmit(send_buffer);	//send commend

		 HAL_Delay(100);
		 memcpy(ST_RFID_03, read_data, 25);
		 UART_Log(ST_RFID_03);

		 for(size_t i = 0; i < sizeof read_data; ++i)	//clearread data
		  read_data[i] = 0;

		 ethernetif_input(&gnetif);
		 	  sys_check_timeouts();
	/*-------------------------------------------------------------------------------*/
		 send_buffer[3] = '0';	//adres 04
		 send_buffer[4] = '4';

		 send_buffer[6] = '0';	//commend 00
		 send_buffer[7] = '0';

		 HAL_UART_Receive_IT(&huart6, read_data, 25); //read
		 RS_Transmit(send_buffer);	//send commend

		 HAL_Delay(100);
		 memcpy(ST_RFID_04, read_data, 25);
		 UART_Log(ST_RFID_04);

		 for(size_t i = 0; i < sizeof read_data; ++i)	//clearread data
		  read_data[i] = 0;

		 ethernetif_input(&gnetif);
		 	  sys_check_timeouts();
	/*-------------------------------------------------------------------------------*/
		 send_buffer[3] = '0';	//adres 05
		 send_buffer[4] = '5';

		 send_buffer[6] = '0';	//commend 00
		 send_buffer[7] = '0';

		 HAL_UART_Receive_IT(&huart6, read_data, 25); //read
		 RS_Transmit(send_buffer);	//send commend

		 HAL_Delay(100);
		 memcpy(ST_RFID_05, read_data, 25);
		 UART_Log(ST_RFID_05);

		 for(size_t i = 0; i < sizeof read_data; ++i)	//clearread data
		  read_data[i] = 0;

		 ethernetif_input(&gnetif);
		 	  sys_check_timeouts();
	/*-------------------------------------------------------------------------------*/
		 send_buffer[3] = '0';	//adres 06
		 send_buffer[4] = '6';

		 send_buffer[6] = '0';	//commend 00
		 send_buffer[7] = '0';

		 HAL_UART_Receive_IT(&huart6, read_data, 25); //read
		 RS_Transmit(send_buffer);	//send commend

		 HAL_Delay(100);
		 memcpy(ST_RFID_06, read_data, 25);
		 UART_Log(ST_RFID_06);

		 for(size_t i = 0; i < sizeof read_data; ++i)	//clearread data
		  read_data[i] = 0;
	/*-------------------------------------------------------------------------------*/
		 send_buffer[3] = '0';	//adres 07
		 send_buffer[4] = '7';

		 send_buffer[6] = '0';	//commend 00
		 send_buffer[7] = '0';

		 HAL_UART_Receive_IT(&huart6, read_data, 25); //read
		 RS_Transmit(send_buffer);	//send commend

		 HAL_Delay(100);
		 memcpy(ST_RFID_07, read_data, 25);
		 UART_Log(ST_RFID_07);

		 for(size_t i = 0; i < sizeof read_data; ++i)	//clearread data
		  read_data[i] = 0;

		 ethernetif_input(&gnetif);
		 sys_check_timeouts();
	/*-------------------------------------------------------------------------------*/
		 send_buffer[3] = '0';	//adres 08
		 send_buffer[4] = '8';

		 send_buffer[6] = '0';	//commend 00
		 send_buffer[7] = '0';

		 HAL_UART_Receive_IT(&huart6, read_data, 25); //read
		 RS_Transmit(send_buffer);	//send commend

		 HAL_Delay(100);
		 memcpy(ST_RFID_08, read_data, 25);
		 UART_Log(ST_RFID_08);

		 for(size_t i = 0; i < sizeof read_data; ++i)	//clearread data
		  read_data[i] = 0;

		 ethernetif_input(&gnetif);
		 sys_check_timeouts();
	/*-------------------------------------------------------------------------------*/
		 send_buffer[3] = '0';	//adres 09
		 send_buffer[4] = '9';

		 send_buffer[6] = '0';	//commend 00
		 send_buffer[7] = '0';

		 HAL_UART_Receive_IT(&huart6, read_data, 25); //read
		 RS_Transmit(send_buffer);	//send commend

		 HAL_Delay(100);
		 memcpy(ST_RFID_09, read_data, 25);
		 UART_Log(ST_RFID_09);

		 for(size_t i = 0; i < sizeof read_data; ++i)	//clearread data
		  read_data[i] = 0;

		 ethernetif_input(&gnetif);
		 sys_check_timeouts();
	/*-------------------------------------------------------------------------------*/
		 send_buffer[3] = '1';	//adres 10
		 send_buffer[4] = '0';

		 send_buffer[6] = '0';	//commend 00
		 send_buffer[7] = '0';

		 HAL_UART_Receive_IT(&huart6, read_data, 25); //read
		 RS_Transmit(send_buffer);	//send commend

		 HAL_Delay(100);
		 memcpy(ST_RFID_10, read_data, 25);
		 UART_Log(ST_RFID_10);

		 for(size_t i = 0; i < sizeof read_data; ++i)	//clearread data
		  read_data[i] = 0;

		 ethernetif_input(&gnetif);
		 sys_check_timeouts();
	/*-------------------------------------------------------------------------------*/
		 send_buffer[3] = '1';	//adres 11
		 send_buffer[4] = '1';

		 send_buffer[6] = '0';	//commend 00
		 send_buffer[7] = '0';

		 HAL_UART_Receive_IT(&huart6, read_data, 25); //read
		 RS_Transmit(send_buffer);	//send commend

		 HAL_Delay(100);
		 memcpy(ST_RFID_11, read_data, 25);
		 UART_Log(ST_RFID_11);

		 for(size_t i = 0; i < sizeof read_data; ++i)	//clearread data
		  read_data[i] = 0;

		 ethernetif_input(&gnetif);
		 sys_check_timeouts();
	/*-------------------------------------------------------------------------------*/
		 send_buffer[3] = '1';	//adres 12
		 send_buffer[4] = '2';

		 send_buffer[6] = '0';	//commend 00
		 send_buffer[7] = '0';

		 HAL_UART_Receive_IT(&huart6, read_data, 25); //read
		 RS_Transmit(send_buffer);	//send commend

		 HAL_Delay(100);
		 memcpy(ST_RFID_12, read_data, 25);
		 UART_Log(ST_RFID_12);

		 for(size_t i = 0; i < sizeof read_data; ++i)	//clearread data
		  read_data[i] = 0;

		 ethernetif_input(&gnetif);
		 sys_check_timeouts();
	/*-------------------------------------------------------------------------------*/
		 send_buffer[3] = '1';	//adres 13
		 send_buffer[4] = '3';

		 send_buffer[6] = '0';	//commend 00
		 send_buffer[7] = '0';

		 HAL_UART_Receive_IT(&huart6, read_data, 25); //read
		 RS_Transmit(send_buffer);	//send commend

		 HAL_Delay(100);
		 memcpy(ST_RFID_13, read_data, 25);
		 UART_Log(ST_RFID_13);

		 for(size_t i = 0; i < sizeof read_data; ++i)	//clearread data
		  read_data[i] = 0;

		 ethernetif_input(&gnetif);
		 sys_check_timeouts();
	/*-------------------------------------------------------------------------------*/
		 send_buffer[3] = '1';	//adres 14
		 send_buffer[4] = '4';

		 send_buffer[6] = '0';	//commend 00
		 send_buffer[7] = '0';

		 HAL_UART_Receive_IT(&huart6, read_data, 25); //read
		 RS_Transmit(send_buffer);	//send commend

		 HAL_Delay(100);
		 memcpy(ST_RFID_14, read_data, 25);
		 UART_Log(ST_RFID_14);

		 for(size_t i = 0; i < sizeof read_data; ++i)	//clearread data
		  read_data[i] = 0;

		 ethernetif_input(&gnetif);
		 sys_check_timeouts();
	/*-------------------------------------------------------------------------------*/
		 send_buffer[3] = '1';	//adres 15
		 send_buffer[4] = '5';

		 send_buffer[6] = '0';	//commend 00
		 send_buffer[7] = '0';

		 HAL_UART_Receive_IT(&huart6, read_data, 25); //read
		 RS_Transmit(send_buffer);	//send commend

		 HAL_Delay(100);
		 memcpy(ST_RFID_15, read_data, 25);
		 UART_Log(ST_RFID_15);

		 for(size_t i = 0; i < sizeof read_data; ++i)	//clearread data
		  read_data[i] = 0;

		 ethernetif_input(&gnetif);
		 	  sys_check_timeouts();
	/*-------------------------------------------------------------------------------*/
		 HAL_Delay(100);
  }
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

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
//void DATA_Transmit(uint8_t command, uint8_t data_rec)
//{
//	 HAL_UART_Receive_IT(&huart6, read_data, 25); //read
//	 RS_Transmit(command);	//send commend
//
//	 HAL_Delay(100);
//	 memcpy(data_rec, read_data, 25);
//	 //UART_Log(ST_RFID_07);
//
//	 for(size_t i = 0; i < sizeof read_data; ++i)	//clearread data
//	  read_data[i] = 0;
//}



void UART_Log(char* Message1)
{
	HAL_UART_Transmit(&huart3, (uint8_t*)Message1, strlen(Message1), 1000);
}

void RS_Transmit(char* Message)
{
HAL_UART_Transmit_IT(&huart6, (uint8_t*)Message, strlen(Message));
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{


}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

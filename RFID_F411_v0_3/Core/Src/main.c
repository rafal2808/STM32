/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "rc522.h"
#include "string.h"
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

//zmienne z biblioteki pod rc522
uint8_t request_status;
uint8_t anticoll_status;
uint8_t selecttag_status;
uint8_t auth_status;
uint8_t read_status;
uint8_t str[4]; 	// Max_LEN = 16
uint8_t rec[14];	// chwilowe odczytanie jakby
uint8_t sectorkey[6]= {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

uint8_t rec_data[14];		//dane odczytane z karty
uint8_t read_flag =1;		//0 dana odczytana, na chwile zeby wyslac na usb
bool addr_d[4];
uint8_t slave_addr[2]={0,0};
const bool liczba0[4]={0,0,0,0};
const bool liczba1[4]={0,0,0,1};
const bool liczba2[4]={0,0,1,0};
const bool liczba3[4]={0,0,1,1};
const bool liczba4[4]={0,1,0,0};
const bool liczba5[4]={0,1,0,1};
const bool liczba6[4]={0,1,1,0};
const bool liczba7[4]={0,1,1,1};
const bool liczba8[4]={1,0,0,0};
const bool liczba9[4]={1,0,0,1};
const bool liczba10[4]={1,0,1,0};
const bool liczba11[4]={1,0,1,1};
const bool liczba12[4]={1,1,0,0};
const bool liczba13[4]={1,1,0,1};
const bool liczba14[4]={1,1,1,0};
const bool liczba15[4]={1,1,1,1};

bool sw_pass_s;
uint8_t rx_data[64];
uint8_t rx_data_temp;
uint8_t sw_status;
uint8_t send_buffer[] = {"TM_00_P_S_PAL_Linia_X_XX\n"};
uint8_t ReceviedLines; // Complete lines counter
uint8_t ReceivedData[64]; // A buffer for parsing



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void UART_Log(char* Message);
void RFID_Read();
void RS_Transmit(char* Message);
void ADDR_Check();
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  //ledy wylaczone
  HAL_GPIO_WritePin(ST_RED_GPIO_Port, ST_RED_Pin, GPIO_PIN_SET);	//error

  HAL_GPIO_WritePin(ST_BLUE_GPIO_Port, ST_BLUE_Pin, GPIO_PIN_SET);	//other

  UART_Log("RFID ver. 0.3\n\r");
  ADDR_Check();
  send_buffer[3] = slave_addr[0];
  send_buffer[4] = slave_addr[1];
  HAL_Delay(1500);

  HAL_GPIO_WritePin(ST_RED_GPIO_Port, ST_RED_Pin, GPIO_PIN_RESET);	//error
  HAL_GPIO_WritePin(ST_BLUE_GPIO_Port, ST_BLUE_Pin, GPIO_PIN_RESET);	//other




  //send_status_buffer = "AT_00_"

  HAL_UART_Receive_IT(&huart6, &rx_data_temp, 1);
  //send_buffer[24] = ('\n');
  UART_Log("INIT OK...\n\r");



  //HAL_GPIO_WritePin(ST_YELLOW_GPIO_Port, ST_YELLOW_Pin, GPIO_PIN_SET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //HAL_UART_Receive_IT(&huart6, &rx_data_temp, 1);
	  //for(size_t i = 0; i < sizeof rx_data; ++i)	//clearread data
		//  rx_data[i] = 0;

	  //sw_pass_s = HAL_GPIO_ReadPin(PASS_SW_GPIO_Port, PASS_SW_Pin);

	  //if(sw_pass_s == true)
	  //{
		  //pass
		  //HAL_UART_Receive_DMA(&huart6, &rx_data, 4);
		  //UART_Log("SW_PASS status = PASS\n\r");
		  send_buffer[6] = 'P';
		  read_flag = 1;
		  RFID_Read();
		  //HAL_Delay(5);
	  //}
	  //else
	  //{
		  //bypass
		 // UART_Log("SW_PASS status = BYPASS\n\r");
		 // send_buffer[6] = 'B';
		 // while(1)
		 // {
		//	  sw_pass_s = HAL_GPIO_ReadPin(PASS_SW_GPIO_Port, PASS_SW_Pin);
		//	  HAL_Delay(10);
			  //HAL_UART_Receive_DMA(&huart6, rx_data, 4);

			//  if(sw_pass_s == true)
			//  {
			//	  sw_status = 1; //PASS
				  //UART_Log("SW_PASS status = PASS\n\r");
			//	  break;
			 // }
			 // else
			 // {
				//  sw_status = 0; //BYPASS
				 // send_buffer[6] = 'B';
				 // UART_Log("SW_PASS status = BYPASS\n\r");

				  if(ReceviedLines > 0)
				 	  {
				 		  // Take one line from the Ring Buffer to work-buffer
				 		  Parser_TakeLine(&rx_data, ReceivedData);

				 		  // Decrement complete lines counter
				 		  ReceviedLines--;

				 		  // Run the parser with work-buffer
				 		  Parser_Parse(ReceivedData);
				 		  //RB_Flush(&rx_data);
				 	  }
				 // continue;
			  //}
		 // }
	 // }

	 // if(ReceviedLines > 0)
	 	//  {
	 		  // Take one line from the Ring Buffer to work-buffer
	 		//  Parser_TakeLine(&rx_data, ReceivedData);

	 		  // Decrement complete lines counter
	 		//  ReceviedLines--;

	 		  // Run the parser with work-buffer
	 		 // Parser_Parse(ReceivedData);
	 		  //RB_Flush(&rx_data);
	 	  //}

  }



    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if(huart->Instance == USART6)
	{
		//RB_Write(&rx_data, rx_data_temp);
		//HAL_UART_Receive_IT(&huart6, &rx_data_temp, 1);


		// Try to write into the Ring Buffer
		if(RB_OK == RB_Write(&rx_data, rx_data_temp))
		{
			// Check if current byte is the endline char
			if(rx_data_temp == ENDLINE)
			{
				// Increment complete lines
				ReceviedLines++;
			}
		}

		//if(rx_data[31] != 0)
		//{
		//	RB_Flush(&rx_data);
		//}
		// Start to listening UART again - important!
		HAL_UART_Receive_IT(&huart6, &rx_data_temp, 1);


	}
}

void UART_Log(char* Message1)
{
	HAL_UART_Transmit(&huart2, (uint8_t*)Message1, strlen(Message1), 1000);
}


void RS_Transmit(char* Message)
{
	//HAL_UART_Transmit_IT(&huart6, (uint8_t*)Message, strlen(Message));
	HAL_UART_Transmit(&huart6, (uint8_t*)Message, strlen(Message), 1000);
}


void RFID_Read()
{
	  MFRC522_Init();
	  	HAL_Delay(5);
	request_status = MFRC522_Request(PICC_REQIDL, str);
	//HAL_Delay(5);

	if(request_status == 0){
		request_status = 2;
		HAL_GPIO_WritePin(ST_BLUE_GPIO_Port, ST_BLUE_Pin, GPIO_PIN_SET);	//other
		//HAL_Delay(5);
	anticoll_status = MFRC522_Anticoll(str);
	//HAL_Delay(5);
	selecttag_status = MFRC522_SelectTag(str);
	//HAL_Delay(5);

	auth_status = MFRC522_Auth(0x60,20,sectorkey,str);
	//HAL_Delay(5);

	//if(auth_status == 0){
	read_status = MFRC522_Read(20, rec);
	//HAL_Delay(5);
	MFRC522_Halt();
	//HAL_Delay(5);



	//if(read_status == 0)
	//	{
		 HAL_GPIO_WritePin(ST_BLUE_GPIO_Port, ST_BLUE_Pin, GPIO_PIN_SET);	//other
			memcpy(rec_data, rec, 14);
			//UART_Log("Read Tag: ");
			//UART_Log(rec_data);
			//UART_Log("\n\r");

			send_buffer[20] = rec_data[10];
			send_buffer[22] = rec_data[12];
			send_buffer[23] = rec_data[13];
			//send_buffer[5] = rec_data[3];
//			send_buffer[6] = rec_data[4];
//			send_buffer[7] = rec_data[5];
//			send_buffer[8] = rec_data[6];
//			send_buffer[9] = rec_data[7];
//			send_buffer[10] = rec_data[8];
//			send_buffer[11] = rec_data[9];
//			send_buffer[12] = rec_data[10];
//			send_buffer[13] = rec_data[11];
//			send_buffer[14] = rec_data[12];
//			send_buffer[15] = rec_data[13];
			// HAL_GPIO_WritePin(ST_BLUE_GPIO_Port, ST_BLUE_Pin, GPIO_PIN_RESET);	//other
			HAL_GPIO_WritePin(ST_BLUE_GPIO_Port, ST_BLUE_Pin, GPIO_PIN_RESET);	//other
		}
	//MFRC522_Halt();
		//HAL_Delay(1);

}

void ADDR_Check()
{
	  addr_d[0] = HAL_GPIO_ReadPin(ADDR0_GPIO_Port, ADDR0_Pin);
	  addr_d[1] = HAL_GPIO_ReadPin(ADDR1_GPIO_Port, ADDR1_Pin);
	  addr_d[2] = HAL_GPIO_ReadPin(ADDR2_GPIO_Port, ADDR2_Pin);
	  addr_d[3] = HAL_GPIO_ReadPin(ADDR3_GPIO_Port, ADDR3_Pin);

	  UART_Log("Addr: ");

	  if(addr_d[0] == liczba0[3] && addr_d[1] == liczba0[2] && addr_d[2] == liczba0[1] && addr_d[3] == liczba0[0])
	  {
		  slave_addr[0] = '0';
		  slave_addr[1] = '0';
		  UART_Log("0x00");
	  }
	  else if(addr_d[0] == liczba1[3] && addr_d[1] == liczba1[2] && addr_d[2] == liczba1[1] && addr_d[3] == liczba1[0])
	  {
		  slave_addr[0] = '0';
		  slave_addr[1] = '1';
		  UART_Log("0x01");
	  }
	  else if(addr_d[0] == liczba2[3] && addr_d[1] == liczba2[2] && addr_d[2] == liczba2[1] && addr_d[3] == liczba2[0])
	  {
		  slave_addr[0] = '0';
		  slave_addr[1] = '2';
		  UART_Log("0x02");
	  }
	  else if(addr_d[0] == liczba3[3] && addr_d[1] == liczba3[2] && addr_d[2] == liczba3[1] && addr_d[3] == liczba3[0])
	  {
		  slave_addr[0] = '0';
		  slave_addr[1] = '3';
		  UART_Log("0x03");
	  }
	  else if(addr_d[0] == liczba4[3] && addr_d[1] == liczba4[2] && addr_d[2] == liczba4[1] && addr_d[3] == liczba4[0])
	  {
		  slave_addr[0] = '0';
		  slave_addr[1] = '4';
		  UART_Log("0x04");
	  }
	  else if(addr_d[0] == liczba5[3] && addr_d[1] == liczba5[2] && addr_d[2] == liczba5[1] && addr_d[3] == liczba5[0])
	  {
		  slave_addr[0] = '0';
		  slave_addr[1] = '5';
		  UART_Log("0x05");
	  }
	  else if(addr_d[0] == liczba6[3] && addr_d[1] == liczba6[2] && addr_d[2] == liczba6[1] && addr_d[3] == liczba6[0])
	  {
		  slave_addr[0] = '0';
		  slave_addr[1] = '6';
		  UART_Log("0x06");
	  }
	  else if(addr_d[0] == liczba7[3] && addr_d[1] == liczba7[2] && addr_d[2] == liczba7[1] && addr_d[3] == liczba7[0])
	  {
		  slave_addr[0] = '0';
		  slave_addr[1] = '7';
		  UART_Log("0x07");
	  }
	  else if(addr_d[0] == liczba8[3] && addr_d[1] == liczba8[2] && addr_d[2] == liczba8[1] && addr_d[3] == liczba8[0])
	  {
		  slave_addr[0] = '0';
		  slave_addr[1] = '8';
		  UART_Log("0x08");
	  }
	  else if(addr_d[0] == liczba9[3] && addr_d[1] == liczba9[2] && addr_d[2] == liczba9[1] && addr_d[3] == liczba9[0])
	  {
		  slave_addr[0] = '0';
		  slave_addr[1] = '9';
		  UART_Log("0x09");
	  }
	  else if(addr_d[0] == liczba10[3] && addr_d[1] == liczba10[2] && addr_d[2] == liczba10[1] && addr_d[3] == liczba10[0])
	  {
		  slave_addr[0] = '1';
		  slave_addr[1] = '0';
		  UART_Log("0x0A");
	  }
	  else if(addr_d[0] == liczba11[3] && addr_d[1] == liczba11[2] && addr_d[2] == liczba11[1] && addr_d[3] == liczba11[0])
	  {
		  slave_addr[0] = '1';
		  slave_addr[1] = '1';
		  UART_Log("0x0B");
	  }
	  else if(addr_d[0] == liczba12[3] && addr_d[1] == liczba12[2] && addr_d[2] == liczba12[1] && addr_d[3] == liczba12[0])
	  {
		  slave_addr[0] = '1';
		  slave_addr[1] = '2';
		  UART_Log("0x0C");
	  }
	  else if(addr_d[0] == liczba13[3] && addr_d[1] == liczba13[2] && addr_d[2] == liczba13[1] && addr_d[3] == liczba13[0])
	  {
		  slave_addr[0] = '1';
		  slave_addr[1] = '3';
		  UART_Log("0x0D");
	  }
	  else if(addr_d[0] == liczba14[3] && addr_d[1] == liczba14[2] && addr_d[2] == liczba14[1] && addr_d[3] == liczba14[0])
	  {
		  slave_addr[0] = '1';
		  slave_addr[1] = '4';
		  UART_Log("0x0E");
	  }
	  else if(addr_d[0] == liczba15[3] && addr_d[1] == liczba15[2] && addr_d[2] == liczba15[1] && addr_d[3] == liczba15[0])
	  {
		  slave_addr[0] = '1';
		  slave_addr[1] = '5';
		  UART_Log("0x0F");
	  }
	  else
	  {
		  while(true)
		  {
		  slave_addr[0] = 'F';
		  slave_addr[1] = 'F';
		  UART_Log("0xFF incorrect addres check\n\r");
		  HAL_GPIO_WritePin(ST_RED_GPIO_Port, ST_RED_Pin, GPIO_PIN_SET);	//error
		  HAL_GPIO_WritePin(ST_YELLOW_GPIO_Port, ST_YELLOW_Pin, GPIO_PIN_RESET); //bypass
		  HAL_GPIO_WritePin(ST_GREEN_GPIO_Port, ST_GREEN_Pin, GPIO_PIN_RESET);	//run
		  HAL_GPIO_WritePin(ST_BLUE_GPIO_Port, ST_BLUE_Pin, GPIO_PIN_RESET);	//other
		  HAL_GPIO_WritePin(Hirata_NG_GPIO_Port, Hirata_NG_Pin, GPIO_PIN_RESET);
		  HAL_Delay(1000);
		  }
	  }

	  UART_Log("\n\r");
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

	  UART_Log("Hard Fault: ERROR HANDLER\n\r");
	  HAL_GPIO_WritePin(ST_RED_GPIO_Port, ST_RED_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(Hirata_NG_GPIO_Port, Hirata_NG_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(ST_YELLOW_GPIO_Port, ST_YELLOW_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(ST_GREEN_GPIO_Port, ST_GREEN_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(ST_BLUE_GPIO_Port, ST_BLUE_Pin, GPIO_PIN_RESET);
	  HAL_Delay(1000);
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

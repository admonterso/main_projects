/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "FLASH_PAGE_F1.h"
#include "quectelCommands.h"
#include "liquidcrystal_i2c.h"
#include "GEOtextLib.h"
#include <string.h>
#include <stdlib.h>

#define address 0x0800C000
//#define address 0x080056A0
#define versionAdress 0x0800BFF0
//#define versionAdress 0x080056A0
#define bytesToRead 960
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define currentTerminalADRR 0x0800B000
#define currentTerminal 164522982240839
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
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
uint8_t flag = 0; // flag for operating fleshing used in UART callback
uint8_t data[bytesToRead]; // main data that need to be flehed
uint32_t testData[] = {0xB9337823, 0x237833B9}; // test data
uint32_t Adress = address; // for address increment
uint32_t state; // for buttons
uint8_t updateChars[8]; // update char store
uint8_t MQTT_CHECK_DATA[180];
uint8_t MQTT_GOT_DATA[100];
uint8_t c; // this stores the input chars from uart to find start and the end of input data
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if(flag == 0){
		if(c != '<'){
			HAL_UART_Receive_IT(&huart1, &c, 1);
		}
		else if(c == '<'){

			flag = 1;
		}

	}

}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint32_t convertStringToHex(uint8_t* str) {
    uint32_t hexValue = 0;
    int i = 0;

    // Iterate over each character in the string
    while (i < 8) {
        hexValue <<= 4;  // Shift left by 4 bits

        // Convert character to hexadecimal digit
        if (str[i] >= '0' && str[i] <= '9') {
            hexValue |= (str[i] - '0');
        } else if (str[i] >= 'A' && str[i] <= 'F') {
            hexValue |= (str[i] - 'A' + 10);
        } else if (str[i] >= 'a' && str[i] <= 'f') {
            hexValue |= (str[i] - 'a' + 10);
        } else {
            // Invalid character, handle error
            //printf("Invalid character: %c\n", str[i]);
            return 0;
        }

        i++;
    }

    return hexValue;
}

char* convertNumberToCharArray(uint64_t number) {
    // Count the number of digits in the number
    uint64_t temp = number;
    int numDigits = 1;
    while (temp /= 10) {
        numDigits++;
    }

    // Allocate memory for the character array (+1 for null-terminator)
    char* buffer = (char*)malloc((numDigits + 1) * sizeof(char));
    if (buffer == NULL) {
        // Error in memory allocation
        return NULL;
    }

    // Convert each digit to its corresponding character representation
    int i = numDigits - 1;
    while (number != 0) {
        buffer[i--] = '0' + (number % 10);
        number /= 10;
    }

    buffer[numDigits] = '\0'; // Null-terminate the character array

    return buffer;
}

uint32_t convertIntToHex(int num) {
    uint32_t hexValue = 0;

    // Find the position of the leftmost non-zero bit
    int shift = 0;
    while ((num >> shift) > 0) {
        shift += 4;
    }
    shift -= 4;  // Adjust the shift value to the leftmost non-zero nibble

    // Iterate over each nibble in the integer
    while (shift >= 0) {
        hexValue |= ((num >> shift) & 0xF) << shift;  // Extract the nibble and shift it to the correct position
        shift -= 4;  // Move to the next nibble
    }

    return hexValue;
}

void fleshArray(uint8_t* array) { // takes 960 bytes of data and fleshes it at start address which is defined at start
        int count = 0;
        uint8_t data[8];
        uint32_t hexVal;
        uint32_t hexVals[4];

        int size = strlen((char*)array);

        memcpy(data, array, 8);
        hexVal = convertStringToHex(data);
        hexVals[count] = hexVal;
        count++;

    for (int i = 0; i < size; i++) {

        if((i+1) % 8 == 0){

            memcpy(data, array + (i+1), 8);
            hexVal = convertStringToHex(data);
            hexVals[count] = hexVal;

            count++;
            if(count >= 4){
                //for(int i = 0; i<4; i++) printf("%08X ", hexVals[i]);
            	Flash_Write_Data(Adress,hexVals, 4);

            	Adress+=16;
                count = 0;
            }
//
        }

    }

}



void jumpToAddress(uint32_t ADDRESSTOGO){
	  uint32_t addresstojump;

	  addresstojump = *((volatile uint32_t*)(ADDRESSTOGO + 4));
	  HAL_DeInit();
	  HAL_RCC_DeInit();
	  void (*GoToApp)(void);
	  GoToApp = (void (*) (void))addresstojump;
	  __disable_irq();
	  __set_MSP(*((volatile uint32_t*)ADDRESSTOGO));
	  GoToApp();
}

void led(uint8_t n) {
	for (uint8_t i = 0; i < n; i++) {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);    // LED1 ON
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);    // LED1 OFF
		HAL_Delay(100);
	}
}

void quectelInit(){
	  memset(data, 0, sizeof(data));

	 checkQueqtelWithNoEcho(data);
	  HD44780_PrintStr("#");
	  memset(data, 0, sizeof(data));

	 ckeckQuectelSignalWithNoEcho(data);
	  HD44780_PrintStr("#");
	  memset(data, 0, sizeof(data));

	 MQTTRecMod(data);
	  HD44780_PrintStr("#");
	  memset(data, 0, sizeof(data));

	 MQTTOpenPort(data);
	  HD44780_PrintStr("##");
	  memset(data, 0, sizeof(data));

	 MQTTConnect(data);
	  HD44780_PrintStr("##");
	  memset(data, 0, sizeof(data));

	 MQTTSubToTopic(data);

	  //memset(buffer, 0, sizeof(buffer));

	  //MQTTPubToTopic(buffer, MQTT_EXMP_DATA);
	  memset(data, 0, sizeof(data));

      HAL_Delay(500);
}


uint64_t * flashCurTerminal(uint64_t DATA, uint32_t ADRESS){


	uint64_t FData = DATA;

	eraseFlashRange(ADRESS, ADRESS + 1);

	HAL_FLASH_Unlock();
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,ADRESS, FData);
	HAL_FLASH_Lock();

	uint64_t * RDAddr = (uint64_t *)  ADRESS;
	return RDAddr;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	__disable_irq();
	SCB->VTOR = 0x8000000;
	__enable_irq();
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
 // uint32_t *ptr = data;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  HD44780_Init(2);
  printItvirteba(0, 3);

//  eraseFlashRange(currentTerminalADRR, currentTerminalADRR + 4);
//
//  HAL_FLASH_Unlock();
//
//  HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, currentTerminalADRR, 1);
//
//  HAL_FLASH_Lock();
//
//  uint64_t terminal =  *(__IO uint64_t *)currentTerminalADRR;

  HD44780_SetCursor(0, 1);
  for(int i = 0;i<2; i++){
	  quectelInit();
  }

  uint32_t * versionPTR = (uint32_t *)  versionAdress;
  uint32_t version = *versionPTR; // for version check

  uint64_t terminalID = *flashCurTerminal(currentTerminal, currentTerminalADRR);

  if(version == 0xFFFFFFFF || version == 0){
	  	eraseFlashRange(versionAdress, versionAdress + 4);
		HAL_FLASH_Unlock();
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,versionAdress, 1);
		HAL_FLASH_Lock();
  }

  version = *versionPTR;



  char * terminalStr = convertNumberToCharArray(terminalID);

  //sprintf((char*) MQTT_CHECK_DATA, "{\"operationType\":\"check\",\"content\":{\"terminalID\":\"164522982240839\",\"firmwareVersion\":1}}");
  sprintf((char*)MQTT_CHECK_DATA, "{\"operationType\":\"check\",\"content\":{\"terminalID\":\"%s\",\"firmwareVersion\":%ld}}", terminalStr, version);

  sprintf((char*)MQTT_GOT_DATA, "{\"operationType\":\"acknowledge\",\"content\":{\"status\":\"success\",\"terminalID\":\"%s\"}}", terminalStr);
  MQTTPubToTopic(strlen((char*)MQTT_CHECK_DATA));

  uint8_t checkCon = 0;
  uint8_t printDownCount = 0;
  uint8_t printDownCountAcum = 0;

  HAL_UART_Transmit(&huart1, MQTT_CHECK_DATA, strlen((char*)MQTT_CHECK_DATA), 50);
  HD44780_PrintStr("#");



  HAL_UART_Receive_IT(&huart1, &c, 1);
  int T = HAL_GetTick();
 //eraseFlashRange(0x0800C000, 0x0800D2D0);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(flag == 1){
		  uint8_t flesh = 0;
		  checkCon = 1;
		  HAL_UART_Receive(&huart1, data, bytesToRead, 200);
		  if(data[0] == 'E' && data[1] == 'N' && data[2] == 'D'){ // if the data == END
			  memcpy(updateChars, data + 4, 8);
			  int x = atoi((char*)updateChars);
			  eraseFlashRange(versionAdress, versionAdress + 4);
			  HAL_FLASH_Unlock();
			  HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,versionAdress, x);
			  HAL_FLASH_Lock();
			  flesh = 1;
			  jumpToAddress(address);
		  }
		  if(data[0] == '2' && data[1] == '0' && data[2] == '0' && data[3] == 'R'){ // if the data == END
		  			  jumpToAddress(address);
		  }

		  if(data[0] == 'E' && data[1] == 'R' && data[2] == 'A' && data[3] == 'S' && data[4] == 'E'){ // if the data == END
			  printAxldeba(0, 4);
			  HD44780_SetCursor(0, 1);
			  memcpy(updateChars, data + 6, 8);
			  uint32_t endNewProg = convertStringToHex(updateChars);
			  eraseFlashRange(address, endNewProg);
			  flesh = 1;
		  }
		  if(flesh == 0){
			  fleshArray(data);
			  printDownCount++;

			  if(printDownCount > printDownCountAcum + 3){
				   HD44780_PrintStr("#");
				   printDownCountAcum = printDownCount;
			  }

		  }

		  MQTTPubToTopic(strlen((char*)MQTT_GOT_DATA));
		  HAL_UART_Transmit(&huart1, MQTT_GOT_DATA, strlen((char*)MQTT_GOT_DATA), 100);
		  //if(flesh == 1)HAL_NVIC_SystemReset();
		  memset(data, 0, sizeof(data));
		  HAL_UART_Receive_IT(&huart1, &c, 1);
		  HAL_UART_Receive_IT(&huart1, &c, 1);


		  //HAL_UART_Transmit(&huart1, data, bytesToRead + 1, 1000);
		  flag = 0;

	  }
	  state = HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_4);
	  if(state == 0){
		  eraseFlashRange(0x0800C000, 0x0800D2D0);
		  led(10);
	  }
	  state = HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_5);
	  if(state == 0){
		  jumpToAddress(address);
		  led(10);
	  }

	  if(checkCon == 0 && HAL_GetTick() - T >= 10000)HAL_NVIC_SystemReset();
	  if(HAL_GetTick() - T >= 2*(60000))HAL_NVIC_SystemReset();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB4 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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

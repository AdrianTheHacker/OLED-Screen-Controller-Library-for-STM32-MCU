/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t programState = 0;
const uint8_t maxProgramState = 2;

const uint8_t SSD1306SlaveAddressWriteMode = 0x78;
const uint8_t SSD1306SlaveAddressReadMode = 0x79;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
void writeSSD1306Commands(uint8_t commands[], uint8_t commandsLength, uint8_t slaveAddress, I2C_HandleTypeDef* I2CHandler);
void writeSSD1306Data(uint8_t data[], uint8_t dataLength, uint8_t slaveAddress, I2C_HandleTypeDef* I2CHandler);
void initializeScreen(uint8_t slaveAddress, I2C_HandleTypeDef* I2CHandler);
void testScreen(uint8_t slaveAddress, I2C_HandleTypeDef* I2CHandler);
void eraseScreen(uint8_t slaveAddress, I2C_HandleTypeDef* I2CHandler);
void fillScreen(uint8_t slaveAddress, I2C_HandleTypeDef* I2CHandler);
void drawText(char message[], uint8_t messageLengthInCharacters, uint8_t fontSize, uint8_t topLeftXCoordinate, uint8_t topLeftYCoordiante, uint8_t slaveAddress, I2C_HandleTypeDef* I2CHandler);
//void drawBitmap(uint8_t* bitMap, uint8_t bitMapLength, uint8_t bitMapHeight, uint8_t topLeftXCoordinate, uint8_t topLeftYCoordinate);
//void drawLine(uint8_t point1Coordinates[], uint8_t point2Coordinates[]);
//void drawPolygon(uint8_t xCoordinates[], uint8_t yCoordinates[], uint8_t numberOfPoints);
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
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  initializeScreen(SSD1306SlaveAddressWriteMode, &hi2c1);
  testScreen(SSD1306SlaveAddressWriteMode, &hi2c1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	if(programState == 0) {
		eraseScreen(SSD1306SlaveAddressWriteMode, &hi2c1);
	}

	if(programState == 1) {
		fillScreen(SSD1306SlaveAddressWriteMode, &hi2c1);
		HAL_Delay(300);
		eraseScreen(SSD1306SlaveAddressWriteMode, &hi2c1);
		HAL_Delay(300);
	}

	if(programState == 2) {
		char testMessage[] = "ABC DEF GHI JKL MNO PQR STU VWX YZ";
		drawText(testMessage, 34, 8, 0, 0, SSD1306SlaveAddressWriteMode, &hi2c1);
	}

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
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
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIOPin) {
	if(GPIOPin != B1_Pin) {
		__NOP();
		return;
	}

	eraseScreen(SSD1306SlaveAddressWriteMode, &hi2c1);
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	if(programState >= maxProgramState) {
		programState = 0;
		return;
	}

	programState += 1;
}

// OLED Screen Controller Functions
void writeSSD1306Commands(uint8_t commands[], uint8_t commandsLength, uint8_t slaveAddress, I2C_HandleTypeDef* I2CHandler) {
	uint8_t buffer[commandsLength + 1];
	buffer[0] = 0x00;

	for (int i = 0; i < commandsLength; i++) {
		buffer[i + 1] = commands[i];
	}

	HAL_I2C_Master_Transmit(I2CHandler, slaveAddress, buffer, commandsLength + 1, HAL_MAX_DELAY);
}

void writeSSD1306Data(uint8_t data[], uint8_t dataLength, uint8_t slaveAddress, I2C_HandleTypeDef* I2CHandler) {
	uint8_t buffer[dataLength + 1];
	buffer[0] = 0x40;

	for (int i = 0; i < dataLength; i++) {
		buffer[i + 1] = data[i];
	}

	HAL_I2C_Master_Transmit(I2CHandler, slaveAddress, buffer, dataLength + 1, HAL_MAX_DELAY);
}

void initializeScreen(uint8_t slaveAddress, I2C_HandleTypeDef* I2CHandler) {
	uint8_t initializerCommands[] = {
		0xAE,       // Display OFF
		0x20, 0x00, // Set Memory Addressing Mode to Horizontal
		0xA1,       // Segment re-map (column address 127 is mapped to SEG0)
		0xC8,       // COM Output Scan Direction: re-mapped mode
		0xA8, 0x3F, // Multiplex Ratio: 0x3F = 63
		0xD3, 0x00, // Display Offset: 0
		0xDA, 0x12, // COM Pins Hardware Configuration
		0x81, 0x7F, // Contrast
		0xA4,       // Display follows RAM content
		0xA6,       // Normal (not inverted) display
		0xD5, 0x80, // Display clock divide ratio
		0x8D, 0x14, // Charge Pump
		0xAF        // Display ON
	};

	writeSSD1306Commands(initializerCommands, 21, slaveAddress, I2CHandler);
	eraseScreen(slaveAddress, I2CHandler);
}

void testScreen(uint8_t slaveAddress, I2C_HandleTypeDef* I2CHandler) {
	uint8_t entireScreenOnCommand = 0xA5;
	uint8_t resumeRAMContentsCommand = 0xA4;
	writeSSD1306Commands(&entireScreenOnCommand, 1, slaveAddress, I2CHandler);
	HAL_Delay(300);
	writeSSD1306Commands(&resumeRAMContentsCommand, 1, slaveAddress, I2CHandler);
	HAL_Delay(300);
}

void eraseScreen(uint8_t slaveAddress, I2C_HandleTypeDef* I2CHandler) {
	uint8_t columnsAndPagesSetupCommands[] = {
		0x21,
		0x00, 0x7F,
		0x22,
		0x00, 0x07
	};
	writeSSD1306Commands(columnsAndPagesSetupCommands, 6, slaveAddress, I2CHandler);

	uint8_t blankBitMap[128];
	for(int i = 0; i < 128; i++) {
		blankBitMap[i] = 0x00;
	}

	for(int page = 0; page <= 0x07; page++) {
		writeSSD1306Data(blankBitMap, 128, slaveAddress, I2CHandler);
	}
}

void fillScreen(uint8_t slaveAddress, I2C_HandleTypeDef* I2CHandler) {
	uint8_t columnsAndPagesSetupCommands[] = {
		0x21,
		0x00, 0x7F,
		0x22,
		0x00, 0x07
	};
	writeSSD1306Commands(columnsAndPagesSetupCommands, 6, slaveAddress, I2CHandler);

	uint8_t fullBitMap[128];
	for(int i = 0; i < 128; i++) {
		fullBitMap[i] = 0xFF;
	}

	for(int page = 0; page <= 0x07; page++) {
		writeSSD1306Data(fullBitMap, 128, slaveAddress, I2CHandler);
	}
}

void drawText(char message[], uint8_t messageLengthInCharacters, uint8_t fontSize, uint8_t topLeftXCoordinate, uint8_t topLeftYCoordiante, uint8_t slaveAddress, I2C_HandleTypeDef* I2CHandler) {
	uint8_t characterBitMaps[27][8] = {
		{	// A
			0b00000000,
			0b11111100,
			0b11111110,
			0b00110011,
			0b00110011,
			0b11111110,
			0b11111100,
			0b00000000
		},
		{	// B
			0b00000000,
			0b11111111,
			0b11111111,
			0b10011001,
			0b10011001,
			0b01100110,
			0b01100110,
			0b00000000
		},
		{	// C
			0b00000000,
			0b00111100,
			0b01111110,
			0b11000011,
			0b11000011,
			0b11000011,
			0b01100110,
			0b00000000
		},
		{	// D
			0b00000000,
			0b11111111,
			0b11111111,
			0b11000011,
			0b11000011,
			0b01111110,
			0b00111100,
			0b00000000
		},
		{	// E
			0b00000000,
			0b11111111,
			0b11111111,
			0b10011001,
			0b10011001,
			0b10011001,
			0b00000000,
			0b00000000
		},
		{	// F
			0b00000000,
			0b00000000,
			0b11111111,
			0b11111111,
			0b00110011,
			0b00110011,
			0b00000000,
			0b00000000
		},
		{	// G
			0b00000000,
			0b01111100,
			0b11111110,
			0b11000011,
			0b11010011,
			0b11110011,
			0b01110010,
			0b00000000
		},
		{	// H
			0b00000000,
			0b11111111,
			0b11111111,
			0b00011000,
			0b00011000,
			0b11111111,
			0b11111111,
			0b00000000
		},
		{	// I
			0b00000000,
			0b11000011,
			0b11000011,
			0b11111111,
			0b11111111,
			0b11000011,
			0b11000011,
			0b00000000
		},
		{	// J
			0b00000000,
			0b01100011,
			0b11000011,
			0b11111111,
			0b01111111,
			0b00000011,
			0b00000011,
			0b00000000
		},
		{	// K
			0b00000000,
			0b11111111,
			0b11111111,
			0b00011000,
			0b00111100,
			0b01100110,
			0b11000011,
			0b00000000
		},
		{	// L
			0b00000000,
			0b11111111,
			0b11111111,
			0b11000000,
			0b11000000,
			0b11000000,
			0b00000000,
			0b00000000
		},
		{	// M
			0b00000000,
			0b11111110,
			0b11111110,
			0b00001100,
			0b00011000,
			0b00001100,
			0b11111110,
			0b11111110
		},
		{	// N
			0b00000000,
			0b11111110,
			0b11111110,
			0b00001100,
			0b00011000,
			0b00110000,
			0b11111110,
			0b11111110
		},
		{	// O
			0b00000000,
			0b01111100,
			0b11000110,
			0b10000010,
			0b10000010,
			0b11000110,
			0b01111100,
			0b00000000
		},
		{	// P
			0b00000000,
			0b11111111,
			0b11111111,
			0b00001001,
			0b00001001,
			0b00001111,
			0b00000110,
			0b00000000
		},
		{	// Q
			0b00000000,
			0b01111100,
			0b10000010,
			0b10000010,
			0b10010010,
			0b10100010,
			0b01111100,
			0b10000000
		},
		{	// R
			0b00000000,
			0b11111111,
			0b11111111,
			0b00011001,
			0b00111001,
			0b11100110,
			0b00000000,
			0b00000000
		},
		{	// S
			0b00000000,
			0b10011111,
			0b10011111,
			0b10011001,
			0b10011001,
			0b11111001,
			0b11111001,
			0b00000000
		},
		{	// T
			0b00000000,
			0b00000011,
			0b00000011,
			0b11111111,
			0b11111111,
			0b00000011,
			0b00000011,
			0b00000000
		},
		{	// U
			0b00000000,
			0b00111111,
			0b11111111,
			0b11000000,
			0b11000000,
			0b11111111,
			0b00111111,
			0b00000000
		},
		{	// V
			0b00000000,
			0b00011110,
			0b01111000,
			0b11000000,
			0b11000000,
			0b01111000,
			0b00011110,
			0b00000000
		},
		{	// W
			0b00000000,
			0b01111110,
			0b11100000,
			0b01111000,
			0b01111000,
			0b11100000,
			0b01111110,
			0b00000000
		},
		{	// X
			0b00000000,
			0b11000110,
			0b01101100,
			0b00111000,
			0b00111000,
			0b01101100,
			0b11000110,
			0b00000000
		},
		{	// Y
			0b00000000,
			0b00000111,
			0b00001110,
			0b11111000,
			0b11111000,
			0b00001110,
			0b00000111,
			0b00000000
		},
		{	// Z
			0b00000000,
			0b11000011,
			0b11100011,
			0b11110011,
			0b11011011,
			0b11001111,
			0b11000111,
			0b00000000
		},
		{	// SPACE
			0b00000000,
			0b00000000,
			0b00000000,
			0b00000000,
			0b00000000,
			0b00000000,
			0b00000000,
			0b00000000
		}
	};

	// Given message, messageLengthInCharacters, fontSize, topLeftXCoordinate
	// Find topLeftX2Coordinate // x-coordinate of last character on line.

	uint8_t columnEndCoordinate = 0;
	if(topLeftXCoordinate + (fontSize*messageLengthInCharacters) > 128) { columnEndCoordinate = 127; }
	else { columnEndCoordinate = (fontSize * messageLengthInCharacters) - 1; }

	uint8_t charactersPerPage = (columnEndCoordinate - topLeftXCoordinate + 1) / fontSize;
	uint8_t numberOfPages = (messageLengthInCharacters + charactersPerPage - 1) / charactersPerPage;
	uint8_t pageEndCoordinate = numberOfPages - 1;

	uint8_t columnsAndPagesSetupCommands[] = {
		0x21, 		// Set columns starting coordinate.
		topLeftXCoordinate, columnEndCoordinate,
		0x22,		// Set pages starting coordinate.
		topLeftYCoordiante / 8, pageEndCoordinate		// TODO: Add text's ability to go in between pages.
	};

	writeSSD1306Commands(columnsAndPagesSetupCommands, 6, slaveAddress, I2CHandler);

	for(int page = 0; page < numberOfPages; page++) {
		uint8_t pageData[columnEndCoordinate - topLeftXCoordinate];

		for(int character = 0; character < charactersPerPage; character++) {
			if(character + (charactersPerPage * page) >= messageLengthInCharacters) {
				// Make buffer stop writing without erasing previously existing data.
				// Possible fix is to do this looping algorithm for 0 to n-1 lines and then
				// on last line (line n), write to a different column length with a shorter buffer.
				continue;
			}

			char currentCharacter = message[character + (charactersPerPage * page)];
			for(int column = 0; column < fontSize; column++) {
				if(currentCharacter == ' ') {
					pageData[column + (fontSize * character)] = characterBitMaps[26][column];
					continue;
				}

				pageData[column + (fontSize * (character))] = characterBitMaps[currentCharacter - 65][column];
			}
		}

		writeSSD1306Data(pageData, columnEndCoordinate - topLeftXCoordinate, slaveAddress, I2CHandler);
	}
}
//void drawBitmap(uint8_t* bitMap, uint8_t bitMapLength, uint8_t bitMapHeight, uint8_t topLeftXCoordinate, uint8_t topLeftYCoordinate) {}
//void drawLine(uint8_t point1Coordinates[], uint8_t point2Coordinates[]) {}
//void drawPolygon(uint8_t xCoordinates[], uint8_t yCoordinates[], uint8_t numberOfPoints) {}

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

/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
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
#include "adc.h"
#include "dma.h"
#include "rtc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BIT(n) 						(1 << n)
#define LINE_FLAG_OVERFLOW                BIT(0)
#define LINE_FLAG_COMMENT_PARENTHESES    BIT(1)
#define LINE_FLAG_COMMENT_SEMICOLON    BIT(2)
#define ADC_CONVERTED_DATA_BUFFER_SIZE   ((uint32_t)   3)
#define RX_BUFFER_SIZE   12
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile uint8_t alarm = 0;
static uint16_t aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE];
uint8_t aRXBufferA[RX_BUFFER_SIZE];
uint8_t aRXBufferB[RX_BUFFER_SIZE];
uint8_t aTXbuffer[64];
volatile uint32_t uwNbReceivedChars;
volatile uint32_t uwBufferReadyIndication;
uint8_t *pBufferReadyForUser;
uint8_t *pBufferReadyForReception;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
uint8_t ExecuteLine(char *line);
void Report(uint8_t code);
void PrintInfo(uint8_t *String, uint32_t Size);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */
	//char c;
	//uint8_t char_counter = 0;
	//static char line[12] = { }; // Line to be executed. Zero-terminated.
	//uint8_t line_flags = 0;
	uint32_t index = 0;

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
	MX_ADC_Init();
	MX_RTC_Init();
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */
	HAL_ADCEx_Calibration_Start(&hadc);
	pBufferReadyForReception = aRXBufferA;
	pBufferReadyForUser = aRXBufferB;
	uwNbReceivedChars = 0;
	uwBufferReadyIndication = 0;
	LL_USART_ClearFlag_ORE(USART1);
	LL_USART_EnableIT_RXNE(USART1);
	LL_USART_EnableIT_ERROR(USART1);

	HAL_ADC_Start_DMA(&hadc, (uint32_t*) aADCxConvertedData,
	ADC_CONVERTED_DATA_BUFFER_SIZE);
	LL_GPIO_SetOutputPin(CE_GPIO_Port, CE_Pin);
//	Printf("Humidity 1: %d%%\r\n",
//			100 - ((100 * aADCxConvertedData[0]) / 4096));
//	Printf("Humidity 2: %d%%\r\n",
//			100 - ((100 * aADCxConvertedData[1]) / 4096));
//	Printf("Humidity 3: %d%%\r\n",
//			100 - ((100 * aADCxConvertedData[2]) / 4096));
//	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
//	Printf("Date: %02d-%02d-%02d\r\n", sDate.Year, sDate.Month, sDate.Date);
//	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
//	Printf("Time: %02d:%02d:%02d\r\n", sTime.Hours, sTime.Minutes,
//			sTime.Seconds);
//	HAL_RTC_GetAlarm(&hrtc, &sAlarm, RTC_ALARM_A, RTC_FORMAT_BIN);
//	Printf("Alarm: %02d:%02d:%02d\r\n", sAlarm.AlarmTime.Hours,
//			sAlarm.AlarmTime.Minutes, sAlarm.AlarmTime.Seconds);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
//		while (Getc(&c) == 0) {
//			Putc(c);
//			if ((c == '\n') || (c == '\r')) { // End of line reached
//				Printf("\r\n");
//				line[char_counter] = 0;
//				if (line_flags & LINE_FLAG_OVERFLOW) {
//					Printf(
//							"Max characters per line exceeded. Line was not processed and executed.\r\n");
//				} else if (line[0] == 0) {
//					Printf("ok\r\n");
//				} else {
//					Report(ExecuteLine(line));
//				}
//				char_counter = 0;
//			} else {
//				if (c <= ' ') {
//					continue;
//				} else if (char_counter <= 11) {
//					line_flags |= LINE_FLAG_OVERFLOW;
//				} else if (c >= 'a' && c <= 'z') { // Upcase lowercase
//					line[char_counter++] = c - 'a' + 'A';
//				} else {
//					line[char_counter++] = c;
//				}
//
//			}
//		}
		if (uwBufferReadyIndication != 0) {
			char first_letter = pBufferReadyForUser[0];
			char letter = 0;
			char digit = 0;
			for (index = 0; index < RX_BUFFER_SIZE; index++) {
				if (pBufferReadyForUser[index] != '\0') {
					letter = pBufferReadyForUser[index];
					digit = letter - '0';
					switch (index) {
					case 1:
						switch (first_letter) {
						case 'D':
							if ((digit > 0) && (digit < 8)) {
								sDate.WeekDay = (uint8_t) digit;
							}
							break;
						case 'T':
							if ((digit >= 0) && (digit < 6)) {
								sTime.Hours = 10 * (uint8_t) digit;
							}
							break;
						case 'A':
							if ((digit >= 0) && (digit < 6)) {
								sAlarm.AlarmTime.Hours = 10 * ((uint8_t) digit);
							}
							break;
						default:
							break;
						}
						break;
					case 2:
						switch (first_letter) {
						case 'D':
							if ((digit > 0) && (digit < 3)) {
								sDate.Year = 10 * ((uint8_t) digit);
							}
							break;
						case 'T':
							if ((digit >= 0) && (digit < 10)) {
								sTime.Hours += ((uint8_t) digit);
								if (sTime.Hours > 23) {
									sTime.Hours = 23;
								}
							}
							break;
						case 'A':
							if ((digit >= 0) && (digit < 10)) {
								sAlarm.AlarmTime.Hours += ((uint8_t) digit);
								if (sAlarm.AlarmTime.Hours > 23) {
									sAlarm.AlarmTime.Hours = 23;
								}
							}
							break;
						default:
							break;
						}
						break;
					case 3:
						switch (first_letter) {
						case 'D':
							if ((digit >= 0) && (digit < 10)) {
								sDate.Year += ((uint8_t) digit);
							}
							break;
						case 'T':
							if ((digit >= 0) && (digit < 6)) {
								sTime.Minutes = 10 * ((uint8_t) digit);
							}
							break;
						case 'A':
							if ((digit >= 0) && (digit < 10)) {
								sAlarm.AlarmTime.Minutes = 10
										* ((uint8_t) digit);
							}
							break;
						default:
							break;
						}
						break;
					case 4:
						switch (first_letter) {
						case 'D':
							if ((digit >= 0) && (digit <= 1)) {
								sDate.Month = 10 * ((uint8_t) digit);
							}
							break;
						case 'T':
							if ((digit >= 0) && (digit < 10)) {
								sTime.Minutes += ((uint8_t) digit);
								if (sTime.Minutes > 59) {
									sTime.Minutes = 59;
								}
							}
							break;
						case 'A':
							if ((digit >= 0) && (digit < 10)) {
								sAlarm.AlarmTime.Minutes += ((uint8_t) digit);
								if (sAlarm.AlarmTime.Minutes > 59) {
									sAlarm.AlarmTime.Minutes = 59;
								}
							}
							break;
						default:
							break;
						}
						break;
					case 5:
						switch (first_letter) {
						case 'D':
							if ((digit >= 0) && (digit <= 9)) {
								sDate.Month += ((uint8_t) digit);
								if (sDate.Month > 0x12) {
									sDate.Month = 0x12;
								}
							}
							break;
						case 'T':
							if ((digit >= 0) && (digit < 6)) {
								sTime.Seconds = 10 * ((uint8_t) digit);
							}
							break;
						case 'A':
							if ((digit >= 0) && (digit < 6)) {
								sAlarm.AlarmTime.Seconds = 10
										* ((uint8_t) digit);
							}
							break;
						default:
							break;
						}
						break;
					case 6:
						switch (first_letter) {
						case 'D':
							if ((digit >= 0) && (digit <= 9)) {
								sDate.Date = 10 * ((uint8_t) digit);
							}
							break;
						case 'T':
							if ((digit >= 0) && (digit < 10)) {
								sTime.Seconds += ((uint8_t) digit);
								if (sTime.Seconds > 59) {
									sTime.Seconds = 59;
								}
							}
							break;
						case 'A':
							if ((digit >= 0) && (digit < 6)) {
								sAlarm.AlarmTime.Seconds += ((uint8_t) digit);
								if (sAlarm.AlarmTime.Seconds > 59) {
									sAlarm.AlarmTime.Seconds = 59;
								}
							}
							break;
						default:
							break;
						}
						break;
					case 7:
						switch (first_letter) {
						case 'D':
							if ((digit >= 0) && (digit <= 9)) {
								sDate.Date += ((uint8_t) digit);
								if (sDate.Date < 1) {
									sDate.Date = 1;
								}
								if (sDate.Date > 31) {
									sDate.Date = 31;
								}
							}
							break;
						default:
							break;
						}
						break;
					default:
						break;
					}

				} else
					break;
			}
			switch (first_letter) {
			case 'G':
				sprintf((char*) aTXbuffer, "Humidity 1: %d%%\r\n",
						100 - ((100 * aADCxConvertedData[0]) / 4096));
				PrintInfo(aTXbuffer, strlen((char*) aTXbuffer));
				memset(aTXbuffer, 0, sizeof(aTXbuffer));
				sprintf((char*) aTXbuffer, "Humidity 2: %d%%\r\n",
						100 - ((100 * aADCxConvertedData[1]) / 4096));
				PrintInfo(aTXbuffer, strlen((char*) aTXbuffer));
				memset(aTXbuffer, 0, sizeof(aTXbuffer));
				sprintf((char*) aTXbuffer, "Humidity 3: %d%%\r\n",
						100 - ((100 * aADCxConvertedData[2]) / 4096));
				PrintInfo(aTXbuffer, strlen((char*) aTXbuffer));
				memset(aTXbuffer, 0, sizeof(aTXbuffer));
				HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
				sprintf((char*) aTXbuffer, "Date: %02d-%02d-%02d\r\n",
						sDate.Year, sDate.Month, sDate.Date);
				PrintInfo(aTXbuffer, strlen((char*) aTXbuffer));
				memset(aTXbuffer, 0, sizeof(aTXbuffer));
				HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
				sprintf((char*) aTXbuffer, "Time: %02d:%02d:%02d\r\n",
						sTime.Hours, sTime.Minutes, sTime.Seconds);
				PrintInfo(aTXbuffer, strlen((char*) aTXbuffer));
				memset(aTXbuffer, 0, sizeof(aTXbuffer));
				HAL_RTC_GetAlarm(&hrtc, &sAlarm, RTC_ALARM_A,
				RTC_FORMAT_BIN);
				sprintf((char*) aTXbuffer, "Alarm: %02d:%02d:%02d\r\n",
						sAlarm.AlarmTime.Hours, sAlarm.AlarmTime.Minutes,
						sAlarm.AlarmTime.Seconds);
				PrintInfo(aTXbuffer, strlen((char*) aTXbuffer));
				memset(aTXbuffer, 0, sizeof(aTXbuffer));
				break;
			case 'T':
				HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
				break;
			case 'D':
				HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
				break;
			case 'A':
				HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN);
			default:
				break;
			}
			uwBufferReadyIndication = 0;
		}
		if (alarm == 1) {
			if (aADCxConvertedData[0] > 2500) {
				LL_GPIO_SetOutputPin(WP1_GPIO_Port, WP1_Pin);
				while (aADCxConvertedData[0] > 2500) {
					LL_mDelay(100);
				}
				LL_GPIO_ResetOutputPin(WP1_GPIO_Port, WP1_Pin);
			}
			if (aADCxConvertedData[1] > 2500) {
				LL_GPIO_SetOutputPin(WP2_GPIO_Port, WP2_Pin);
				while (aADCxConvertedData[1] > 2500) {
					LL_mDelay(100);
				}
				LL_GPIO_ResetOutputPin(WP2_GPIO_Port, WP2_Pin);
			}
			if (aADCxConvertedData[2] > 2500) {
				LL_GPIO_SetOutputPin(WP3_GPIO_Port, WP3_Pin);
				while (aADCxConvertedData[2] > 2500) {
					LL_mDelay(100);
				}
				LL_GPIO_ResetOutputPin(WP3_GPIO_Port, WP3_Pin);
			}
			alarm = 0;
		}
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
	while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1) {
	}
	LL_RCC_HSE_Enable();

	/* Wait till HSE is ready */
	while (LL_RCC_HSE_IsReady() != 1) {

	}
	LL_PWR_EnableBkUpAccess();
	if (LL_RCC_GetRTCClockSource() != LL_RCC_RTC_CLKSOURCE_LSE) {
		LL_RCC_ForceBackupDomainReset();
		LL_RCC_ReleaseBackupDomainReset();
	}
	LL_RCC_LSE_SetDriveCapability(LL_RCC_LSEDRIVE_HIGH);
	LL_RCC_LSE_Enable();

	/* Wait till LSE is ready */
	while (LL_RCC_LSE_IsReady() != 1) {

	}
	if (LL_RCC_GetRTCClockSource() != LL_RCC_RTC_CLKSOURCE_LSE) {
		LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);
	}
	LL_RCC_EnableRTC();
	LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_3);
	LL_RCC_PLL_Enable();

	/* Wait till PLL is ready */
	while (LL_RCC_PLL_IsReady() != 1) {

	}
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

	/* Wait till System clock is ready */
	while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) {

	}
	LL_SetSystemCoreClock(48000000);

	/* Update the time base */
	if (HAL_InitTick(TICK_INT_PRIORITY) != HAL_OK) {
		Error_Handler();
	}
	LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK1);
}

/* USER CODE BEGIN 4 */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc) {
	alarm = 1;
}

uint8_t ExecuteLine(char *line) {
	char letter = 0;
	char desc[4] = "";
	letter = line[0];
	if ((letter < 'A') || (letter > 'Z')) {
		return 1; //EXPECTED command letter
	} // [Expected word letter]
	if (letter == 'D') {
		if (strlen(&line[1]) == 7) {
			strncpy(desc, &line[1], 1);
			desc[1] = '\0';
			sDate.WeekDay = (uint8_t) atoi(desc);
			memset(&desc, 0, sizeof(desc));
			strncpy(desc, &line[2], 2);
			desc[2] = '\0';
			sDate.Year = (uint8_t) atoi(desc);
			memset(&desc, 0, sizeof(desc));
			strncpy(desc, &line[4], 2);
			desc[2] = '\0';
			sDate.Month = (uint8_t) atoi(desc);
			memset(&desc, 0, sizeof(desc));
			strncpy(desc, &line[6], 2);
			desc[2] = '\0';
			sDate.Date = (uint8_t) atoi(desc);
			memset(&desc, 0, sizeof(desc));
			HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
		} else
			return 2; // wrong number of digits
	} else if (letter == 'T') {
		if (strlen(&line[1]) == 6) {
			strncpy(desc, &line[1], 2);
			desc[2] = '\0';
			sTime.Hours = (uint8_t) atoi(desc);
			memset(&desc, 0, sizeof(desc));
			strncpy(desc, &line[3], 2);
			desc[2] = '\0';
			sTime.Minutes = (uint8_t) atoi(desc);
			memset(&desc, 0, sizeof(desc));
			strncpy(desc, &line[5], 2);
			desc[2] = '\0';
			sTime.Seconds = (uint8_t) atoi(desc);
			HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		} else
			return 2;
	} else if (letter == 'A') {
		if (strlen(&line[1]) == 6) {
			strncpy(desc, &line[1], 2);
			desc[2] = '\0';
			sAlarm.AlarmTime.Hours = (uint8_t) atoi(desc);
			memset(&desc, 0, sizeof(desc));
			strncpy(desc, &line[3], 2);
			desc[2] = '\0';
			sAlarm.AlarmTime.Minutes = (uint8_t) atoi(desc);
			memset(&desc, 0, sizeof(desc));
			strncpy(desc, &line[5], 2);
			desc[2] = '\0';
			sAlarm.AlarmTime.Seconds = (uint8_t) atoi(desc);
			HAL_RTC_SetAlarm(&hrtc, &sAlarm, RTC_FORMAT_BIN);
		} else
			return 2;
	} else if (letter == 'G') {

//		PrintInfo("Humidity 1: %d%%\r\n",
//				100 - ((100 * aADCxConvertedData[0]) / 4096));
//		PrintInfo("Humidity 2: %d%%\r\n",
//				100 - ((100 * aADCxConvertedData[1]) / 4096));
//		PrintInfo("Humidity 3: %d%%\r\n",
//				100 - ((100 * aADCxConvertedData[2]) / 4096));
//		HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
//		PrintInfo("Date: %02d-%02d-%02d\r\n", sDate.Year, sDate.Month,
//				sDate.Date);
//		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
//		PrintInfo("Time: %02d:%02d:%02d\r\n", sTime.Hours, sTime.Minutes,
//				sTime.Seconds);
//		HAL_RTC_GetAlarm(&hrtc, &sAlarm, RTC_ALARM_A, RTC_FORMAT_BIN);
//		PrintInfo("Alarm: %02d:%02d:%02d\r\n", sAlarm.AlarmTime.Hours,
//				sAlarm.AlarmTime.Minutes, sAlarm.AlarmTime.Seconds);
	} else {
		return 3; // wrong command
	}
	return 0;
}

void Report(uint8_t code) {
	switch (code) {
	case 1:
		//PrintInfo("Command letter expected\r\n");
		break;
	case 2:
		//PrintInfo("Wrong number of digits\r\n");
		break;
	case 3:
		//PrintInfo("Wrong command\r\n");
		break;
	default:
		break;
	}
}

void USART_CharReception_Callback(void) {
	uint8_t *ptemp;
	pBufferReadyForReception[uwNbReceivedChars++] = LL_USART_ReceiveData8(
	USART1);
	if (uwNbReceivedChars >= RX_BUFFER_SIZE) {
		uwBufferReadyIndication = 1;
		ptemp = pBufferReadyForUser;
		pBufferReadyForUser = pBufferReadyForReception;
		pBufferReadyForReception = ptemp;
		uwNbReceivedChars = 0;
	}
}

/**
 * @brief  Send Txt information message on USART Tx line (to PC Com port).
 * @param  None
 * @retval None
 */
void PrintInfo(uint8_t *String, uint32_t Size) {
	uint32_t index = 0;
	uint8_t *pchar = String;

	/* Send characters one per one, until last char to be sent */
	for (index = 0; index < Size; index++) {
		/* Wait for TXE flag to be raised */
		while (!LL_USART_IsActiveFlag_TXE(USART1)) {
		}

		/* Write character in Transmit Data register.
		 TXE flag is cleared by writing data in TDR register */
		LL_USART_TransmitData8(USART1, *pchar++);
	}

	/* Wait for TC flag to be raised for last char */
	while (!LL_USART_IsActiveFlag_TC(USART1)) {
	}
}

void Error_Callback(void) {
	__IO uint32_t isr_reg;

	/* Disable USARTx_IRQn */
	NVIC_DisableIRQ(USART1_IRQn);

	/* Error handling example :
	 - Read USART ISR register to identify flag that leads to IT raising
	 - Perform corresponding error handling treatment according to flag
	 */
	isr_reg = LL_USART_ReadReg(USART1, ISR);
	if (isr_reg & LL_USART_ISR_NE) {
		/* case Noise Error flag is raised : Clear NF Flag */
		LL_USART_ClearFlag_NE(USART1);
	} else {
		/* Unexpected IT source : Set LED to Blinking mode to indicate error occurs */
		while (1) {
			LL_GPIO_TogglePin(STATE_GPIO_Port, STATE_Pin);
			LL_mDelay(1000);
		}
	}
}

/* USER CODE END 4 */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM1 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM1) {
		HAL_IncTick();
	}
	/* USER CODE BEGIN Callback 1 */

	/* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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

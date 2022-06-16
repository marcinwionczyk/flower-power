/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Checkout
 * C:\Users\marci\repos\STM32Cube_FW_F0_V1.11.0\Projects\STM32F072RB-Nucleo\Examples_LL\RTC\RTC_Alarm\
  * C:\Users\marci\repos\STM32Cube_FW_F0_V1.11.0\Projects\STM32F072RB-Nucleo\Examples_LL\USART\USART_Communication_TxRx_DMA
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
#include <string.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

void WaitAndCheckEndOfTransfer(void);
void ProcessInputMessage(void);
void ProcessRTCAlarm(void);
void GetRTCValues(void);
void ConfigureRTCAlarm(DateTimeAlarm_st dateTimeAlarm);
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ADC_DELAY_CALIB_ENABLE_CPU_CYCLES (LL_ADC_DELAY_CALIB_ENABLE_ADC_CYCLES * 32)
#define ADC_CONVERTED_DATA_BUFFER_SIZE   ((uint32_t)   3)
#define RX_TX_BUFFER_SIZE 64
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

volatile uint8_t ubTransmissionComplete = 0;
volatile uint8_t uartReceptionComplete = 0;
/* Variable containing ADC conversions data */
static uint16_t aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE];
char aTxBuffer[RX_TX_BUFFER_SIZE] = { };
char aRxBuffer[RX_TX_BUFFER_SIZE] = { };
RTC_TimeTypeDef time = { };
RTC_TimeTypeDef alarm = { };
RTC_DateTypeDef date = { };

/* Variable to report status of ADC group regular sequence conversions:       */
/*  0: ADC group regular sequence conversions are not completed               */
/*  1: ADC group regular sequence conversions are completed                   */
volatile uint8_t adcGrpRegularSequenceConvStatus = 0; /* Variable set into ADC interruption callback */
/* Variable to report number of ADC group regular sequence completed  */
volatile uint8_t alarm = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
  MX_ADC_Init();
  MX_RTC_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	//LL_ADC_REG_StartConversion(ADC1);
	//LL_GPIO_SetOutputPin(CE_GPIO_Port, CE_Pin);
	//LL_USART_EnableDMAReq_RX(USART1);
	//LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_3);
	//sprintf(aTxBuffer, "humidity 1: %d%%\r\n",
    //		(100 * aADCxConvertedData[0]) >> 12);
	//WaitAndCheckEndOfTransfer();
	//sprintf(aTxBuffer, "humidity 2: %d%%\r\n",
	//		(100 * aADCxConvertedData[1]) >> 12);
	//WaitAndCheckEndOfTransfer();
	//sprintf(aTxBuffer, "humidity 3: %d%%\r\n",
	//		(100 * aADCxConvertedData[2]) >> 12);
	//WaitAndCheckEndOfTransfer();
	//ShowRtcDate();
	//ShowRtcTime();
	//ShowAlarm();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
		if (uartReceptionComplete == 1) {
			ProcessInputMessage();
			uartReceptionComplete = 0;
		}
		if (alarm == 1) {
			ProcessRTCAlarm();
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
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1)
  {
  }
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_PWR_EnableBkUpAccess();
  if(LL_RCC_GetRTCClockSource() != LL_RCC_RTC_CLKSOURCE_LSE)
  {
    LL_RCC_ForceBackupDomainReset();
    LL_RCC_ReleaseBackupDomainReset();
  }
  LL_RCC_LSE_SetDriveCapability(LL_RCC_LSEDRIVE_HIGH);
  LL_RCC_LSE_Enable();

   /* Wait till LSE is ready */
  while(LL_RCC_LSE_IsReady() != 1)
  {

  }
  if(LL_RCC_GetRTCClockSource() != LL_RCC_RTC_CLKSOURCE_LSE)
  {
    LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);
  }
  LL_RCC_EnableRTC();
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_3);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_SetSystemCoreClock(48000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
  LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK1);
}

/* USER CODE BEGIN 4 */
void AdcDmaTransferError_Callback() {
	Error_Handler();
}

void AdcGrpRegularSequenceConvComplete_Callback() {
	/* Update status variable of ADC group regular sequence */
	adcGrpRegularSequenceConvStatus = 1;
}


void Alarm_Callback() {
	alarm = 1;
}

inline void ProcessInputMessage(void) {
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_3);
	uartReceptionComplete = 0;
	uint8_t index = 0;
	int digit = 0;
	char letter;
	char first_letter = aRxBuffer[0];
	while ((letter != '0') || (index < RX_TX_BUFFER_SIZE)) {
		switch (index) {
		case 0:
			switch (first_letter) {
			case 'D':
			case 'A':
			case 'T':
				break;
			case 'G':
				sprintf(aTxBuffer, "humidity 1: %d%%\r\n",
						(100 * aADCxConvertedData[0]) >> 12);
				WaitAndCheckEndOfTransfer();
				sprintf(aTxBuffer, "humidity 2: %d%%\r\n",
						(100 * aADCxConvertedData[1]) >> 12);
				WaitAndCheckEndOfTransfer();
				sprintf(aTxBuffer, "humidity 3: %d%%\r\n",
						(100 * aADCxConvertedData[2]) >> 12);
				WaitAndCheckEndOfTransfer();
				//ShowRtcDate();
				//ShowRtcTime();
				//ShowAlarm();
				break;
			default:
				sprintf(aTxBuffer, "1st letter should be D, A, T or G\r\n");
				WaitAndCheckEndOfTransfer();
				break;
			}
			break;
		case 1:
			switch (first_letter) {
			case 'D':
				if ((digit > 0) && (digit < 8)) {
					date.WeekDay = digit;
				}
				break;
			case 'A':
				if ((digit > 0) && (digit < 3)) {
					alarm.Hours = 10 * digit;
				}
				break;
			case 'T':
				if ((digit > 0) && (digit < 3)) {

					dateTimeAlarmSt.hour = 10 * digit;
				}
				break;
			default:
				break;
			}
			break;
		case 2:
			switch (first_letter) {
			case 'D':
				if ((digit > 0) && (digit <= 9)) {
					dateTimeAlarmSt.year = 10 * digit;
				}
				break;
			case 'A':
				if ((digit >= 0) && (digit <= 9)) {
					dateTimeAlarmSt.alarm_h += digit;
				}
				break;
			case 'T':
				if ((digit >= 0) && (digit <= 9)) {
					dateTimeAlarmSt.hour += digit;
				}
				break;
			default:
				break;
			}
			break;
		case 3:
			switch (first_letter) {
			case 'D':
				if ((digit >= 0) && (digit <= 9)) {
					dateTimeAlarmSt.year += digit;
				}
				break;
			case 'A':
				if ((digit >= 0) && (digit <= 5)) {
					dateTimeAlarmSt.alarm_m = 10 * digit;
				}
				break;
			case 'T':
				if ((digit >= 0) && (digit < 6)) {
					dateTimeAlarmSt.minute = 10 * digit;
				}
				break;
			default:
				break;
			}
			break;
		case 4:
			switch (first_letter) {
			case 'D':
				if ((digit >= 0) && (digit < 2)) {
					dateTimeAlarmSt.month = 10 * digit;
				}
				break;
			case 'A':
				if ((digit >= 0) && (digit < 10)) {
					dateTimeAlarmSt.alarm_m += digit;
				}
				break;
			case 'T':
				if ((digit >= 0) && (digit < 10)) {
					dateTimeAlarmSt.minute += digit;
				}
				break;
			default:
				break;
			}
			break;
		case 5:
			switch (first_letter) {
			case 'D':
				if ((digit >= 0) && (digit < 10)) {
					dateTimeAlarmSt.month += digit;
				}
				break;
			case 'A':
				if ((digit >= 0) && (digit < 6)) {
					dateTimeAlarmSt.alarm_s = 10 * digit;
				}
				break;
			case 'T':
				if ((digit >= 0) && (digit < 6)) {
					dateTimeAlarmSt.second = 10 * digit;
				}
				break;
			default:
				break;
			}
			break;
		case 6:
			switch (first_letter) {
			case 'D':
				if ((digit >= 0) && (digit <= 3)) {
					dateTimeAlarmSt.day = 10 * digit;
				}
				break;
			case 'A':
				if ((digit >= 0) && (digit <= 9)) {
					dateTimeAlarmSt.alarm_s += digit;
				}
				break;
			case 'T':
				if ((digit >= 0) && (digit <= 9)) {
					dateTimeAlarmSt.second += digit;
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
					dateTimeAlarmSt.day += digit;
				}
				break;
			case 'A':
			case 'T':
				if (letter == '\r') {
					dateTimeAlarmSt.year = __LL_RTC_CONVERT_BCD2BIN(
							LL_RTC_DATE_GetYear(RTC));
					dateTimeAlarmSt.month = __LL_RTC_CONVERT_BCD2BIN(
							LL_RTC_DATE_GetMonth(RTC));
					dateTimeAlarmSt.day = __LL_RTC_CONVERT_BCD2BIN(
							LL_RTC_DATE_GetDay(RTC));
					// ConfigureRTCAlarm(dateTimeAlarmSt);
				}
				break;
			default:
				break;
			}
			break;
		case 8:
			if ((first_letter == 'D') && (letter == '\r')) {
				ConfigureRTCAlarm(dateTimeAlarmSt);
			}
			break;
		default:
			break;
		}
		index++;
		letter = aRxBuffer[index];
		digit = letter - '0';
	}
	memset(aRxBuffer, 0, RX_TX_BUFFER_SIZE);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_3);
}

inline void ProcessRTCAlarm(void) {
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
}




/**
 * @brief  Function called in case of error detected in USART IT Handler
 * @param  None
 * @retval None
 */
void USART_TransferError_Callback(void) {
	/* Disable DMA1 Tx Channel */
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);

	/* Disable DMA1 Rx Channel */
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_3);
	Error_Handler();
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
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
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
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	//__disable_irq();
	while (1) {
		LL_mDelay(500);
		LL_GPIO_TogglePin(STATE_GPIO_Port, STATE_Pin);
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

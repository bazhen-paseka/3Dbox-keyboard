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
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

	#include "VRbox-keyboards-sm.h"
	#include "keyb_4x4_pcf8574_i2c_sm.h"

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
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

	PCF8574_Struct  	hpcf[ VR_KeyBoard_QNT ] ;

	PCF8574_struct_init( &hpcf[ VR_KeyBoard_0 ] , &hi2c1 ,  VR_KeyBoard_0 ,  I2C_ADR_PCF_0 ) ;
	PCF8574_struct_init( &hpcf[ VR_KeyBoard_1 ] , &hi2c1 ,  VR_KeyBoard_1 ,  I2C_ADR_PCF_1 ) ;
	PCF8574_IRQ_enable ( &hpcf[ VR_KeyBoard_0 ] ) ;
	PCF8574_IRQ_enable ( &hpcf[ VR_KeyBoard_1 ] ) ;

	VRbox_Struct 	hvrbox ;
	VRbox_Init( &hvrbox , &huart1 , &huart3 , &hi2c1 ) ;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	VRbox_Main ( &hvrbox , &hpcf[ VR_KeyBoard_0 ] , &hpcf[ VR_KeyBoard_1 ] ) ;

//	//---------------------------------------------------------------
//	if ( PCF8574_get_IRQ_flag( &hpcf[0] ) == SET ) 			{
//		hpcf[0].key_current = PCF8574_scan_keyboard( &hpcf[0] ) 	;
//		if ( hpcf[0].key_previous != hpcf[0].key_current )		{
//			VRbox_Send( &hvrbox , &hpcf[0] )				;
//			hpcf[0].key_previous = hpcf[0].key_current	;
//		}
//		PCF8574_update_IRQ_flag( &hpcf[0] , RESET )		;
//		PCF8574_IRQ_enable( &hpcf[0] )						;
//	}
//	//---------------------------------------------------------------
//	if ( PCF8574_get_IRQ_flag( &hpcf[1] ) == SET ) 			{
//		hpcf[1].key_current = PCF8574_scan_keyboard( &hpcf[1] )	;
//		if ( hpcf[1].key_previous != hpcf[1].key_current )		{
//			VRbox_Send( &hvrbox , &hpcf[1] )				;
//			hpcf[1].key_previous = hpcf[1].key_current	;
//		}
//		PCF8574_update_IRQ_flag( &hpcf[1] , SET )			;
//		PCF8574_IRQ_enable( &hpcf[1] )						;
//	}
//	//---------------------------------------------------------------

	for ( VR_Keyboard_enum keyb = 0; keyb < VR_KeyBoard_QNT; keyb++ ) {
		if ( PCF8574_get_IRQ_flag( &hpcf[keyb] ) == SET ) {
		hpcf[keyb].key_current = PCF8574_scan_keyboard( &hpcf[keyb] ) ;
		if ( hpcf[keyb].key_previous != hpcf[keyb].key_current ) {
			VRbox_Send( &hvrbox , &hpcf[keyb] )					;
			hpcf[keyb].key_previous = hpcf[keyb].key_current	;
		}
		PCF8574_update_IRQ_flag( &hpcf[keyb] , RESET )	;
		PCF8574_IRQ_enable( &hpcf[keyb] )				;
		}
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

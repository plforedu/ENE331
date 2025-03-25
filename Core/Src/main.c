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
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */
int 	Count_Timer =0;

int Count_State=0;





void GPIOA_Config(void) {
    // Enable GPIOA clock
    RCC -> AHB1ENR |= (0x01 << 0);
    // Config 		PA1 			as input
    // Config (PA1 to PA3) and PA6  as output

    GPIOA -> MODER |= (0x00 << (0* 2));
    GPIOA -> MODER |= (0x01 << (1* 2)); //LED 1
    GPIOA -> MODER |= (0x01 << (2* 2)); //LED 2
    GPIOA -> MODER |= (0x01 << (3* 2)); //LED 3
    GPIOA -> MODER |= (0x01 << (6* 2)); //Blink

    // Set output type
    // ALL GPIO (PB) is  Output push-pull
    GPIOA-> OTYPER &= (0x00);


    // ALL GPIO (PA) Speed is High speed Output
    GPIOA -> OSPEEDR &= ~(0x00);

    // Enable PA0  pull-up
    GPIOA->PUPDR |= (0x01 << (0 * 2));

    // Disable (PA1 to PA3) and PA6  pull-up
    GPIOA->PUPDR |= (0x00 << (1 * 2));
    GPIOA->PUPDR |= (0x00 << (2 * 2));
    GPIOA->PUPDR |= (0x00 << (3 * 2));
    GPIOA->PUPDR |= (0x00 << (6 * 2));
}


void GPIOB_Config(void) {
	// Enable GPIOC Clock
	RCC -> AHB1ENR |= (0x01 << 1);

	// Config PB10 as Output
	GPIOB -> MODER |= (0x01 << (10* 2));

    // Set output type
    // ALL GPIO (PB) is  Output push-pull
	GPIOB-> OTYPER &= (0x00);

	// ALL GPIO (PBA) Speed is High speed Output
	GPIOB -> OSPEEDR &= ~(0x00);

	// Disable PA10  pull-up
	GPIOB->PUPDR |= (0x00 << (10 * 2));
}

void GPIOC_Config(void)
{
    // step 1 : enable GPIOB clock
    RCC -> AHB1ENR |= (0x01 << 2);
    // step 2 : Config PB1 as input
    GPIOC -> MODER &= ~(0x03 << (13 * 2));
    GPIOC->PUPDR  |= (0x01 << (13 * 2));

}

unsigned char Read_PC13(void)
{
    return (GPIOC -> IDR & (0x01 << 13)) ? 1 : 0;
}


unsigned char Read_PA0(void)
{
    return (GPIOA -> IDR & (0x01 << 0)) ? 1 : 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if(htim -> Instance == TIM2)
	{
		Count_Timer++;
		GPIOB -> ODR ^= (1 << 10);
	}
}































/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  GPIOA_Config();
    GPIOB_Config();
    GPIOC_Config();
    HAL_TIM_Base_Start_IT(&htim2);

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */


    uint8_t led_state = 0;
    uint8_t last_button_state = 0;


        while(1)
        {
      	  uint8_t button_state = Read_PA0();

            if(button_state  == 0 && last_button_state == 1 )
            {
          	  Count_State ++;
          	   if (Count_State >=4)
  			  {
          		  Count_State = 0;
  			  }
            }

            if (Count_State == 0)
            			  {
            				  GPIOA -> ODR |= (0x01 << 1);
            				  GPIOA -> ODR &= ~(0x01 << 2);
            				  GPIOA -> ODR &= ~(0x01 << 3);
            			  }
            			  else if (Count_State == 1)
            			  {
            				  GPIOA -> ODR &= ~(0x01 << 1);
            				  GPIOA -> ODR |= (0x01 << 2);
            				  GPIOA -> ODR &= ~(0x01 << 3);
            			  }
            			  else if (Count_State == 2)
            			  {
            				  GPIOA -> ODR &= ~(0x01 << 1);
            				  GPIOA -> ODR &= ~(0x01 << 2);
            				  GPIOA -> ODR |= (0x01 << 3);
            			  }
            			  else if (Count_State == 3)
            			  {
            				  GPIOA -> ODR |= (0x01 << 1);
  						  GPIOA -> ODR |= (0x01 << 2);
  						  GPIOA -> ODR |= (0x01 << 3);
            			  }


            if (Count_Timer >= 60)
  			{
  			GPIOA -> ODR ^= (1 << 6);
  			 Count_Timer = 0;
  			}



            last_button_state = button_state;
    /* USER CODE END 3 */
        }
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  htim2.Init.Period = 4294967295;
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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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

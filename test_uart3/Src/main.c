#include "stm32f1xx_hal.h"
#include <stdio.h>

UART_HandleTypeDef huart2;

int i; //Khai bao bien dem
char buffer[100]; //Khai bao mang buffer 100 phan tu
int len; //Khai bao bien len tuong ung voi chieu dai
char Rx_indx, Rx_data[2], Rx_Buffer[100], Transfer_cplt; //Khai bao cac bien de nhan du lieu
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
  #define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */
	PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
	HAL_UART_Transmit(&huart2, (uint8_t*)&ch,1,100);

  /* Loop until the end of transmission */


  return ch;
}
//Ham ngat uart
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint8_t i;
	if (huart->Instance == USART3)	//UART hien tai la uart3
		{
		if (Rx_indx==0) {for (i=0;i<100;i++) Rx_Buffer[i]=0;}	//xoa Rx_Buffer truoc khi nhan du lieu	
		
		if (Rx_data[0]!=13)	//Neu du lieu nhan duoc khac voi 13 (13 la ma cua phim enter)
			{
			Rx_Buffer[Rx_indx++]=Rx_data[0];	//them du lieu vao Rx_Buffer
			}
		else			//Neu phim enter duoc an
			{
			Rx_indx=0;	//Dua index ve 0
			Transfer_cplt=1;//Qua trinh truyen hoan tat, du lieu da san sang de doc
			}

		HAL_UART_Receive_IT(&huart2, Rx_data, 1);	//Kich hoat UART nhan du lieu ngat moi luc
		}

}
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */
	HAL_UART_Receive_IT(&huart2, Rx_data, 1); //Kich hoat ngat uart khi nhan duoc 1 byte
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
	printf("Hello\r \n");		// In dong chu Hello len tren may tinh
	printf("Gia tri cua i: %d \r\n",i);
		HAL_Delay(500);
		i++;
	// Gio khong su dung printf nua ma thay vao do su dung sprinf
	sprintf(buffer, "Xin chao\r\n");
	len = strlen(buffer);
	HAL_UART_Transmit(&huart2, buffer, len, 1000);	//Truyen du lieu tren uart3, du lieu o day la buffer co chieu dai la len 
	HAL_Delay(500);		
//	if (Transfer_cplt)
//	{
//		sprintf(buffer, "%s\r\n", Rx_Buffer);	// IN bufer
//		len = strlen(buffer);	//Lay chieu dai cua buffer
//		HAL_UART_Transmit(&huart2, buffer,len, 1000);	//Truyen buffer voi chieu dai qua uart3
//		Transfer_cplt =0; //Reset lai bien tranfer_complete
//		HAL_Delay(500);	
//	}
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USART3 init function */
void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart2);

}

/** Pinout Configuration
*/
void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
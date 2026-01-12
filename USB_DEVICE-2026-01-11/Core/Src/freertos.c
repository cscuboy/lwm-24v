/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

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
/* USER CODE BEGIN Variables */
QueueHandle_t xADCQueueHandle = NULL;
QueueHandle_t xADC_Stop_QueueHandle = NULL;
QueueHandle_t xADC_Finish_QueueHandle = NULL;
QueueHandle_t xCAL_Finish_QueueHandle = NULL;
QueueHandle_t xLC_ADC_Finish_QueueHandle = NULL; //单次励磁的ADC采样结束

/* 信号量句柄 */
QueueHandle_t xDebugSemaphore = NULL;
QueueHandle_t xRs485Semaphore = NULL;
QueueHandle_t xAdcFinishForFlashSemaphore = NULL;



/* USER CODE END Variables */
osThreadId SecondTaskHandle;
osThreadId LC_TaskHandle;
osThreadId Modbus_TaskHandle;
osThreadId BT_TaskHandle;
osThreadId LCD_TaskHandle;
osThreadId CAL_TaskHandle;
osThreadId Debug_TaskHandle;
osThreadId Adc_TaskHandle;
osSemaphoreId xADCStartSemaphoreHandle;
osSemaphoreId xADCStopSemaphoreHandle;
osSemaphoreId xDelayUsSemaphoreHandle;
// 1. 互斥锁句柄：核心！用于LC和WF任务的互斥访问，防止同时执行
osSemaphoreId xTask_LC_Mutex = NULL;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
extern void LC_Task_Proc(void const * argument);
extern void Modbus_Task_Proc(void const * argument);
extern void BT_Task_Proc(void const * argument);
extern void LCD_Task_Proc(void const * argument);
extern void CAL_Task_Proc(void const * argument);
extern void Debug_Task_Proc(void const * argument);
extern void Adc_Task_Proc(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of xADCStartSemaphore */
  osSemaphoreDef(xADCStartSemaphore);
  xADCStartSemaphoreHandle = osSemaphoreCreate(osSemaphore(xADCStartSemaphore), 1);

  /* definition and creation of xADCStopSemaphore */
  osSemaphoreDef(xADCStopSemaphore);
  xADCStopSemaphoreHandle = osSemaphoreCreate(osSemaphore(xADCStopSemaphore), 1);

  /* definition and creation of xDelayUsSemaphore */
  osSemaphoreDef(xDelayUsSemaphore);
  xDelayUsSemaphoreHandle = osSemaphoreCreate(osSemaphore(xDelayUsSemaphore), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
      /* 1. ������ֵ�ź�������ʼֵ0�� */
    xADCQueueHandle =  xQueueCreate(1, sizeof(uint8_t)); // ��ֵ�ź�����ʼֵΪ0
    
    // �򴴽��������ź�������Ч����xSemaphoreCreateCounting(1, 0);
    if (xADCQueueHandle == NULL) {
        // ����ʧ�ܴ���
        Error_Handler();
    }
    //
    xADC_Stop_QueueHandle = xSemaphoreCreateBinary();
    if (xADC_Stop_QueueHandle == NULL) {
        // ����ʧ�ܴ���
        Error_Handler();
    }
    
    //AD�ɼ�����ź��� 
    xADC_Finish_QueueHandle= xSemaphoreCreateBinary();
    if (xADC_Finish_QueueHandle == NULL) {
        // ����ʧ�ܴ���
        Error_Handler();
    }
    
    //cal��������ź��� 
    xCAL_Finish_QueueHandle= xSemaphoreCreateBinary();
    if (xCAL_Finish_QueueHandle == NULL) {
        // ����ʧ�ܴ���
        Error_Handler();
    }    
    
    xLC_ADC_Finish_QueueHandle= xSemaphoreCreateBinary();
    if (xLC_ADC_Finish_QueueHandle == NULL) {
        // ����ʧ�ܴ���
        Error_Handler();
    } 
    
    /* 创建二值信号量 */
    xDebugSemaphore = xSemaphoreCreateBinary();
    if (xDebugSemaphore == NULL) 
    {
      // 信号量创建失败，错误处理（如点亮LED报警）
      Error_Handler();
    }   
    xRs485Semaphore = xSemaphoreCreateBinary();
    if (xRs485Semaphore == NULL) 
    {
      // 信号量创建失败，错误处理（如点亮LED报警）
      Error_Handler();
    }    
      
    xAdcFinishForFlashSemaphore = xSemaphoreCreateBinary();
    if (xAdcFinishForFlashSemaphore == NULL) 
    {
      // 信号量创建失败，错误处理（如点亮LED报警）
      Error_Handler();
    }   
      
    xTask_LC_Mutex = xSemaphoreCreateMutex();
    if (xTask_LC_Mutex == NULL) 
    {
      // 信号量创建失败，错误处理（如点亮LED报警）
      Error_Handler();
    }

  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of SecondTask */
  osThreadDef(SecondTask, StartDefaultTask, osPriorityNormal, 0, 512);
  SecondTaskHandle = osThreadCreate(osThread(SecondTask), NULL);

  /* definition and creation of LC_Task */
  osThreadDef(LC_Task, LC_Task_Proc, osPriorityHigh, 0, 512);
  LC_TaskHandle = osThreadCreate(osThread(LC_Task), NULL);

  /* definition and creation of Modbus_Task */
  osThreadDef(Modbus_Task, Modbus_Task_Proc, osPriorityIdle, 0, 512);
  Modbus_TaskHandle = osThreadCreate(osThread(Modbus_Task), NULL);

  /* definition and creation of BT_Task */
  osThreadDef(BT_Task, BT_Task_Proc, osPriorityIdle, 0, 512);
  BT_TaskHandle = osThreadCreate(osThread(BT_Task), NULL);

  /* definition and creation of LCD_Task */
  osThreadDef(LCD_Task, LCD_Task_Proc, osPriorityIdle, 0, 512);
  LCD_TaskHandle = osThreadCreate(osThread(LCD_Task), NULL);

  /* definition and creation of CAL_Task */
  osThreadDef(CAL_Task, CAL_Task_Proc, osPriorityIdle, 0, 512);
  CAL_TaskHandle = osThreadCreate(osThread(CAL_Task), NULL);

  /* definition and creation of Debug_Task */
  osThreadDef(Debug_Task, Debug_Task_Proc, osPriorityIdle, 0, 512);
  Debug_TaskHandle = osThreadCreate(osThread(Debug_Task), NULL);

  /* definition and creation of Adc_Task */
  osThreadDef(Adc_Task, Adc_Task_Proc, osPriorityIdle, 0, 512);
  Adc_TaskHandle = osThreadCreate(osThread(Adc_Task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
   // dprintf("hello\r");
    LED_RUN_Toggle();
    osDelay(100);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

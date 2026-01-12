

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* 全局变量 */
extern TIM_HandleTypeDef htim7;                  // TIM7句柄
//SemaphoreHandle_t xTimer7Semaphore = NULL;// TIM7中断同步信号量
extern osSemaphoreId xDelayUsSemaphoreHandle;

//StaticSemaphore_t xTimer7SemaphoreBuffer; // 静态信号量缓冲区（可选，避免动态内存分配）

/* 函数声明 */
void TIM7_Init(void);                     // TIM7初始化
void Task_DelayExample(void *pvParameters);// 延时示例任务
void Error_Handler(void);                 // 错误处理（需自行实现）

/**
 * @brief  TIM7初始化（1us计数周期，100us更新中断）
 * @retval 无
 */
/*
void TIM7_Init(void)
{
    htim7.Instance = TIM7;
    // 预分频：80MHz / 80 = 1MHz → 1us/计数
    htim7.Init.Prescaler = 80 - 1;
    // 计数模式：向上计数
    htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
    // 自动重装值：99 → 计数0~99 = 100us
    htim7.Init.Period = 99;
    // 时钟分频：不分频
    htim7.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    // 禁止自动重装预加载
    htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
    {
        Error_Handler();
    }
}*/

/**
 * @brief  TIM7底层硬件初始化（时钟+中断配置）
 * @param  htim_base: TIM7句柄
 * @retval 无
 */
/*
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
    if (htim_base->Instance == TIM7)
    {
         1. 使能TIM7时钟 
        __HAL_RCC_TIM7_CLK_ENABLE();

         2. 配置TIM7中断优先级（关键！）
           - Cortex-M4用4位抢占优先级（0~15），数值越小优先级越高
           - 必须 ≤ configMAX_SYSCALL_INTERRUPT_PRIORITY（FreeRTOSConfig.h中定义）
           - 示例：设为5（需确保FreeRTOSConfig.h中该值≥5）
        
        HAL_NVIC_SetPriority(TIM7_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(TIM7_IRQn);
    }
}*/

/**
 * @brief  TIM7底层硬件反初始化
 * @param  htim_base: TIM7句柄
 * @retval 无
 */
/*
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
    if (htim_base->Instance == TIM7)
    {
        __HAL_RCC_TIM7_CLK_DISABLE();
        HAL_NVIC_DisableIRQ(TIM7_IRQn);
    }
}*/

//系统延时us，注意最大定时65ms
void Os_Delay_us(int16_t us)
{
    int16_t ms = us /1000 + 1;
    htim7.Init.Period = us;
    /* 3. 启动TIM7 100us计时（重置计数器+使能中断） */
    __HAL_TIM_SET_COUNTER(&htim7, 0);          // 重置计数器为0
    HAL_TIM_Base_Start_IT(&htim7);             // 启动TIM7并使能更新中断  
    
     /* 4. 等待信号量（非忙等，释放CPU给其他任务）
       - 超时时间设为定时时间+1ms，防止定时器异常导致任务卡死
       - pdTRUE：成功等待到100us中断；pdFALSE：超时
    */
    if (xSemaphoreTake(xDelayUsSemaphoreHandle, pdMS_TO_TICKS(ms)) == pdTRUE)
    {
        HAL_TIM_Base_Stop_IT(&htim7); // 计时完成，停止TIM7中断
    }
    else
    {
        // 延时超时处理（可选：打印日志/标记错误）
        // Error_Handler();
    }
}

void Timer7_Delay_Isr(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE; // 标记是否需要任务调度

    /* 1. 清除更新中断标志位（必须手动清，否则会重复触发） */
    if (TIM7->SR & TIM_SR_UIF)
    {
        TIM7->SR &= ~TIM_SR_UIF;
    }

    /* 2. 中断中释放信号量（必须用FromISR版本！） */
    if (xDelayUsSemaphoreHandle != NULL)
    {
        xSemaphoreGiveFromISR(xDelayUsSemaphoreHandle, &xHigherPriorityTaskWoken);
    }

    /* 3. 触发任务调度（若有更高优先级任务就绪） */
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

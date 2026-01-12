

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "string.h"
#include "timer2_pwm.h"
#include "Task_Adc.h"

/*ADC任务*/
void Adc_Task_Proc(void const * argument);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);

static void ADC_DMA_Init(void);
static void ThreadAdcDmaDoneProc();
static void Adc_Start(uint8_t index);

extern int Lc_Adc_Get_State();

//extern osSemaphoreId xADCStartSemaphoreHandle;
//extern osSemaphoreId xADCStopSemaphoreHandle;
extern QueueHandle_t xADCQueueHandle ;
extern QueueHandle_t xADC_Stop_QueueHandle ;
extern QueueHandle_t xADC_Finish_QueueHandle ;
extern QueueHandle_t xADC_Finish_QueueHandle ;
extern QueueHandle_t xLC_ADC_Finish_QueueHandle ; //单次励磁的ADC采样结束

static uint16_t adcValue[ADC_SIZE_PER_CHANEL*ADC_BUFFER_SIZE];
//dma的数据缓存
static uint16_t dma_adcValue[ADC_DMA_DONE_CYCLE*ADC_BUFFER_SIZE];


//ADC 采样数据结构
stAdcCfg  AdcRunCfg;
//QueueHandle_t  Semaphore_AdcStart;

void stopadc()
{
          HAL_ADC_Stop_DMA(&hadc1);
        HAL_ADC_Stop(&hadc1);
}

void Adc_Task_Proc(void const * argument)
{
    BaseType_t xRecvResult = pdFALSE; // 队列接收返回值
    uint8_t recv_data = 0;
    

    
  ADC_DMA_Init();
  //memset(g_adc_buffer,0x00,sizeof(g_adc_buffer));
  for(;;)
  {
        // 无限期等待启动信号量
       // xResult = xSemaphoreTake(Semaphore_AdcStart, portMAX_DELAY);
        xRecvResult = xQueueReceive(xADCQueueHandle, &recv_data, portMAX_DELAY);
        if (xRecvResult == pdTRUE) 
        {
            
           /* if(0 == recv_data)
            {
                memset(adcValue,0x00,sizeof(adcValue));
            }*/
          
            // 初始化并启动ADC，并通过DMA传输数据
            Adc_Start(recv_data);

            //BaseType_t xResult = xSemaphoreTake(xADC_Stop_QueueHandle, portMAX_DELAY);
            //HAL_ADC_Stop_DMA(&hadc1);
        }
  }
}

//初始化ADC采样数据
void Adc_Start(uint8_t index)
{
    HAL_StatusTypeDef adcret = HAL_OK;
    
    if(index >= ADC_BUFFER_SIZE)
    {
        dprintf("ADC 通道超限!!\r\n");
        Error_Handler();
    }
    
        
    AdcRunCfg.AdcCnt         = 0;
    AdcRunCfg.AdcCallBackTimes[index]  = 0;
    AdcRunCfg.Flag_Lc_Post   = index;
    AdcRunCfg.AdcValueOffset = index*ADC_SIZE_PER_CHANEL;
        //ADC值存储的地址
    AdcRunCfg.pAdcValueBase[0] = adcValue;
    AdcRunCfg.pAdcValueBase[1] = adcValue+ADC_SIZE_PER_CHANEL;
    
    AdcRunCfg.pAdcValue = adcValue+AdcRunCfg.AdcValueOffset;
      
     
    // 注意：Size参数是总转换次数，
        HAL_ADC_Stop_DMA(&hadc1);
        HAL_ADC_Stop(&hadc1);
        
    
    adcret = HAL_ADC_Start_DMA(&hadc1, (uint32_t*)dma_adcValue, ADC_DMA_DONE_CYCLE);
    //adcret = ADC_SafeStart_DMA(&hadc1, (uint32_t*)AdcRunCfg.pAdcValue, ADC_DMA_DONE_CYCLE);
    //adcret = Safe_ADC_DMA_Start(&hadc1, (uint32_t*)AdcRunCfg.pAdcValue, ADC_DMA_DONE_CYCLE);
    if ( adcret != HAL_OK) 
      
    {
        // 错误处理
        Error_Handler();
    }

}

// 替代memcpy(dst, src, 20)，80MHz下耗时≈0.2μs
static inline void copy_bytes(uint16_t *dst, const uint16_t *src)
{
    // 32位批量复制（Cortex-M4支持，比逐字节快）
    int i;
    
    for(i = 0; i < ADC_DMA_DONE_CYCLE ;i++)
    {
        dst[i] = src[i];
    }
}

/**************************************************************************
函数名称：ThreadAdcDmaDoneProc
函数版本：1.0
作者：   
创建日期：
函数功能说明：采样1次DMA完成
输入参数：      
输出参数：无
返回值：无
***************************************************************************/
void ThreadAdcDmaDoneProc()
{

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    //起始的1000个不采集
    AdcRunCfg.jump_tick++;
    if(AdcRunCfg.jump_tick < 150)
    {
      return ;
    }

    
    AdcRunCfg.AdcCallBackTimes[AdcRunCfg.Flag_Lc_Post]++;

    if(AdcRunCfg.AdcCallBackTimes[AdcRunCfg.Flag_Lc_Post] >= ADC_SIZE_PER_CHANEL / ADC_DMA_DONE_CYCLE)
    {
        AdcRunCfg.jump_tick = 0;
        HAL_ADC_Stop_DMA(&hadc1);
        xSemaphoreGiveFromISR(xLC_ADC_Finish_QueueHandle, &xHigherPriorityTaskWoken);
        return;
    }
    
    copy_bytes((uint16_t*)AdcRunCfg.pAdcValue,(uint16_t*)dma_adcValue);
    //2.重新采样
    AdcRunCfg.AdcCnt         += ADC_DMA_DONE_CYCLE;
    AdcRunCfg.AdcValueOffset += ADC_DMA_DONE_CYCLE;
    AdcRunCfg.pAdcValue      = adcValue+AdcRunCfg.AdcValueOffset;
    
}
    
//int pwm_fre[]={110000,220000,250000,80000,180000};
//int freq_index =0;
void PwmResetDuty()
{
    uint16_t present1;
    uint16_t present2;
    
    if(2 == Lc_Adc_Get_State()) //ADC起始
    {
          //1.根据AD值调整PWM输出
        present1 = dma_adcValue[0]+dma_adcValue[2]+dma_adcValue[4]+dma_adcValue[6]+dma_adcValue[8];
        present1 = present1>>6;

        present2 = dma_adcValue[1]+dma_adcValue[3]+dma_adcValue[5]+dma_adcValue[7]+dma_adcValue[9];
        present2 = present2>>6;
 
        PWM_Filter_ResetFreq( 250000,present1,present2 );

    }
    
}

void PwmSetWithAdc(uint16_t ms)
{
    int i = 0;
    uint16_t delay_us = ms * 100;
    
    //每100us更新一次pwm
    for(i = 0 ; i < delay_us ; i++)
    {
        
    }
}


static void ADC_DMA_Init(void)
{
  /*
使用时需注意以下要点：

调用时机：校准应在ADC初始化（如 HAL_ADC_Init()）之后进行，且每次从低功耗模式唤醒或上电后都需重新校准，因为校准数据在掉电后会丢失。??
1
阻塞特性：该函数是阻塞式的，会等待校准完成（通常耗时几微秒至毫秒），期间不应执行其他ADC操作。??
1
校准类型：函数内部隐含使用单端校准（ADC_SINGLE_ENDED），针对单端输入通道校准偏移和增益误差；若需差分校准，需通过其他配置实现。??
3
错误处理：调用前应确保ADC时钟稳定（建议初始化后延迟1ms再校准），并检查返回状态以确认校准成功。??
1
*/
    osDelay(1);
    // 启动ADC校准（可选但推荐，可提高精度）
    if (HAL_ADCEx_Calibration_Start(&hadc1,ADC_SINGLE_ENDED) != HAL_OK) 
    {
        // 错误处理
        Error_Handler();
    }


}

// 当DMA传输完成指定数据量（4个通道数据）后，会自动调用此函数
//CH1 CH2 为后级采样值
//CH3 CH4 为前级采样值
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    // 检查是否是ADC1的转换完成
    if (hadc->Instance == ADC1)
    {
        ThreadAdcDmaDoneProc();
        //g_adc_ready = 1; // 设置数据就绪标志
        if(0 == Lc_Adc_Get_State()) //励磁任务要求ADC停止
        {
            AdcRunCfg.jump_tick = 0;
            //HAL_ADC_Stop_DMA(&hadc1);
            HAL_ADC_Stop_DMA(&hadc1);
            HAL_ADC_Stop(&hadc1);
            hadc1.State = HAL_ADC_STATE_READY;
        }
    }
}




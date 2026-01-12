

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "timer1_pwm.h"
#include "timer2_pwm.h"

typedef struct{
  uint8_t MeasureStep;
  uint8_t ExcitationDirection;
  uint16_t PerodLcTime;  //励磁任务周期，时长
}stLctask;



/*励磁任务*/
void LC_Task_Proc(void const * argument);
//H桥方向切换
static void HbrigePost(uint8_t vaule);
static void Lc_Adc_Set_State(int8_t flag);
static void Step_Lc_Pre(void);
//反转并励磁 
static void LC_Invert(void);
static void Adc_Pre_Pwm(void);
static void Adc_Stop_Pwm(void);
static void Set_Lc_Power_Pwm(void);


static uint8_t g_lc_dir = 0; // 最后一次励磁的方向
static uint8_t g_lc_adc_stop = 1; // LC通知ADC停止

stLctask LcTask;

//系统延时us，注意最大定时65ms
extern void Os_Delay_us(int16_t us);
//extern osSemaphoreId xADCStartSemaphoreHandle;
//extern osSemaphoreId xADCStopSemaphoreHandle;
extern QueueHandle_t xADCQueueHandle ;
extern QueueHandle_t xADC_Finish_QueueHandle;
extern QueueHandle_t xCAL_Finish_QueueHandle;
extern QueueHandle_t xLC_ADC_Finish_QueueHandle; //单次励磁的ADC采样结束
extern QueueHandle_t xAdcFinishForFlashSemaphore;
extern osSemaphoreId xTask_LC_Mutex ;

//extern QueueHandle_t  Semaphore_AdcStart;

void LC_Task_Proc(void const * argument)
{
    int i = 0;
    uint8_t adc_post = 0;
    BaseType_t xSendResult = pdFALSE; // 队列发送返回值
    
    LcTask.ExcitationDirection = 0;
    osDelay(50);
    
    Step_Lc_Pre(); //初始化PWM等
    osDelay(5);
        
    for(;;)
    {

      
        // 等待锁：无限等待（pdMS_TO_TICKS(portMAX_DELAY)），直到WF任务释放锁
        if(xSemaphoreTake(xTask_LC_Mutex, portMAX_DELAY) == pdPASS)
        {
            //开始励磁，采样
            LED_LC(IO_ON);      //LED打开
            
            adc_post = 0;//第一次AD采样开始
           // PWM_TIM1_SetDutyCycle(PWM_CH2, 0.0f);  // 50%占空比  CH2是10M电阻调节PWM
            for(i = 0; i < 2; i ++)
            {
                LC_POW_EN(IO_ON);
                //反转并励磁 
                LC_Invert();
                
                Lc_Adc_Set_State(1);
                //PWM预输出
                Adc_Pre_Pwm();        
                Set_Lc_Power_Pwm();              
                PWM_TIM1_SetDutyCycle(PWM_CH2, 100.0f);
                 //增加6毫秒的延时，LC的励磁电压有一个从高位跌落到稳定的电压，大概8MS左右，之后再采样
                osDelay(2); 
                PWM_TIM1_SetDutyCycle(PWM_CH2, 0.0f);//10M电阻开关
     
                Lc_Adc_Set_State(2);
                xSendResult = xQueueSend(xADCQueueHandle, &adc_post, pdMS_TO_TICKS(100));
                if (xSendResult != pdTRUE) 
                {
                    // 释放失败处理
                    dprintf("启动ADC采样 失败!!\r\n");
                }
                 //等待采样结束，最大等待24ms
                xSendResult = xSemaphoreTake(xLC_ADC_Finish_QueueHandle, 24);
                //停止采样
                Lc_Adc_Set_State(0); 
                Adc_Stop_Pwm();
                HbrigePost(0);
                LC_POW_EN(IO_OFF);
                //PWM_TIM1_SetDutyCycle(PWM_CH2, 100.0f); 

                osDelay(4);
                
                
                adc_post++;//第n次AD采样开始
            }
            LcTask.ExcitationDirection = !LcTask.ExcitationDirection;
            LED_LC(IO_OFF);      //LED关闭
            PWM_TIM1_AllChannelsStop();
          //  Adc_Stop_Pwm();
            LC_POW_EN(IO_ON);
            HbrigePost(0);
            //采集完成，通知计算
            xSendResult = xSemaphoreGive(xADC_Finish_QueueHandle);
            if (xSendResult != pdTRUE) 
            {
                // 释放失败处理
                dprintf("启动CAL任务 失败!!\r\n");
            }
            
            
            xSendResult = xSemaphoreTake(xCAL_Finish_QueueHandle, portMAX_DELAY);
            
             // 先清空旧的信号量（防止多次释放），再释放新的
            xSemaphoreTake(xAdcFinishForFlashSemaphore, 0);
            xSendResult = xSemaphoreGive(xAdcFinishForFlashSemaphore);
            
            //计算完成了
            if (xSendResult == pdTRUE)
            {
            } 
            //释放互斥锁，写FLASH任务可继续执行
            xSemaphoreGive(xTask_LC_Mutex);
        }
            
        osDelay(200);
        //reset_all_per();
        //Step_Lc_Pre(); 
    }
}

//励磁第一阶段
void Step_Lc_Pre(void)
{

    
    CL_POW_EN(IO_ON);   //打开+-3.0V_D电源，包括电子开关U10,U11的电源
    AD_POW_EN(IO_ON);   //打开U12,U13运放正负电源开关., 给V_Digital_Buffer提供1.5V恒压
    
    AC_EN(IO_ON);       //导通电极段开关，电极信号进入放大电路
    BD_EN(IO_OFF);       //关闭LC1_PWM和LC2_PWM 的电子开关
    
  //  AC_EN(IO_OFF);       
  //  BD_EN(IO_ON);       
    
    POWER_2_5(IO_ON);   //2.5V开关
    LC_POW_EN(IO_ON);   //励磁电流会缓慢上升，提前打开
    

    //实测
    //PWM 1%    3ma
    //PWM 5%    16ma
    //PWM 6%    19.5ma
    //PWM 7%    22.1ma
    //PWM 8%    24.5ma   大于8%都是24.5ma
    //硬件上R114可以微调励磁电流
    PWM_TIM1_Init(100000);//100K 

 // 设置各通道占空比
    PWM_TIM1_SetDutyCycle(PWM_CH2, 100.0f);  // 50%占空比  CH2是10M电阻调节PWM
    PWM_TIM1_SetDutyCycle(PWM_CH3, 18.0f);  // 18%占空比  
    PWM_TIM1_SetDutyCycle(PWM_CH4, 18.0f);  // 18%占空比
    
    // 启动所有通道
    PWM_TIM1_AllChannelsStart();

}

//设置励磁电流的PWM
static void Set_Lc_Power_Pwm(void)
{
      PWM_TIM1_Init(100000);//100K 

 // 设置各通道占空比
    PWM_TIM1_SetDutyCycle(PWM_CH2, 100.0f);  // 50%占空比  CH2是10M电阻调节PWM
    PWM_TIM1_SetDutyCycle(PWM_CH3, 18.0f);  // 18%占空比  
    PWM_TIM1_SetDutyCycle(PWM_CH4, 18.0f);  // 18%占空比
    
    // 启动所有通道
    PWM_TIM1_AllChannelsStart();
}

//ADC采样前的PWM输出
static void Adc_Pre_Pwm(void)
{
    // 初始化PWM5~6，频率设置为500kHz
    //采样前开启PWM 
    if (PWM_TIM2_Init(250000) == PWM_TIM2_OK) 
    {
        // 设置各通道占空比
        PWM_TIM2_SetDutyCycle(PWM_TIM2_CH3, 50.0f);  // CH3占空比30%
        PWM_TIM2_SetDutyCycle(PWM_TIM2_CH4, 50.0f);  // CH4占空比70%
    }
   // PWM_TIM2_SetFrequency(250000);
          /* 4. 使能TIM2更新（溢出）中断 */
  __HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE);
    PWM_TIM2_AllChannelsStart();
}

static void Adc_Stop_Pwm(void)
{
    
  
      //  PWM_TIM2_SetDutyCycle(PWM_TIM2_CH3, 0.0f);  // CH3占空比30%
      //  PWM_TIM2_SetDutyCycle(PWM_TIM2_CH4, 0.0f);  // CH4占空比70%
        
      PWM_TIM2_AllChannelsStop();
}



//反转并励磁 
void LC_Invert(void)
{
    //关闭0M电子开关
    PWM_TIM1_SetDutyCycle(PWM_CH2, 0.0f);  // 0%占空比  CH2是10M电阻调节PWM
    
    //H桥导通顺序，正反励磁交替
    if(LcTask.ExcitationDirection == 0)
    {
        HbrigePost(1);
    }
    else if(LcTask.ExcitationDirection == 1)
    {
        HbrigePost(2);
    }
    LcTask.ExcitationDirection = !LcTask.ExcitationDirection;
}

//H桥方向切换
void HbrigePost(uint8_t vaule)
{
   	if(0 == vaule)//停止励磁
	{
          COIL_EN1(IO_OFF);
          COIL_EN2(IO_OFF);
	}
	else if(1 == vaule)//励磁正向
	{	  	
          COIL_EN1(IO_ON);
          COIL_EN2(IO_OFF);
          g_lc_dir = 1;
	}
	else if(2 == vaule)//励磁负向
	{
          COIL_EN1(IO_OFF);
          COIL_EN2(IO_ON);
          g_lc_dir = 0; 
	}
    else if(3 == vaule)//励磁
	{	 
          COIL_EN1(IO_ON);
          COIL_EN2(IO_ON);	
	}
	
}

//获取最后一次励磁的方向
int Get_Lc_Dir(void)
{
    return g_lc_dir;
}

//获取LC的ADC停止信号
int Lc_Adc_Get_State()
{
    return g_lc_adc_stop;
}

//获取LC的ADC停止信号
void Lc_Adc_Set_State(int8_t flag)
{
    g_lc_adc_stop = flag;
}


#include "timer2_pwm.h"

// 全局变量
static uint32_t pwm_tim2_frequency = 1000;  // 默认1kHz
static uint32_t pwm_tim2_period = 1000;     // 自动重装载值
static uint32_t pwm_tim2_prescaler = 0;     // 预分频值

/**
  * @brief  TIM2 PWM初始化函数
  * @param  frequency: PWM频率(Hz)
  * @retval PWM状态
  */
PWM_TIM2_Status PWM_TIM2_Init(uint32_t frequency)
{
    TIM_OC_InitTypeDef sConfigOC = {0};
    
    // 保存频率参数
    pwm_tim2_frequency = frequency;
    
 
    
    // 3. 配置定时器时基
    uint32_t timer_clock = HAL_RCC_GetPCLK1Freq(); // 获取TIM2时钟频率(APB1)
    if (timer_clock == 0) {
        return PWM_TIM2_ERROR;
    }
    
    // 计算预分频器和自动重装载值
    pwm_tim2_prescaler = 0;
    pwm_tim2_period = (timer_clock / frequency) - 1;
    
    // 如果周期值太大，需要增加预分频
    if (pwm_tim2_period > 0xFFFF) {
        pwm_tim2_prescaler = (timer_clock / (frequency * 0xFFFF)) - 1;
        pwm_tim2_period = (timer_clock / ((pwm_tim2_prescaler + 1) * frequency)) - 1;
    }
    
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = pwm_tim2_prescaler;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = pwm_tim2_period;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
        return PWM_TIM2_ERROR;
    }
    
    // 4. 配置PWM通道
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;  // 初始占空比为0
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    
    // 配置通道3
    if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK) {
        return PWM_TIM2_ERROR;
    }
    
    // 配置通道4
    if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK) {
        return PWM_TIM2_ERROR;
    }
    
    // 5. 启动定时器（但不启动PWM通道）
    if (HAL_TIM_Base_Start(&htim2) != HAL_OK) {
        return PWM_TIM2_ERROR;
    }
    
    return PWM_TIM2_OK;
}

/**
  * @brief  启动指定通道的PWM输出
  * @param  channel: PWM通道
  * @retval 无
  */
void PWM_TIM2_Start(PWM_TIM2_Channel channel)
{
    switch (channel) {
        case PWM_TIM2_CH3:
            HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
            break;
        case PWM_TIM2_CH4:
            HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
            break;
        default:
            break;
    }
}

/**
  * @brief  停止指定通道的PWM输出
  * @param  channel: PWM通道
  * @retval 无
  */
void PWM_TIM2_Stop(PWM_TIM2_Channel channel)
{
    switch (channel) {
        case PWM_TIM2_CH3:
            HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_3);
            break;
        case PWM_TIM2_CH4:
            HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4);
            break;
        default:
            break;
    }
}

/**
  * @brief  设置指定通道的占空比
  * @param  channel: PWM通道
  * @param  duty_cycle: 占空比 (0.0 - 100.0)
  * @retval 无
  */
void PWM_TIM2_SetDutyCycle(PWM_TIM2_Channel channel, float duty_cycle)
{
    // 限制占空比范围
    if (duty_cycle < 0.0f) duty_cycle = 0.0f;
    if (duty_cycle > 100.0f) duty_cycle = 100.0f;
    
    // 计算比较值
    uint32_t compare_value = (uint32_t)((duty_cycle / 100.0f) * pwm_tim2_period);
    
    switch (channel) {
        case PWM_TIM2_CH3:
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, compare_value);
            break;
        case PWM_TIM2_CH4:
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, compare_value);
            break;
        default:
            break;
    }
}

/**
  * @brief  设置PWM频率（所有通道）
  * @param  frequency: 新的频率值(Hz)
  * @retval 无
  */
void PWM_TIM2_SetFrequency(uint32_t frequency)
{
    // 停止所有PWM输出
    PWM_TIM2_AllChannelsStop();
    
    // 重新计算预分频器和周期值
    uint32_t timer_clock = HAL_RCC_GetPCLK1Freq();
    uint16_t new_prescaler = 0;
    uint32_t new_period = (timer_clock / frequency) - 1;
    
    if (new_period > 0xFFFF) {
        new_prescaler = (timer_clock / (frequency * 0xFFFF)) - 1;
        new_period = (timer_clock / ((new_prescaler + 1) * frequency)) - 1;
    }
    
    // 更新定时器配置
    __HAL_TIM_SET_PRESCALER(&htim2, new_prescaler);
    __HAL_TIM_SET_AUTORELOAD(&htim2, new_period);
    
    // 保存新的参数
    pwm_tim2_frequency = frequency;
    pwm_tim2_prescaler = new_prescaler;
    pwm_tim2_period = new_period;
    
      /* 4. 使能TIM2更新（溢出）中断 */
  __HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE);
  
    // 重新启动所有通道
    PWM_TIM2_AllChannelsStart();
}

//重置PWM  频率占空比
void PWM_Filter_ResetFreq(uint32_t freq, uint16_t present1, uint16_t present2)
{
    uint32_t plus1;
    uint32_t plus2;
    uint32_t uwTimerPeriod;
    
    uwTimerPeriod = (uint32_t)((SystemCoreClock / freq) - 1); //80m/250k = 320
    //uwTimerPeriod = 320;
    //present1\present2 = (4096 * 5) >>6 = 320
    
    plus1   = present1;//(uint32_t)(((uint32_t) present1 * (uwTimerPeriod - 1)) / 256);
    plus2   = present2;//(uint32_t)(((uint32_t) present2 * (uwTimerPeriod - 1)) / 256);
    
    htim2.Instance->ARR = uwTimerPeriod;
    
    htim2.Instance->CCR3 = plus1;

    htim2.Instance->CCR4 = plus2;
    
}

/**
  * @brief  获取当前PWM频率
  * @retval 当前频率值(Hz)
  */
uint32_t PWM_TIM2_GetFrequency(void)
{
    return pwm_tim2_frequency;
}

/**
  * @brief  获取指定通道的占空比
  * @param  channel: PWM通道
  * @retval 当前占空比
  */
float PWM_TIM2_GetDutyCycle(PWM_TIM2_Channel channel)
{
    uint32_t compare_value = 0;
    
    switch (channel) {
        case PWM_TIM2_CH3:
            compare_value = __HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_3);
            break;
        case PWM_TIM2_CH4:
            compare_value = __HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_4);
            break;
        default:
            return 0.0f;
    }
    
    return ((float)compare_value / pwm_tim2_period) * 100.0f;
}

/**
  * @brief  启动所有TIM2 PWM通道
  * @retval 无
  */
void PWM_TIM2_AllChannelsStart(void)
{
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
}

/**
  * @brief  停止所有TIM2 PWM通道
  * @retval 无
  */
void PWM_TIM2_AllChannelsStop(void)
{
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_3);
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4);
}
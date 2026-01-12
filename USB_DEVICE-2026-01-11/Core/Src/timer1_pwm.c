#include "timer1_pwm.h"

// 全局变量
static uint32_t pwm_frequency = 1000;  // 默认1kHz
static uint32_t pwm_period = 1000;     // 自动重装载值
static uint16_t pwm_prescaler = 0;     // 预分频值

/**
  * @brief  PWM初始化函数
  * @param  frequency: PWM频率(Hz)
  * @retval PWM状态
  */
PWM_Status PWM_TIM1_Init(uint32_t frequency)
{
    TIM_OC_InitTypeDef sConfigOC = {0};
   
    // 3. 配置定时器时基
    uint32_t timer_clock = HAL_RCC_GetPCLK2Freq(); // 获取TIM1时钟频率
    if (timer_clock == 0) {
        return PWM_ERROR;
    }
    
    // 计算预分频器和自动重装载值
    pwm_prescaler = 0;
    pwm_period = (timer_clock / frequency) - 1;
    
    // 如果周期值太大，需要增加预分频
    if (pwm_period > 0xFFFF) {
        pwm_prescaler = (timer_clock / (frequency * 0xFFFF)) - 1;
        pwm_period = (timer_clock / ((pwm_prescaler + 1) * frequency)) - 1;
    }
    
    htim1.Instance = TIM1;
    htim1.Init.Prescaler = pwm_prescaler;
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.Period = pwm_period;
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.RepetitionCounter = 0;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    
    if (HAL_TIM_Base_Init(&htim1) != HAL_OK) {
        return PWM_ERROR;
    }
    
    // 4. 配置PWM通道
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;  // 初始占空比为0
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    
    // 配置通道2
    if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK) {
        return PWM_ERROR;
    }
    
    // 配置通道3
    if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK) {
        return PWM_ERROR;
    }
    
    // 配置通道4
    if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK) {
        return PWM_ERROR;
    }
    
    // 5. 启动定时器并使能PWM输出（但不启动通道）
    if (HAL_TIM_Base_Start(&htim1) != HAL_OK) {
        return PWM_ERROR;
    }
    
    // 6. 使能高级定时器的主输出（关键步骤）
    __HAL_TIM_MOE_ENABLE(&htim1);
    
    return PWM_OK;
}

/**
  * @brief  启动指定通道的PWM输出
  * @param  channel: PWM通道
  * @retval 无
  */
void PWM_TIM1_Start(PWM_Channel channel)
{
    switch (channel) {
        case PWM_CH2:
            HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
            break;
        case PWM_CH3:
            HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
            break;
        case PWM_CH4:
            HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
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
void PWM_TIM1_Stop(PWM_Channel channel)
{
    switch (channel) {
        case PWM_CH2:
            HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
            break;
        case PWM_CH3:
            HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
            break;
        case PWM_CH4:
            HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_4);
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
void PWM_TIM1_SetDutyCycle(PWM_Channel channel, float duty_cycle)
{
    // 限制占空比范围
    if (duty_cycle < 0.0f) duty_cycle = 0.0f;
    if (duty_cycle > 100.0f) duty_cycle = 100.0f;
    
    // 计算比较值
    uint32_t compare_value = (uint32_t)((duty_cycle / 100.0f) * pwm_period);
    
    switch (channel) {
        case PWM_CH2:
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, compare_value);
            break;
        case PWM_CH3:
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, compare_value);
            break;
        case PWM_CH4:
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, compare_value);
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
void PWM_TIM1_SetFrequency(uint32_t frequency)
{
    // 停止所有PWM输出
    PWM_TIM1_AllChannelsStop();
    
    // 重新计算预分频器和周期值
    uint32_t timer_clock = HAL_RCC_GetPCLK2Freq();
    uint16_t new_prescaler = 0;
    uint32_t new_period = (timer_clock / frequency) - 1;
    
    if (new_period > 0xFFFF) {
        new_prescaler = (timer_clock / (frequency * 0xFFFF)) - 1;
        new_period = (timer_clock / ((new_prescaler + 1) * frequency)) - 1;
    }
    
    // 更新定时器配置
    __HAL_TIM_SET_PRESCALER(&htim1, new_prescaler);
    __HAL_TIM_SET_AUTORELOAD(&htim1, new_period);
    
    // 保存新的参数
    pwm_frequency = frequency;
    pwm_prescaler = new_prescaler;
    pwm_period = new_period;
    
    // 重新启动定时器
    __HAL_TIM_SET_COUNTER(&htim1, 0);
}

/**
  * @brief  获取当前PWM频率
  * @retval 当前频率值(Hz)
  */
uint32_t PWM_TIM1_GetFrequency(void)
{
    return pwm_frequency;
}

/**
  * @brief  获取指定通道的占空比
  * @param  channel: PWM通道
  * @retval 当前占空比
  */
float PWM_TIM1_GetDutyCycle(PWM_Channel channel)
{
    uint32_t compare_value = 0;
    
    switch (channel) {
        case PWM_CH2:
            compare_value = __HAL_TIM_GET_COMPARE(&htim1, TIM_CHANNEL_2);
            break;
        case PWM_CH3:
            compare_value = __HAL_TIM_GET_COMPARE(&htim1, TIM_CHANNEL_3);
            break;
        case PWM_CH4:
            compare_value = __HAL_TIM_GET_COMPARE(&htim1, TIM_CHANNEL_4);
            break;
        default:
            return 0.0f;
    }
    
    return ((float)compare_value / pwm_period) * 100.0f;
}

/**
  * @brief  启动所有PWM通道
  * @retval 无
  */
void PWM_TIM1_AllChannelsStart(void)
{
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
}

/**
  * @brief  停止所有PWM通道
  * @retval 无
  */
void PWM_TIM1_AllChannelsStop(void)
{
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_4);
}
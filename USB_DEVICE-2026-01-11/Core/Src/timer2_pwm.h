#ifndef __PWM_TIM2_H
#define __PWM_TIM2_H

#include "main.h"
#include "tim.h"

// PWM通道定义
typedef enum {
    PWM_TIM2_CH3 = 0,
    PWM_TIM2_CH4
} PWM_TIM2_Channel;

// 初始化状态
typedef enum {
    PWM_TIM2_OK = 0,
    PWM_TIM2_ERROR
} PWM_TIM2_Status;

// 函数声明
PWM_TIM2_Status PWM_TIM2_Init(uint32_t frequency);
void PWM_TIM2_Start(PWM_TIM2_Channel channel);
void PWM_TIM2_Stop(PWM_TIM2_Channel channel);
void PWM_TIM2_SetDutyCycle(PWM_TIM2_Channel channel, float duty_cycle);
void PWM_TIM2_SetFrequency(uint32_t frequency);
uint32_t PWM_TIM2_GetFrequency(void);
float PWM_TIM2_GetDutyCycle(PWM_TIM2_Channel channel);
void PWM_TIM2_AllChannelsStart(void);
void PWM_TIM2_AllChannelsStop(void);

void PWM_Filter_ResetFreq(uint32_t freq, uint16_t present1, uint16_t present2);

#endif /* __PWM_TIM2_H */
#ifndef __PWM_TIM1_H
#define __PWM_TIM1_H

#include "main.h"
#include "tim.h"

// PWM通道定义
typedef enum {
    PWM_CH2 = 0,
    PWM_CH3,
    PWM_CH4
} PWM_Channel;

// 初始化状态
typedef enum {
    PWM_OK = 0,
    PWM_ERROR
} PWM_Status;

// 函数声明
PWM_Status PWM_TIM1_Init(uint32_t frequency);
void PWM_TIM1_Start(PWM_Channel channel);
void PWM_TIM1_Stop(PWM_Channel channel);
void PWM_TIM1_SetDutyCycle(PWM_Channel channel, float duty_cycle);
void PWM_TIM1_SetFrequency(uint32_t frequency);
uint32_t PWM_TIM1_GetFrequency(void);
float PWM_TIM1_GetDutyCycle(PWM_Channel channel);
void PWM_TIM1_AllChannelsStart(void);
void PWM_TIM1_AllChannelsStop(void);

#endif /* __PWM_TIM1_H */
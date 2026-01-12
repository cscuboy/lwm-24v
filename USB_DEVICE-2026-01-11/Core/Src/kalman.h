#ifndef _KALMAN_H_
#define _KALMAN_H_

#include "stdint.h"
#include "math.h"

/* 卡尔曼滤波参数结构体（一维） */
typedef struct
{
    float X_est;    // 最优估计值（滤波后流量）
    float P_est;    // 估计协方差
    float Q;        // 过程噪声协方差（可调）
    float R;        // 测量噪声协方差（可调）
} KalmanFilter_Typedef;

void EMFlow_Kalman_Init( float init_X, float Q, float R);
float EMFlow_Kalman_Update(float Z_k);

void  set_flow_avg_ssll_simple(float current_val);
float get_flow_avg_ssll_simple(void);

#endif

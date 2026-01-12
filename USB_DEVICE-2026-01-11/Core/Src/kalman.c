#include "FreeRTOS.h"
#include "kalman.h"





/* 全局卡尔曼滤波器实例（电磁流量计专用） */
KalmanFilter_Typedef pKalman;

/**
 * @brief  电磁流量计卡尔曼滤波初始化
 * @param  pKalman: 滤波器实例指针
 * @param  init_X: 初始估计值（建议设为第一个采样值）
 * @param  Q: 过程噪声协方差（默认0.01，流量变化慢设小）
 * @param  R: 测量噪声协方差（默认1.0，传感器噪声大设大）
 * @retval 无
 */
void EMFlow_Kalman_Init(float init_X, float Q, float R)
{
    pKalman.X_est = init_X;  // 初始估计值
    pKalman.P_est = 1.0f;    // 初始协方差（默认1.0）
    pKalman.Q = Q;           // 过程噪声（推荐0.01~0.1）
    pKalman.R = R;           // 测量噪声（推荐0.5~2.0）
}


/**
 * @brief  卡尔曼滤波迭代（输入采样值，输出滤波值）
 * @param  pKalman: 滤波器实例指针
 * @param  Z_k: 当前采样值（电磁流量计原始数据）
 * @retval float: 滤波后的最优流量值
 */
float EMFlow_Kalman_Update(float Z_k)
{
   // if (pKalman == NULL) return Z_k; // 空指针则返回原始值

    /* 1. 状态预测：X(k|k-1) = X(k-1|k-1) */
    float X_pred = pKalman.X_est;

    /* 2. 协方差预测：P(k|k-1) = P(k-1|k-1) + Q */
    float P_pred = pKalman.P_est + pKalman.Q;

    /* 3. 计算卡尔曼增益：K(k) = P_pred / (P_pred + R) */
    float K_k = P_pred / (P_pred + pKalman.R);

    /* 4. 状态更新：融合采样值 */
    pKalman.X_est = X_pred + K_k * (Z_k - X_pred);

    /* 5. 协方差更新 */
    pKalman.P_est = (1 - K_k) * P_pred;

    /* 返回滤波后的值 */
    return pKalman.X_est;
}



static float lcd_sslc_data_buf[10] = {0};  // 固定存放10个测量点

/**
 * @brief  电磁流量计10点平均滤波函数-极简版
 * @param  current_val 当前测量值
 * @retval 最近10个测量值的平均值
 * @note   逻辑简单，新手友好，满10个值后才开始输出真实10点平均
 */
void set_flow_avg_ssll_simple(float current_val)
{
    static uint8_t  index = 0;           // 数据存储下标


    lcd_sslc_data_buf[index++] = current_val;     // 存入当前值，下标自增
    if(index >= 10) index = 0;           // 下标到10则清零，循环覆盖


}
/* * @brief  电磁流量计10点平均滤波函数-极简版
*/
float get_flow_avg_ssll_simple(void)
{
    float sum = 0;
    uint8_t  i;
    
     // 累加全部10个缓存值
    for(i=0; i<10; i++)
    {
        sum += lcd_sslc_data_buf[i];
    }

    return (float)sum / 10;  // 严格返回10个点的平均值，满足你的核心需求  
}




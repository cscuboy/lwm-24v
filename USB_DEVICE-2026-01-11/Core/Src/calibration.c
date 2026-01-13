#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "calibration.h"


// 校准系统结构
typedef struct {
    LONG small_flow_point;// 小流量点的实际ADC值
    LONG large_flow_point;// 大流量点的实际ADC值
    
    FLOAT *small_actual_flow;        // 表测流量点的流量值
    FLOAT *small_meter_flow;        // 表测流量点的流量值
    // 中间计算参数
    LONG *zero_point;        // 修正零点原始滤波值orzero
    FLOAT *k_small;            // 小流量点K
    FLOAT *k_large;            // 大流量点K
    
    FLOAT *k_convert;            // 转换器系数
    
  //  FLOAT cal_k_small; //通过流量校正的K系数
 //   FLOAT cal_k_large; //通过流量校正的K系数
} CalibrationSystem_t;



// 全局校准系统实例
static CalibrationSystem_t g_calibration_sys;// = {0};
// ==================== 校准点设置函数 ====================


//设置默认的校正参数
void  calibration_init_default_params(EMDCB_Params_t* params)
{
//小流量点实际流量值
    params->small_flow_meter  =0.1;
    params->small_flow_actual =0.1;
    //大流量点实际流量值
    params->large_flow_meter = 10.0;
    params->large_flow_actual= 10.0;
    //零点的仪表测量值
    params->zero_point_meter= 0.01;
    //转换系数
    params->converter_factor = 1.0;
    
    params->Internal_zero_point = 300000; //流量计的零点，adc的滤波值
    params->Internal_K_small = 0.000012; //小流量的K系数
    params->Internal_K_large = 0.000012; //大流量的K系数
}


    
//计算通过大流量点小流量点和零点值，计算K系数和零点值的ADC值
void Init_Calibration_Param(EMDCB_Params_t* params)
{
      
    g_calibration_sys.zero_point = &params->Internal_zero_point; //流量计的零点，adc的滤波值
    g_calibration_sys.k_small = &params->Internal_K_small; //小流量的K系数
    g_calibration_sys.k_large = &params->Internal_K_large; //大流量的K系数 
    g_calibration_sys.small_actual_flow = &params->small_flow_actual;//小流量的实际值
    g_calibration_sys.small_meter_flow = &params->small_flow_meter;//小流量的实际值
    g_calibration_sys.k_convert =  &params->converter_factor;//转换系数
    
    //通过K系数和0点采样值，计算出小流量采样值和大流量采样值，用于分段比较
    g_calibration_sys.small_flow_point = params->Internal_zero_point +(LONG) (params->small_flow_meter/params->Internal_K_small);
    g_calibration_sys.large_flow_point = params->Internal_zero_point + (LONG)(params->large_flow_meter/params->Internal_K_large);
    
      
    //计算大流量点K 校准
  /*  FLOAT meter_diff = params->large_flow_meter - params->small_flow_meter;
    FLOAT flow_diff = params->large_flow_actual - params->small_flow_actual;
    
    g_calibration_sys.cal_k_large = flow_diff/meter_diff;
    
    // 计算小流量点K 校准
    g_calibration_sys.cal_k_small = =  (params->small_flow_actual) / (small_flow_meter);*/

}




/**
 * @brief  设置零点校准数据
 * @param  filter_value: 零点仪表测量值
 * @retval 设置是否成功
 */
bool Calibration_SetZeroPoint(FLOAT read_flow) 
{

    LONG *zero_offset = g_calibration_sys.zero_point;
    
    // 3. 计算修正零点原始滤波值orzero
    // 公式: 修正零点原始滤波值orzero = 零点的仪表测量值 / 小流量点K + 零点的的原始滤波值orzero
    *zero_offset += read_flow / *g_calibration_sys.k_small;
    
    
    return true;
}
           

/**
 * @brief  设置小流量点校准数据
 * @param  read_flow: 小流量点仪表测量值
 * @param  actual_flow: 小流量点实际流量值
 * @retval 设置是否成功
 */
bool Calibration_SetSmallFlowPoint(FLOAT read_flow, FLOAT actual_flow) 
{
   
    //校正小流量
    FLOAT *k_small = g_calibration_sys.k_small;
    
      //1.重新调整小流量K系数
    *k_small = *k_small * (actual_flow  ) / (read_flow);
    
    //通过K系数和0点采样值，计算出小流量采样值和大流量采样值，用于分段比较
    g_calibration_sys.small_flow_point = *g_calibration_sys.zero_point + (LONG)(read_flow/(*k_small));
    
    
    return true;
}


/**
 * @brief  设置大流量点校准数据
 * @param  read_flow: 大流量点仪表测量值
 * @param  actual_flow: 大流量点实际流量值
 * @retval 设置是否成功
 */
bool Calibration_SetLargeFlowPoint(FLOAT small_read_flow, FLOAT small_actual_flow,FLOAT large_read_flow, FLOAT large_actual_flow)
{

    
    FLOAT *k_large = g_calibration_sys.k_large;
    
    // 2. 计算大流量点K
    FLOAT meter_diff = large_read_flow - small_read_flow;
    
    FLOAT flow_diff = large_actual_flow - small_actual_flow;
    
    *k_large = (*k_large) *  flow_diff / meter_diff;
    
    return true;
}

// ==================== 校准计算函数 ====================


/**
 * @brief  应用分段线性修正
 * @param  raw_filter_value: 原始的滤波值orlv
 * @retval 修正后的实际流量
 */
FLOAT Calibration_ApplyCorrection(LONG raw_filter_value) 
{

    
    FLOAT corrected_flow = 0.0f;
    
    // 仪表测量值 = 原始的滤波值orlv - 修正零点原始滤波值orzero
    LONG meter_value = raw_filter_value - (*g_calibration_sys.zero_point);
    
    if (raw_filter_value <= g_calibration_sys.small_flow_point) 
    {
        // 小流量段: 修正流量 = 仪表测量值 × 小流量点K
        corrected_flow = meter_value * (*g_calibration_sys.k_small);
    } 
    else 
    {
        // 大流量段: 修正流量 = 小流量的实际流量值 + (大流量点的原始的滤波值orlv-小流量点的原始的滤波值orlv) * 大流量点K
        // 注意: 这里应该是当前的原始滤波值与小流量点的原始滤波值之差乘以大流量点K        
        corrected_flow = *g_calibration_sys.small_actual_flow;
        corrected_flow += (raw_filter_value - g_calibration_sys.small_flow_point) * (*g_calibration_sys.k_large);
    }
    //转换器系数
    corrected_flow *= *g_calibration_sys.k_convert;
    
    return corrected_flow;
}


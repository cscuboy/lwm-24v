#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "Modbus_index.h"
#include "Modbus_Param.h"

// 数据类型定义
//typedef float FLOAT;

// 校准点结构
typedef struct {
    FLOAT filter_value;      // 仪表测量值（原始滤波值orlv）
    FLOAT *read_flow;        // 表测度数流量值
    FLOAT *actual_flow;        // 实际流量值
    FLOAT correction_factor;  // 修正系数K
} CalibrationPoint_t;

// 校准系统结构
typedef struct {
    // 三个校准点
    CalibrationPoint_t zero_point;      // 零点
    CalibrationPoint_t small_flow_point; // 小流量点
    CalibrationPoint_t large_flow_point; // 大流量点
    
    // 中间计算参数
    LONG *zero_offset;        // 修正零点原始滤波值orzero
    FLOAT *k_small;            // 小流量点K
    FLOAT *k_large;            // 大流量点K
    
    // 校准状态
   // bool is_calibrated;      // 是否已校准
  //  bool calibration_mode;   // 校准模式: false=正常模式, true=标定模式
} CalibrationSystem_t;

// 全局校准系统实例
static CalibrationSystem_t g_calibration_sys;// = {0};

//K系数表
static const float k_init_value=0.0000152;
// ==================== 校准点设置函数 ====================


    
//计算通过大流量点小流量点和零点值，计算K系数和零点值的ADC值
void Init_Calibration_Param(EMDCB_Params_t* params)
{
      //小流量的实际流量值
      g_calibration_sys.small_flow_point.read_flow = &params->small_flow_meter;
      g_calibration_sys.small_flow_point.actual_flow = &params->small_flow_actual;
      
      //大流量点实际流量值
      g_calibration_sys.large_flow_point.read_flow = &params->large_flow_meter;
      g_calibration_sys.large_flow_point.actual_flow = &params->small_flow_actual;
      
      //零点的仪表测量值
      g_calibration_sys.zero_point.read_flow = &params->zero_point_meter;
      
      
      g_calibration_sys.zero_offset = &params->Internal_zero_point; //流量计的零点，adc的滤波值
      g_calibration_sys.k_small = &params->Internal_K_small; //小流量的K系数
      g_calibration_sys.k_large = &params->Internal_K_large; //大流量的K系数 
    
      //重新计算零点和K值
      //Reset_Zero_Point();
      //Reset_Small_K();
      //Reset_Large_K();
}


void Reset_Small_K(void)
{
    FLOAT *k_small = g_calibration_sys.k_small;
    
      //1.重新调整小流量K系数
    *k_small = *k_small * (*g_calibration_sys.small_flow_point.actual_flow) / (*g_calibration_sys.small_flow_point.read_flow);
    //2.计算小流量的采样值是多少
    g_calibration_sys.small_flow_point.filter_value = (*g_calibration_sys.zero_offset) + \
                                                      (*g_calibration_sys.small_flow_point.read_flow) / (*k_small);//
}

void Reset_Large_K(void)
{
    FLOAT *k_large = g_calibration_sys.k_large;
    
          // 2. 计算大流量点K
    FLOAT meter_diff = g_calibration_sys.large_flow_point.read_flow - 
                      g_calibration_sys.small_flow_point.read_flow;
    FLOAT flow_diff = g_calibration_sys.large_flow_point.actual_flow - 
                     g_calibration_sys.small_flow_point.actual_flow;
    
    *k_large = (*k_large) *  flow_diff / meter_diff;
}

//校正零点
void Reset_Zero_Point(void)
{
    LONG *zero_offset = g_calibration_sys.zero_offset;
    
    // 3. 计算修正零点原始滤波值orzero
    // 公式: 修正零点原始滤波值orzero = 零点的仪表测量值 / 小流量点K + 零点的的原始滤波值orzero
    *zero_offset = (LONG)(*g_calibration_sys.zero_point.read_flow / 
                                   *g_calibration_sys.k_small) + 
                                   *g_calibration_sys.zero_offset;    
}

/**
 * @brief  设置零点校准数据
 * @param  filter_value: 零点仪表测量值
 * @retval 设置是否成功
 */
bool Calibration_SetZeroPoint(FLOAT read_flow) 
{

    *g_calibration_sys.zero_point.read_flow = read_flow;
    //g_calibration_sys.zero_point.actual_flow = 0.0f;  // 零点实际流量为0
    
    //校正零点
    Reset_Zero_Point();
    
    
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

    *g_calibration_sys.small_flow_point.read_flow   = read_flow;
    *g_calibration_sys.small_flow_point.actual_flow = actual_flow;
    
    //校正小流量
    Reset_Small_K();
    
    return true;
}


/**
 * @brief  设置大流量点校准数据
 * @param  read_flow: 大流量点仪表测量值
 * @param  actual_flow: 大流量点实际流量值
 * @retval 设置是否成功
 */
bool Calibration_SetLargeFlowPoint(FLOAT read_flow, FLOAT actual_flow)
{

    *g_calibration_sys.large_flow_point.read_flow = read_flow;
    *g_calibration_sys.large_flow_point.actual_flow = actual_flow;
       
    //校正大流量
    Reset_Large_K();
    
    
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
    LONG meter_value = raw_filter_value - (*g_calibration_sys.zero_offset);
    
    if (raw_filter_value <= g_calibration_sys.small_flow_point.filter_value) 
    {
        // 小流量段: 修正流量 = 仪表测量值 × 小流量点K
        corrected_flow = meter_value * (*g_calibration_sys.k_small);
    } 
    else 
    {
        // 大流量段: 修正流量 = 小流量的实际流量值 + (大流量点的原始的滤波值orlv-小流量点的原始的滤波值orlv) * 大流量点K
        // 注意: 这里应该是当前的原始滤波值与小流量点的原始滤波值之差乘以大流量点K
        FLOAT small_meter_value = g_calibration_sys.small_flow_point.filter_value - 
                                 *g_calibration_sys.zero_offset;
        
        corrected_flow = *g_calibration_sys.small_flow_point.actual_flow + 
                        (meter_value - small_meter_value) * g_calibration_sys.k_large;
    }
    
    
    return corrected_flow;
}


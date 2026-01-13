#ifndef CALIBRATIAN_H
#define CALIBRATIAN_H
#include <stdbool.h>
#include "Modbus_index.h"
#include "Modbus_Param.h"


//设置默认的校正参数
void  calibration_init_default_params(EMDCB_Params_t* params);

//初始化校准 参数
void Init_Calibration_Param(EMDCB_Params_t* params);

//校准零点
bool Calibration_SetZeroPoint(FLOAT read_flow) ;
//校准小流量
bool Calibration_SetSmallFlowPoint(FLOAT read_flow, FLOAT actual_flow) ;
//校准大流量
bool Calibration_SetLargeFlowPoint(FLOAT small_read_flow, FLOAT small_actual_flow,FLOAT large_read_flow, FLOAT large_actual_flow);
//校准计算函数
FLOAT Calibration_ApplyCorrection(LONG raw_filter_value) ;

#endif // EMDCB_MODBUS_INDEX_H
#include "Modbus_Param.h"
#include "calibration.h"
#include "Modbus_Reg_Addr.h"

#include <string.h>

// 全局变量
extern EMDCB_Params_t g_emdcb_params;
static uint8_t current_permission = PERM_READ_ONLY;  // 当前权限等级

// ==================== 通用辅助函数 ====================

/**
 * @brief  检查当前权限是否足够
 * @param  required_perm: 需要的权限等级
 * @retval 1: 权限足够, 0: 权限不足
 */
/*uint8_t check_permission(RegisterPermission required_perm) 
{
    return (current_permission >= required_perm);
}*/

/**
 * @brief  设置当前权限等级
 * @param  perm: 新的权限等级
 * @retval 无
 */
/*void emdcb_set_permission(uint8_t perm) 
{
    if (perm <= PERM_LEVEL_03) 
    {
        current_permission = perm;
    }
}*/

/**
 * @brief  获取当前权限等级
 * @retval 当前权限等级
 */
/*uint8_t emdcb_get_permission(void) 
{
    return current_permission;
}*/


// ==================== 流量测量参数函数 ====================

/**
 * @brief  读取流量测量参数
 * @param  flow: 瞬时流量输出
 * @param  velocity: 流速输出
 * @param  percentage: 流量百分比输出
 * @retval 无
 */
void emdcb_read_flow_measurement(FLOAT *flow, FLOAT *velocity, FLOAT *percentage) 
{
    if (flow) *flow = g_emdcb_params.instant_flow;
    if (velocity) *velocity = g_emdcb_params.flow_velocity;
    if (percentage) *percentage = g_emdcb_params.flow_percentage;
}

/**
 * @brief  设置流量单位
 * @param  unit: 流量单位 (0-11)
 * @retval 0: 成功, 1: 权限不足, 2: 参数无效
 */
uint8_t emdcb_set_flow_unit(SHORT unit) 
{
    if (!check_permission(PERM_READ_ONLY)) return 1;  // 只读权限即可
    if (unit < 0 || unit > 11) return 2;  // 无效参数
    
    g_emdcb_params.flow_unit = unit;
    return 0;
}

// ==================== 累积流量参数函数 ====================

/**
 * @brief  读取正向累积流量
 * @param  total_int: 整数部分输出
 * @param  total_frac: 小数部分输出
 * @retval 无
 */
void emdcb_read_positive_total(LONG *total_int, FLOAT *total_frac) 
{
    if (total_int) *total_int = g_emdcb_params.positive_total_int;
    if (total_frac) *total_frac = g_emdcb_params.positive_total_frac;
}

/**
 * @brief  读取反向累积流量
 * @param  total_int: 整数部分输出
 * @param  total_frac: 小数部分输出
 * @retval 无
 */
void emdcb_read_negative_total(LONG *total_int, FLOAT *total_frac) 
{
    if (total_int) *total_int = g_emdcb_params.negative_total_int;
    if (total_frac) *total_frac = g_emdcb_params.negative_total_frac;
}

/**
 * @brief  设置正向累积流量
 * @param  total_int: 整数部分
 * @param  total_frac: 小数部分
 * @retval 0: 成功, 1: 权限不足
 */
uint8_t emdcb_set_positive_total(LONG total_int, FLOAT total_frac) 
{
    if (!check_permission(PERM_LEVEL_03)) return 1;
    
    g_emdcb_params.positive_total_int = total_int;
    g_emdcb_params.positive_total_frac = total_frac;
    return 0;
}

/**
 * @brief  设置反向累积流量
 * @param  total_int: 整数部分
 * @param  total_frac: 小数部分
 * @retval 0: 成功, 1: 权限不足
 */
uint8_t emdcb_set_negative_total(LONG total_int, FLOAT total_frac) 
{
    if (!check_permission(PERM_LEVEL_03)) return 1;
    
    g_emdcb_params.negative_total_int = total_int;
    g_emdcb_params.negative_total_frac = total_frac;
    return 0;
}

// ==================== 报警状态参数函数 ====================

/**
 * @brief  读取所有报警状态
 * @param  alarms: 报警状态结构体指针
 * @retval 无
 */
/*void emdcb_read_alarm_status(AlarmStatus_t *alarms) 
{
    if (alarms) 
    {
        alarms->bits.excitation_alarm = g_emdcb_params.excitation_alarm;
        alarms->bits.empty_pipe_alarm = g_emdcb_params.empty_pipe_alarm;
        alarms->bits.reverse_flow_alarm = g_emdcb_params.reverse_flow_alarm;
        alarms->bits.flow_high_alarm = g_emdcb_params.flow_high_alarm;
        alarms->bits.flow_low_alarm = g_emdcb_params.flow_low_alarm;
        alarms->bits.battery_low_alarm = g_emdcb_params.battery_low_alarm;
        alarms->bits.memory_fault = g_emdcb_params.memory_fault_alarm;
        alarms->bits.pressure_high_alarm = g_emdcb_params.pressure_high_alarm;
        alarms->bits.pressure_low_alarm = g_emdcb_params.pressure_low_alarm;
        alarms->bits.temp_high_alarm = g_emdcb_params.temperature_high_alarm;
        alarms->bits.temp_low_alarm = g_emdcb_params.temperature_low_alarm;
    }
}*/

/**
 * @brief  清除所有报警状态
 * @retval 0: 成功, 1: 权限不足
 */
uint8_t emdcb_clear_all_alarms(void) 
{
    if (!check_permission(PERM_LEVEL_01)) return 1;
    
    g_emdcb_params.excitation_alarm = 0;
    g_emdcb_params.empty_pipe_alarm = 0;
    g_emdcb_params.reverse_flow_alarm = 0;
    g_emdcb_params.flow_high_alarm = 0;
    g_emdcb_params.flow_low_alarm = 0;
    g_emdcb_params.battery_low_alarm = 0;
    g_emdcb_params.memory_fault_alarm = 0;
    g_emdcb_params.pressure_high_alarm = 0;
    g_emdcb_params.pressure_low_alarm = 0;
    g_emdcb_params.temperature_high_alarm = 0;
    g_emdcb_params.temperature_low_alarm = 0;
    
    return 0;
}

// ==================== 设备参数函数 ====================

/**
 * @brief  读取设备基本参数
 * @param  range: 仪表量程输出
 * @param  caliber: 仪表口径输出
 * @param  direction: 流量方向输出
 * @retval 无
 */
void emdcb_read_device_params(FLOAT *range, SHORT *caliber, SHORT *direction) 
{
    if (range) *range = g_emdcb_params.meter_range;
    if (caliber) *caliber = g_emdcb_params.meter_caliber;
    if (direction) *direction = g_emdcb_params.flow_direction_set;
}

/**
 * @brief  设置设备基本参数
 * @param  range: 仪表量程
 * @param  caliber: 仪表口径
 * @param  direction: 流量方向
 * @retval 0: 成功, 1: 权限不足, 2: 参数无效
 */
uint8_t emdcb_set_device_params(FLOAT range, SHORT caliber, SHORT direction) 
{
    if (!check_permission(PERM_LEVEL_01)) return 1;
    if (range <= 0) return 2;
    if (caliber <= 0) return 2;
    if (direction < 0 || direction > 1) return 2;
    
    g_emdcb_params.meter_range = range;
    g_emdcb_params.meter_caliber = caliber;
    g_emdcb_params.flow_direction_set = direction;
    
    return 0;
}

// ==================== 报警阈值参数函数 ====================

/**
 * @brief  读取流量报警阈值
 * @param  high_threshold: 上限阈值输出
 * @param  low_threshold: 下限阈值输出
 * @retval 无
 */
void emdcb_read_flow_thresholds(FLOAT *high_threshold, FLOAT *low_threshold)
{
    if (high_threshold) *high_threshold = g_emdcb_params.flow_high_threshold;
    if (low_threshold) *low_threshold = g_emdcb_params.flow_low_threshold;
}

/**
 * @brief  设置流量报警阈值
 * @param  high_threshold: 上限阈值
 * @param  low_threshold: 下限阈值
 * @retval 0: 成功, 1: 权限不足, 2: 参数无效
 */
uint8_t emdcb_set_flow_thresholds(FLOAT high_threshold, FLOAT low_threshold) 
{
    if (!check_permission(PERM_LEVEL_01)) return 1;
    if (high_threshold <= low_threshold) return 2;
    if (high_threshold > g_emdcb_params.meter_range) return 2;
    if (low_threshold < 0) return 2;
    
    g_emdcb_params.flow_high_threshold = high_threshold;
    g_emdcb_params.flow_low_threshold = low_threshold;
    
    return 0;
}

// ==================== 时段设置参数函数 ====================

/**
 * @brief  读取时段设置参数
 * @param  start1: 时段1起始小时输出
 * @param  start2: 时段2起始小时输出
 * @param  interval1: 时段1间隔时间输出
 * @param  interval2: 时段2间隔时间输出
 * @retval 无
 */
void emdcb_read_period_settings(SHORT *start1, SHORT *start2, SHORT *interval1, SHORT *interval2) 
{
    if (start1) *start1 = g_emdcb_params.period1_start_hour;
    if (start2) *start2 = g_emdcb_params.period2_start_hour;
    if (interval1) *interval1 = g_emdcb_params.period1_interval;
    if (interval2) *interval2 = g_emdcb_params.period2_interval;
}

/**
 * @brief  设置时段参数
 * @param  start1: 时段1起始小时
 * @param  start2: 时段2起始小时
 * @param  interval1: 时段1间隔时间
 * @param  interval2: 时段2间隔时间
 * @retval 0: 成功, 1: 权限不足, 2: 参数无效
 */
uint8_t emdcb_set_period_settings(SHORT start1, SHORT start2, SHORT interval1, SHORT interval2) 
{
    if (!check_permission(PERM_LEVEL_02)) return 1;
    if (start1 < 0 || start1 > 23) return 2;
    if (start2 < 0 || start2 > 23) return 2;
    if (interval1 < 1 || interval1 > 60) return 2;
    if (interval2 < 1 || interval2 > 60) return 2;
    
    g_emdcb_params.period1_start_hour = start1;
    g_emdcb_params.period2_start_hour = start2;
    g_emdcb_params.period1_interval = interval1;
    g_emdcb_params.period2_interval = interval2;
    
    return 0;
}

// ==================== 转换器参数函数 ====================

/**
 * @brief  读取转换器参数
 * @param  density: 流体密度输出
 * @param  factor: 转换器系数输出
 * @retval 无
 */
void emdcb_read_converter_params(FLOAT *density, FLOAT *factor) 
{
    if (density) *density = g_emdcb_params.fluid_density;
    if (factor) *factor = g_emdcb_params.converter_factor;
}

/**
 * @brief  设置转换器参数
 * @param  density: 流体密度
 * @param  factor: 转换器系数
 * @retval 0: 成功, 1: 权限不足, 2: 参数无效
 */
uint8_t emdcb_set_converter_params(FLOAT density, FLOAT factor) 
{
    if (!check_permission(PERM_LEVEL_02)) return 1;
    if (density <= 0) return 2;
    if (factor <= 0) return 2;
    
    g_emdcb_params.fluid_density = density;
    g_emdcb_params.converter_factor = factor;
    
    return 0;
}

// ==================== 电源参数函数 ====================

/**
 * @brief  读取电源参数
 * @param  voltage: 电池电压输出
 * @param  low_alarm: 低电量报警输出
 * @retval 无
 */
void emdcb_read_power_params(SHORT *voltage, SHORT *low_alarm) 
{
    if (voltage) *voltage = g_emdcb_params.battery_voltage;
    if (low_alarm) *low_alarm = g_emdcb_params.battery_low_alarm;
}

// ==================== 压力参数函数 ====================

/**
 * @brief  读取压力参数
 * @param  value: 压力值输出
 * @param  high_threshold: 上限阈值输出
 * @param  low_threshold: 下限阈值输出
 * @retval 无
 */
void emdcb_read_pressure_params(FLOAT *value, FLOAT *high_threshold, FLOAT *low_threshold) 
{
    if (value) *value = g_emdcb_params.pressure_value;
    if (high_threshold) *high_threshold = g_emdcb_params.pressure_high_threshold;
    if (low_threshold) *low_threshold = g_emdcb_params.pressure_low_threshold;
}

/**
 * @brief  设置压力报警阈值
 * @param  high_threshold: 上限阈值
 * @param  low_threshold: 下限阈值
 * @retval 0: 成功, 1: 权限不足, 2: 参数无效
 */
uint8_t emdcb_set_pressure_thresholds(FLOAT high_threshold, FLOAT low_threshold)
{
    if (!check_permission(PERM_LEVEL_01)) return 1;
    if (high_threshold <= low_threshold) return 2;
    
    g_emdcb_params.pressure_high_threshold = high_threshold;
    g_emdcb_params.pressure_low_threshold = low_threshold;
    
    return 0;
}

// ==================== 温度参数函数 ====================

/**
 * @brief  读取温度参数
 * @param  value: 温度值输出
 * @param  high_threshold: 上限阈值输出
 * @param  low_threshold: 下限阈值输出
 * @retval 无
 */
void emdcb_read_temperature_params(FLOAT *value, FLOAT *high_threshold, FLOAT *low_threshold)
{
    if (value) *value = g_emdcb_params.temperature_value;
    if (high_threshold) *high_threshold = g_emdcb_params.temperature_high_threshold;
    if (low_threshold) *low_threshold = g_emdcb_params.temperature_low_threshold;
}

/**
 * @brief  设置温度报警阈值
 * @param  high_threshold: 上限阈值
 * @param  low_threshold: 下限阈值
 * @retval 0: 成功, 1: 权限不足, 2: 参数无效
 */
uint8_t emdcb_set_temperature_thresholds(FLOAT high_threshold, FLOAT low_threshold) 
{
    if (!check_permission(PERM_LEVEL_01)) return 1;
    if (high_threshold <= low_threshold) return 2;
    
    g_emdcb_params.temperature_high_threshold = high_threshold;
    g_emdcb_params.temperature_low_threshold = low_threshold;
    
    return 0;
}

// ==================== 通信参数函数 ====================

/**
 * @brief  读取通信参数
 * @param  addr: Modbus地址输出
 * @param  baudrate: 波特率输出
 * @param  data_bits: 数据位输出
 * @param  stop_bits: 停止位输出
 * @param  parity: 校验位输出
 * @retval 无
 */
void emdcb_read_comm_params(SHORT *addr, SHORT *baudrate, SHORT *data_bits, 
                           SHORT *stop_bits, SHORT *parity)
{
    if (addr) *addr = g_emdcb_params.modbus_addr;
    if (baudrate) *baudrate = g_emdcb_params.modbus_baudrate;
    if (data_bits) *data_bits = g_emdcb_params.modbus_data_bits;
    if (stop_bits) *stop_bits = g_emdcb_params.modbus_stop_bits;
    if (parity) *parity = g_emdcb_params.modbus_parity;
}

/**
 * @brief  设置通信参数
 * @param  addr: Modbus地址
 * @param  baudrate: 波特率
 * @param  data_bits: 数据位
 * @param  stop_bits: 停止位
 * @param  parity: 校验位
 * @retval 0: 成功, 1: 权限不足, 2: 参数无效
 */
uint8_t emdcb_set_comm_params(SHORT addr, SHORT baudrate, SHORT data_bits, 
                             SHORT stop_bits, SHORT parity) 
{
    if (!check_permission(PERM_LEVEL_03)) return 1;
    if (addr < 1 || addr > 247) return 2;
    if (baudrate != 1200 && baudrate != 2400 && baudrate != 4800 && 
        baudrate != 9600 && baudrate != 19200) return 2;
    if (data_bits != 7 && data_bits != 8) return 2;
    if (stop_bits != 1 && stop_bits != 2) return 2;
    if (parity < 0 || parity > 2) return 2;
    
    g_emdcb_params.modbus_addr = addr;
    g_emdcb_params.modbus_baudrate = baudrate;
    g_emdcb_params.modbus_data_bits = data_bits;
    g_emdcb_params.modbus_stop_bits = stop_bits;
    g_emdcb_params.modbus_parity = parity;
    
    return 0;
}

// ==================== 系统参数函数 ====================

/**
 * @brief  读取系统参数
 * @param  station_id: 台位号输出
 * @param  timestamp: 时间戳输出
 * @param  work_hours: 工作小时数输出
 * @retval 无
 */
void emdcb_read_system_params(LONG *station_id, LONG *timestamp, LONG *work_hours) 
{
    if (station_id) *station_id = g_emdcb_params.station_id;
    if (timestamp) *timestamp = g_emdcb_params.timestamp;
    if (work_hours) *work_hours = g_emdcb_params.work_hours;
}

/**
 * @brief  设置系统参数
 * @param  station_id: 台位号
 * @param  timestamp: 时间戳
 * @retval 0: 成功, 1: 权限不足
 */
uint8_t emdcb_set_system_params(LONG station_id, LONG timestamp)
{
    if (!check_permission(PERM_LEVEL_03)) return 1;
    
    g_emdcb_params.station_id = station_id;
    g_emdcb_params.timestamp = timestamp;
    
    return 0;
}

// ==================== 显示板参数函数 ====================

/**
 * @brief  读取显示板参数
 * @param  language: 语言选择输出
 * @param  ble_id: BLE设备ID输出
 * @param  accumulator_display: 累积器显示输出
 * @retval 无
 */
void emdcb_read_display_params(SHORT *language, SHORT *ble_id, SHORT *accumulator_display)
{
    if (language) *language = g_emdcb_params.display_language;
    if (ble_id) *ble_id = g_emdcb_params.ble_device_id;
    if (accumulator_display) *accumulator_display = g_emdcb_params.accumulator_display;
}

/**
 * @brief  设置显示板参数
 * @param  language: 语言选择
 * @param  ble_id: BLE设备ID
 * @param  accumulator_display: 累积器显示
 * @retval 0: 成功, 1: 权限不足, 2: 参数无效
 */
uint8_t emdcb_set_display_params(SHORT language, SHORT ble_id, SHORT accumulator_display) {
    if (!check_permission(PERM_LEVEL_03)) return 1;
    if (language < 0 || language > 2) return 2;
    if (ble_id < 0 || ble_id > 0x0F) return 2;
    if (accumulator_display < 0 || accumulator_display > 1) return 2;
    
    g_emdcb_params.display_language = language;
    g_emdcb_params.ble_device_id = ble_id;
    g_emdcb_params.accumulator_display = accumulator_display;
    
    return 0;
}

// ==================== 明渠参数函数 ====================

/**
 * @brief  读取明渠参数
 * @param  channel_type: 明渠类别输出
 * @param  depth: 渠深输出
 * @param  top_width: 上底宽度输出
 * @param  bottom_width: 下底宽度输出
 * @param  param_c: 参数c输出
 * @retval 无
 */
void emdcb_read_channel_params(SHORT *channel_type, FLOAT *depth, FLOAT *top_width, 
                              FLOAT *bottom_width, FLOAT *param_c) {
    if (channel_type) *channel_type = g_emdcb_params.channel_type;
    if (depth) *depth = g_emdcb_params.channel_depth;
    if (top_width) *top_width = g_emdcb_params.channel_top_width;
    if (bottom_width) *bottom_width = g_emdcb_params.channel_bottom_width;
    if (param_c) *param_c = g_emdcb_params.channel_param_c;
}

/**
 * @brief  设置明渠参数
 * @param  channel_type: 明渠类别
 * @param  depth: 渠深
 * @param  top_width: 上底宽度
 * @param  bottom_width: 下底宽度
 * @param  param_c: 参数c
 * @retval 0: 成功, 1: 权限不足, 2: 参数无效
 */
uint8_t emdcb_set_channel_params(SHORT channel_type, FLOAT depth, FLOAT top_width, 
                                FLOAT bottom_width, FLOAT param_c) {
    if (!check_permission(PERM_LEVEL_01)) return 1;
    if (channel_type < 0 || channel_type > 5) return 2;
    if (depth <= 0) return 2;
    if (top_width <= 0) return 2;
    if (bottom_width < 0) return 2;
    
    g_emdcb_params.channel_type = channel_type;
    g_emdcb_params.channel_depth = depth;
    g_emdcb_params.channel_top_width = top_width;
    g_emdcb_params.channel_bottom_width = bottom_width;
    g_emdcb_params.channel_param_c = param_c;
    
    return 0;
}

// ==================== 校准参数函数 ====================

/**
 * @brief  读取校准参数
 * @param  cal_4ma: 4mA校准值输出
 * @param  cal_20ma: 20mA校准值输出
 * @retval 无
 */
void emdcb_read_calibration_params(FLOAT *cal_4ma, FLOAT *cal_20ma) {
    if (cal_4ma) *cal_4ma = g_emdcb_params.calibrate_4ma;
    if (cal_20ma) *cal_20ma = g_emdcb_params.calibrate_20ma;
}

/**
 * @brief  设置校准参数
 * @param  cal_4ma: 4mA校准值
 * @param  cal_20ma: 20mA校准值
 * @retval 0: 成功, 1: 权限不足, 2: 参数无效
 */
uint8_t emdcb_set_calibration_params(FLOAT cal_4ma, FLOAT cal_20ma) {
    if (!check_permission(PERM_LEVEL_03)) return 1;
    if (cal_4ma >= cal_20ma) return 2;
    
    g_emdcb_params.calibrate_4ma = cal_4ma;
    g_emdcb_params.calibrate_20ma = cal_20ma;
    
    return 0;
}



/**
 * @brief  初始化所有参数为默认值
 * @retval 无
 */
void emdcb_init_default_params(EMDCB_Params_t* params) 
{
  //  EMDCB_Params_t* params = &g_emdcb_params;
    
    memset(params, 0, sizeof(EMDCB_Params_t));
    
    // 设置默认值
    params->flow_unit = 5;  // 默认单位: m3/h
    params->meter_caliber = 100;  // 默认口径: 100mm
    params->meter_range = 100.0f;  // 默认量程: 100 m3/h

    //485通信
    params->modbus_addr = 1;             // Modbus地址1
    params->modbus_baudrate = 9600;//115200;      // 波特率9600
    params->modbus_data_bits = 8;  // 默认数据位: 8
    params->modbus_stop_bits = 1;  // 默认停止位: 1
    params->modbus_parity = 0;  // 默认校验: 无


    params->flow_filter_set = 1;  // 默认滤波: 启用
    params->measure_mode = 0;  // 默认测量模式: 周期测量
        
    //设置合理的默认值
    params->instant_flow = 0.0f;         //瞬时流量
    params->flow_unit = 5;              // m3/h
    params->meter_caliber = 100;         // 100mm口径
    params->meter_range = 100.0f;        // 100 m3/h量程

    
    
    params->flow_direction_set = 0;      // 正向流量
    params->flow_filter_set = 1;         // 滤波使能
    params->empty_pipe_alarm_en = 1;    // 空管报警使能
    params->measure_mode = 0;            // 周期测量模式
    
    //设置默认的校准测量参数
    calibration_init_default_params(params);

}

    FLOAT large_flow_actual;         // 地址49: 大流量点实际流量值
    FLOAT large_flow_meter;          // 地址51: 大流量点仪表测量值
    FLOAT small_flow_actual;         // 地址53: 小流量的实际流量值
    FLOAT small_flow_meter;          // 地址55: 小流量点仪表测量值
    FLOAT zero_point_meter;          // 地址57: 零点的仪表测量值

//modbus设置完成后，让配置参数，让其生效
void     emdcb_ModbusSetting(uint16_t reg_addr, uint16_t reg_count)
{
    int i;
    for(i = reg_addr ; i < reg_addr + reg_count;i++ )
    {
        
      switch (i)
      { // 零点的仪表测量值
        case REG_ADDR_ZERO_POINT_METER:
          Calibration_SetZeroPoint(g_emdcb_params.zero_point_meter); 
          break;
          // 大流量点实际流量值
        case REG_ADDR_LARGE_FLOW_ACTUAL:
          // 大流量点仪表测量值
        case REG_ADDR_LARGE_FLOW_METER:
          Calibration_SetLargeFlowPoint(g_emdcb_params.small_flow_meter,g_emdcb_params.small_flow_actual,\
                                     g_emdcb_params.large_flow_meter, g_emdcb_params.large_flow_actual);
          break;
          // 小流量的实际流量值
        case REG_ADDR_SMALL_FLOW_ACTUAL:    
          // 小流量点仪表测量值
        case REG_ADDR_SMALL_FLOW_METER:
          Calibration_SetSmallFlowPoint(g_emdcb_params.small_flow_meter, g_emdcb_params.small_flow_actual); 
          break;                    
      }

    }
}

// ==================== 测试用例函数 ====================

/**
 * @brief  测试所有参数读写功能
 * @retval 无
 */
void emdcb_test_all_functions(void) {
    printf("=== EMDCB 参数读写测试 ===\n");
    
    // 初始化参数
    emdcb_init_all_params();
    printf("1. 参数初始化完成\n");
    
    // 测试流量测量参数
    FLOAT flow, velocity, percentage;
    emdcb_read_flow_measurement(&flow, &velocity, &percentage);
    printf("2. 流量测量参数读取: 流量=%.2f, 流速=%.2f, 百分比=%.1f%%\n", 
           flow, velocity, percentage);
    
    // 测试设备参数设置
    uint8_t result = emdcb_set_device_params(150.0f, 150, 0);
    if (result == 0) {
        printf("3. 设备参数设置成功\n");
    } else {
        printf("3. 设备参数设置失败: 错误码=%d\n", result);
    }
    
    // 测试报警阈值设置
    result = emdcb_set_flow_thresholds(120.0f, 10.0f);
    if (result == 0) {
        printf("4. 流量报警阈值设置成功\n");
    } else {
        printf("4. 流量报警阈值设置失败: 错误码=%d\n", result);
    }
    
    // 测试通信参数设置
    result = emdcb_set_comm_params(2, 9600, 8, 1, 0);
    if (result == 0) {
        printf("5. 通信参数设置成功\n");
    } else {
        printf("5. 通信参数设置失败: 错误码=%d\n", result);
    }
    
    // 测试权限功能
    emdcb_set_permission(PERM_LEVEL_03);
    printf("6. 当前权限等级: %d\n", emdcb_get_permission());
    
    printf("=== 测试完成 ===\n");
}

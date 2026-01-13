
#include <string.h>
#include <stdio.h>
#include "Modbus_index.h"
#include "Modbus_Param.h"
#include "calibration.h"

extern EMDCB_Params_t g_emdcb_params;

const RegisterMap_t g_register_map[] = {
    // ==================== 流量测量寄存器 ====================
    {0, 2, PERM_READ_ONLY, &g_emdcb_params.instant_flow, "瞬时流量"},
    {2, 1, PERM_READ_WRITE, &g_emdcb_params.flow_unit, "瞬时流量单位"},
    {3, 2, PERM_LEVEL_03, &g_emdcb_params.positive_total_int, "正向总量整数位"},
    {5, 2, PERM_LEVEL_03, &g_emdcb_params.positive_total_frac, "正向总量小数位"},
    {7, 1, PERM_READ_WRITE, &g_emdcb_params.total_unit, "总量累积单位"},
    {8, 2, PERM_LEVEL_03, &g_emdcb_params.negative_total_int, "反向总量整数位"},
    {10, 2, PERM_LEVEL_03, &g_emdcb_params.negative_total_frac, "反向总量小数位"},
    {12, 1, PERM_LEVEL_01, &g_emdcb_params.pressure_zero, "压力零点值"},
    
    // ==================== 报警状态寄存器 ====================
    {13, 1, PERM_READ_ONLY, &g_emdcb_params.excitation_alarm, "励磁报警"},
    {14, 1, PERM_READ_ONLY, &g_emdcb_params.empty_pipe_alarm, "空管报警"},
    {15, 1, PERM_READ_ONLY, &g_emdcb_params.reverse_flow_alarm, "流量反向报警"},
    {16, 1, PERM_READ_ONLY, &g_emdcb_params.flow_high_alarm, "流量上限报警"},
    {17, 1, PERM_READ_ONLY, &g_emdcb_params.flow_low_alarm, "流量下限报警"},
    
    // ==================== 测量参数寄存器 ====================
    {18, 2, PERM_READ_ONLY, &g_emdcb_params.flow_velocity, "流速"},
    {20, 2, PERM_READ_ONLY, &g_emdcb_params.flow_percentage, "流量百分比"},
    
    // ==================== 设备参数寄存器 ====================
    {22, 2, PERM_LEVEL_01, &g_emdcb_params.meter_range, "仪表量程设置"},
    {24, 1, PERM_LEVEL_01, &g_emdcb_params.meter_caliber, "仪表口径"},
    {25, 1, PERM_LEVEL_01, &g_emdcb_params.flow_direction_set, "流量方向设置"},
    {26, 1, PERM_LEVEL_01, &g_emdcb_params.reverse_flow_alarm_en, "流量反向报警使能"},
    {27, 2, PERM_LEVEL_01, &g_emdcb_params.start_flow_velocity, "始动流速"},
    {29, 1, PERM_LEVEL_01, &g_emdcb_params.flow_filter_set, "流量滤波设置"},
    {30, 1, PERM_LEVEL_01, &g_emdcb_params.flow_limit_alarm_en, "流量超限报警使能"},
    {31, 2, PERM_LEVEL_01, &g_emdcb_params.flow_high_threshold, "流量上限报警阈值"},
    {33, 2, PERM_LEVEL_01, &g_emdcb_params.flow_low_threshold, "流量下限报警阈值"},
    {35, 1, PERM_LEVEL_01, &g_emdcb_params.empty_pipe_alarm_en, "空管报警使能"},
    {36, 2, PERM_LEVEL_01, &g_emdcb_params.empty_pipe_threshold, "空管报警阈值"},
    {38, 1, PERM_LEVEL_01, &g_emdcb_params.excitation_mode, "励磁方式选择"},
    
    // ==================== 时段设置寄存器 ====================
    {39, 1, PERM_LEVEL_02, &g_emdcb_params.period1_start_hour, "时段1起始小时数"},
    {40, 1, PERM_LEVEL_02, &g_emdcb_params.period2_start_hour, "时段2起始小时数"},
    {41, 1, PERM_LEVEL_02, &g_emdcb_params.period1_interval, "时段1间隔测量时间"},
    {42, 1, PERM_LEVEL_02, &g_emdcb_params.period2_interval, "时段2间隔测量时间"},
    {43, 1, PERM_READ_WRITE, &g_emdcb_params.measure_mode, "测量模式切换"},
    
    // ==================== 转换器参数寄存器 ====================
    {44, 2, PERM_LEVEL_02, &g_emdcb_params.fluid_density, "流体密度"},
    {46, 1, PERM_LEVEL_02, &g_emdcb_params.digital_input2_func, "数字输入2功能"},
    {47, 2, PERM_LEVEL_02, &g_emdcb_params.converter_factor, "转换器系数"},
    {49, 2, PERM_LEVEL_02, &g_emdcb_params.large_flow_actual, "大流量点实际流量值"},
    {51, 2, PERM_LEVEL_02, &g_emdcb_params.large_flow_meter, "大流量点仪表测量值"},
    {53, 2, PERM_LEVEL_02, &g_emdcb_params.small_flow_actual, "小流量的实际流量值"},
    {55, 2, PERM_LEVEL_02, &g_emdcb_params.small_flow_meter, "小流量点仪表测量值"},
    {57, 2, PERM_LEVEL_02, &g_emdcb_params.zero_point_meter, "零点的仪表测量值"},
    {59, 1, PERM_LEVEL_02, &g_emdcb_params.digital_output1_type, "数字输出1类型设置"},
    {60, 2, PERM_LEVEL_02, &g_emdcb_params.pulse_unit_equivalent, "脉冲单位当量"},
    {62, 1, PERM_LEVEL_02, &g_emdcb_params.pulse_measure_object, "脉冲测量对象"},
    {63, 2, PERM_LEVEL_02, &g_emdcb_params.freq_output_full_scale, "频率输出满量程"},
    {65, 2, PERM_LEVEL_02, &g_emdcb_params.freq_output_duty_cycle, "频率输出占空比"},
    
    // ==================== 电源参数寄存器 ====================
    {67, 1, PERM_READ_ONLY, &g_emdcb_params.battery_voltage, "电池电量"},
    {68, 1, PERM_READ_ONLY, &g_emdcb_params.battery_low_alarm, "电池低电量报警"},
    {69, 1, PERM_READ_ONLY, &g_emdcb_params.memory_fault_alarm, "存储器故障报警"},
    
    // ==================== 压力参数寄存器 ====================
    {70, 1, PERM_LEVEL_01, &g_emdcb_params.pressure_measure_en, "压力测量使能"},
    {71, 1, PERM_READ_WRITE, &g_emdcb_params.pressure_unit, "压力测量单位"},
    {72, 2, PERM_READ_ONLY, &g_emdcb_params.pressure_value, "压力测量数值"},
    {74, 2, PERM_LEVEL_01, &g_emdcb_params.pressure_high_threshold, "压力上限报警阈值"},
    {76, 2, PERM_LEVEL_01, &g_emdcb_params.pressure_low_threshold, "压力下限报警阈值"},
    {78, 1, PERM_READ_ONLY, &g_emdcb_params.pressure_high_alarm, "压力上限报警"},
    {79, 1, PERM_READ_ONLY, &g_emdcb_params.pressure_low_alarm, "压力下限报警"},
    
    // ==================== 温度参数寄存器 ====================
    {80, 1, PERM_LEVEL_01, &g_emdcb_params.temperature_measure_en, "温度测量使能"},
    {81, 2, PERM_READ_ONLY, &g_emdcb_params.temperature_value, "温度测量数值"},
    {83, 2, PERM_LEVEL_01, &g_emdcb_params.temperature_high_threshold, "温度上限报警阈值"},
    {85, 2, PERM_LEVEL_01, &g_emdcb_params.temperature_low_threshold, "温度下限报警阈值"},
    {87, 1, PERM_READ_ONLY, &g_emdcb_params.temperature_high_alarm, "温度上限报警"},
    {88, 1, PERM_READ_ONLY, &g_emdcb_params.temperature_low_alarm, "温度下限报警"},
    
    // ==================== 系统参数寄存器 ====================
    {89, 1, PERM_READ_ONLY, &g_emdcb_params.empty_pipe_value, "空管测量值"},
    {90, 2, PERM_READ_ONLY, &g_emdcb_params.work_hours, "工作总小时数"},
    {92, 2, PERM_READ_ONLY, &g_emdcb_params.remaining_quick_measure, "剩余快速测量次数"},
    {93, 2, PERM_READ_ONLY, &g_emdcb_params.converter_code, "转换器编码"},
    {95, 2, PERM_READ_ONLY, &g_emdcb_params.sensor_code, "传感器编码"},
    
    // ==================== 通信参数寄存器 ====================
    {97, 1, PERM_LEVEL_03, &g_emdcb_params.modbus_addr, "Modbus通讯地址"},
    {98, 1, PERM_LEVEL_03, &g_emdcb_params.modbus_type, "Modbus通讯类型"},
    {99, 1, PERM_LEVEL_03, &g_emdcb_params.modbus_data_bits, "Modbus通讯数据位"},
    {100, 1, PERM_LEVEL_03, &g_emdcb_params.modbus_parity, "Modbus通讯校验位"},
    {101, 1, PERM_LEVEL_03, &g_emdcb_params.modbus_stop_bits, "Modbus通讯停止位"},
    {102, 1, PERM_LEVEL_03, &g_emdcb_params.modbus_baudrate, "Modbus通讯波特率"},
    
    // ==================== 系统标识寄存器 ====================
    {103, 2, PERM_LEVEL_03, &g_emdcb_params.station_id, "台位号"},
    {105, 2, PERM_READ_WRITE, &g_emdcb_params.timestamp, "时间戳"},
    {107, 2, PERM_WRITE_ONLY, &g_emdcb_params.permission1_pwd, "权限1密码"},
    {109, 2, PERM_WRITE_ONLY, &g_emdcb_params.permission2_pwd, "权限2密码"},
    {111, 2, PERM_WRITE_ONLY, &g_emdcb_params.total_set_pwd, "累积量设置密码"},
    {113, 2, PERM_WRITE_ONLY, &g_emdcb_params.user_input_pwd, "用户输入密码"},
    
    // ==================== 显示板参数寄存器 ====================
    {115, 1, PERM_LEVEL_03, &g_emdcb_params.accumulator_display, "累积器显示"},
    {116, 1, PERM_LEVEL_03, &g_emdcb_params.parameter_lock, "参数锁"},
    {117, 1, PERM_WRITE_ONLY, &g_emdcb_params.restart_display, "重启显示通信板"},
    {118, 1, PERM_LEVEL_03, &g_emdcb_params.ble_device_id, "BLE设备编号的低4位数字"},
    {119, 1, PERM_LEVEL_03, &g_emdcb_params.display_language, "显示通信板语言选择"},
    {120, 1, PERM_LEVEL_03, &g_emdcb_params.status_output_func, "状态输出功能配置"},
    {121, 1, PERM_LEVEL_03, &g_emdcb_params.digital_pressure_sensor_type, "数字压力传感器类型"},
    {122, 1, PERM_LEVEL_03, &g_emdcb_params.software_license, "软件合法授权"},
    {123, 2, PERM_LEVEL_03, &g_emdcb_params.software_serial, "软件序列号"},
    {125, 1, PERM_LEVEL_03, &g_emdcb_params.oem_select, "OEM选择"},
    {126, 1, PERM_READ_ONLY, &g_emdcb_params.firmware_version, "仪表固件版本号"},
    {127, 2, PERM_READ_ONLY, &g_emdcb_params.factory_id_high, "出厂ID号高位"},
    {129, 2, PERM_READ_ONLY, &g_emdcb_params.factory_id_low, "出厂ID号低位"},
    {131, 1, PERM_LEVEL_03, &g_emdcb_params.slave_modbus_addr, "从机Modbus地址"},
    
    // ==================== 备份参数寄存器 ====================
    {132, 2, PERM_READ_ONLY, &g_emdcb_params.instant_flow_backup, "瞬时流量备份"},
    {134, 2, PERM_READ_ONLY, &g_emdcb_params.positive_total_int_backup, "正向总量整数位备份"},
    {136, 2, PERM_READ_ONLY, &g_emdcb_params.positive_total_frac_backup, "正向总量小数位备份"},
    {138, 2, PERM_READ_ONLY, &g_emdcb_params.negative_total_int_backup, "反向总量整数位备份"},
    {140, 2, PERM_READ_ONLY, &g_emdcb_params.negative_total_frac_backup, "反向总量小数位备份"},
    {142, 2, PERM_READ_ONLY, &g_emdcb_params.flow_velocity_backup, "流速备份"},
    {144, 2, PERM_READ_ONLY, &g_emdcb_params.pressure_backup, "压力备份"},
    {146, 1, PERM_READ_ONLY, &g_emdcb_params.meter_battery_voltage, "仪表电池电压"},
    {147, 2, PERM_READ_ONLY, &g_emdcb_params.status_word, "状态位"},
    {149, 2, PERM_READ_ONLY, &g_emdcb_params.temperature_backup, "温度备份"},
    {151, 1, PERM_READ_ONLY, &g_emdcb_params.measure_mode_backup, "测量模式备份"},
    {152, 1, PERM_READ_ONLY, &g_emdcb_params.empty_pipe_value_backup, "空管测量值备份"},
    {153, 2, PERM_READ_ONLY, &g_emdcb_params.timestamp_backup, "时间戳备份"},
    
    // ==================== 明渠参数寄存器 ====================
    {155, 2, PERM_READ_ONLY, &g_emdcb_params.liquid_level, "液位"},
    {157, 2, PERM_READ_ONLY, &g_emdcb_params.cross_section_area, "截面积"},
    {159, 1, PERM_RESERVED, &g_emdcb_params.reserved_159, "预留"},
    {160, 2, PERM_LEVEL_02, &g_emdcb_params.correct_point_a_actual, "修正点A实际流量值"},
    {162, 2, PERM_LEVEL_02, &g_emdcb_params.correct_point_a_meter, "修正点A仪表测量值"},
    {164, 2, PERM_LEVEL_02, &g_emdcb_params.correct_point_b_actual, "修正点B实际流量值"},
    {166, 2, PERM_LEVEL_02, &g_emdcb_params.correct_point_b_meter, "修正点B仪表测量值"},
    {168, 2, PERM_LEVEL_01, &g_emdcb_params.accumulator_alarm_threshold, "累积器报警阈值"},
    {170, 1, PERM_LEVEL_01, &g_emdcb_params.channel_type, "明渠类别"},
    {171, 2, PERM_LEVEL_01, &g_emdcb_params.channel_depth, "渠深h"},
    {173, 2, PERM_LEVEL_01, &g_emdcb_params.channel_top_width, "上底a"},
    {175, 2, PERM_LEVEL_01, &g_emdcb_params.channel_bottom_width, "下底b"},
    {177, 2, PERM_LEVEL_01, &g_emdcb_params.channel_param_c, "参数c"},
    {179, 1, PERM_RESERVED, &g_emdcb_params.reserved_179, "预留"},
    
    // ==================== 系统功能寄存器 ====================
    {180, 2, PERM_LEVEL_03, &g_emdcb_params.device_number, "装置号"},
    {182, 1, PERM_LEVEL_03, &g_emdcb_params.data_record_interval, "数据记录间隔时间"},
    {183, 1, PERM_LEVEL_03, &g_emdcb_params.upload_interval, "后台上送时间间隔"},
    {184, 1, PERM_LEVEL_03, &g_emdcb_params.test_email, "测试发送邮件"},
    {185, 1, PERM_READ_WRITE, &g_emdcb_params.increment_unit, "增量单位"},
    {186, 2, PERM_READ_WRITE, &g_emdcb_params.positive_increment, "正向增量"},
    {188, 2, PERM_READ_WRITE, &g_emdcb_params.negative_increment, "负向增量"},
    
    // ==================== 备份数据寄存器 ====================
    {190, 2, PERM_READ_ONLY, &g_emdcb_params.positive_total_int_backup1, "正向累计流量整数部分备份1"},
    {192, 2, PERM_READ_ONLY, &g_emdcb_params.positive_total_frac_backup1, "正向累计流量小数部分备份1"},
    {194, 2, PERM_READ_ONLY, &g_emdcb_params.positive_total_int_backup2, "正向累计流量整数部分备份2"},
    {196, 2, PERM_READ_ONLY, &g_emdcb_params.positive_total_frac_backup2, "正向累计流量小数部分备份2"},
    {198, 2, PERM_READ_ONLY, &g_emdcb_params.negative_total_int_backup1, "负向累计流量整数部分备份1"},
    {200, 2, PERM_READ_ONLY, &g_emdcb_params.negative_total_frac_backup1, "负向累计流量小数部分备份1"},
    {202, 2, PERM_READ_ONLY, &g_emdcb_params.negative_total_int_backup2, "负向累计流量整数部分备份2"},
    {204, 2, PERM_READ_ONLY, &g_emdcb_params.negative_total_frac_backup2, "负向累计流量小数部分备份2"},
    
    // ==================== 预留寄存器区域 ====================
    {206, 1, PERM_RESERVED, &g_emdcb_params.reserved_206, "预留"},
    {208, 1, PERM_RESERVED, &g_emdcb_params.reserved_208, "预留"},
    {210, 1, PERM_RESERVED, &g_emdcb_params.reserved_210, "预留"},
    {212, 1, PERM_RESERVED, &g_emdcb_params.reserved_212, "预留"},
    {214, 1, PERM_RESERVED, &g_emdcb_params.reserved_214, "预留"},
    {216, 1, PERM_RESERVED, &g_emdcb_params.reserved_216, "预留"},
    {218, 1, PERM_RESERVED, &g_emdcb_params.reserved_218, "预留"},
    {220, 1, PERM_RESERVED, &g_emdcb_params.reserved_220, "预留"},
    {222, 1, PERM_RESERVED, &g_emdcb_params.reserved_222, "预留"},
    {224, 1, PERM_RESERVED, &g_emdcb_params.reserved_224, "预留"},
    {226, 1, PERM_RESERVED, &g_emdcb_params.reserved_226, "预留"},
    {228, 1, PERM_RESERVED, &g_emdcb_params.reserved_228, "预留"},
    
    // ==================== 校准寄存器 ====================
    {230, 1, PERM_LEVEL_03, &g_emdcb_params.calibrate_4_20ma, "4-20mA校准"},
    {231, 2, PERM_LEVEL_03, &g_emdcb_params.calibrate_4ma, "4mA校准值"},
    {233, 2, PERM_LEVEL_03, &g_emdcb_params.calibrate_20ma, "20mA校准值"},
    {235, 1, PERM_READ_WRITE, &g_emdcb_params.current_output_polarity, "电流输出极性模式"},
    {236, 1, PERM_RESERVED, &g_emdcb_params.reserved_236, "预留"},
    {238, 1, PERM_RESERVED, &g_emdcb_params.reserved_238, "预留"},
    
    // ==================== 显示板信息寄存器 ====================
    {240, 2, PERM_READ_ONLY, &g_emdcb_params.display_factory_id_high, "显示板出厂ID号的高位"},
    {242, 2, PERM_READ_ONLY, &g_emdcb_params.display_factory_id_low, "显示板出厂ID号的低位"},
    {244, 1, PERM_READ_ONLY, &g_emdcb_params.display_software_license, "显示板软件合法授权"},
    {245, 2, PERM_READ_WRITE, &g_emdcb_params.display_software_serial, "显示板软件序列号"},
    {247, 1, PERM_RESERVED, &g_emdcb_params.reserved_247, "预留"},
    
    // 结束标记
    {0xFFFF, 0, PERM_READ_ONLY, NULL, "END_OF_TABLE"}
};

// 寄存器映射表大小
#define REGISTER_MAP_SIZE (sizeof(g_register_map) / sizeof(RegisterMap_t))

// 全局变量
EMDCB_Params_t g_emdcb_params;


static uint8_t current_permission = PERM_READ_ONLY;

// 索引到寄存器映射表的快速查找表
// 注意：这个数组的顺序必须与REGISTER_MAP_SIZE一致
static const RegisterMap_t* g_register_by_index[REG_TOTAL_COUNT] = {NULL};

//#define REGISTER_MAP_SIZE (sizeof(g_register_map) / sizeof(g_register_map[0]))

// 初始化快速查找表

void init_index_table(void) 
{
    // 遍历寄存器映射表，建立索引
    for (int i = 0; i < REGISTER_MAP_SIZE; i++) 
    {
        if (g_register_map[i].addr != 0xFFFF) 
        {  // 不是结束标记
            // 根据描述字符串确定索引
            const char* desc = g_register_map[i].desc;
            
            if (strcmp(desc, "瞬时流量") == 0) g_register_by_index[REG_INDEX_INSTANT_FLOW] = &g_register_map[i];
            else if (strcmp(desc, "瞬时流量单位") == 0) g_register_by_index[REG_INDEX_FLOW_UNIT] = &g_register_map[i];
            else if (strcmp(desc, "正向总量整数位") == 0) g_register_by_index[REG_INDEX_POSITIVE_TOTAL_INT] = &g_register_map[i];
            else if (strcmp(desc, "正向总量小数位") == 0) g_register_by_index[REG_INDEX_POSITIVE_TOTAL_FRAC] = &g_register_map[i];
            else if (strcmp(desc, "总量累积单位") == 0) g_register_by_index[REG_INDEX_TOTAL_UNIT] = &g_register_map[i];
            else if (strcmp(desc, "反向总量整数位") == 0) g_register_by_index[REG_INDEX_NEGATIVE_TOTAL_INT] = &g_register_map[i];
            else if (strcmp(desc, "反向总量小数位") == 0) g_register_by_index[REG_INDEX_NEGATIVE_TOTAL_FRAC] = &g_register_map[i];
            else if (strcmp(desc, "压力零点值") == 0) g_register_by_index[REG_INDEX_PRESSURE_ZERO] = &g_register_map[i];
            else if (strcmp(desc, "励磁报警") == 0) g_register_by_index[REG_INDEX_EXCITATION_ALARM] = &g_register_map[i];
            else if (strcmp(desc, "空管报警") == 0) g_register_by_index[REG_INDEX_EMPTY_PIPE_ALARM] = &g_register_map[i];
            else if (strcmp(desc, "流量反向报警") == 0) g_register_by_index[REG_INDEX_REVERSE_FLOW_ALARM] = &g_register_map[i];
            else if (strcmp(desc, "流量上限报警") == 0) g_register_by_index[REG_INDEX_FLOW_HIGH_ALARM] = &g_register_map[i];
            else if (strcmp(desc, "流量下限报警") == 0) g_register_by_index[REG_INDEX_FLOW_LOW_ALARM] = &g_register_map[i];
            else if (strcmp(desc, "流速") == 0) g_register_by_index[REG_INDEX_FLOW_VELOCITY] = &g_register_map[i];
            else if (strcmp(desc, "流量百分比") == 0) g_register_by_index[REG_INDEX_FLOW_PERCENTAGE] = &g_register_map[i];
            else if (strcmp(desc, "仪表量程设置") == 0) g_register_by_index[REG_INDEX_METER_RANGE] = &g_register_map[i];
            else if (strcmp(desc, "仪表口径") == 0) g_register_by_index[REG_INDEX_METER_CALIBER] = &g_register_map[i];
            else if (strcmp(desc, "流量方向设置") == 0) g_register_by_index[REG_INDEX_FLOW_DIRECTION_SET] = &g_register_map[i];
            else if (strcmp(desc, "流量反向报警使能") == 0) g_register_by_index[REG_INDEX_REVERSE_FLOW_ALARM_EN] = &g_register_map[i];
            else if (strcmp(desc, "始动流速") == 0) g_register_by_index[REG_INDEX_START_FLOW_VELOCITY] = &g_register_map[i];
            else if (strcmp(desc, "流量滤波设置") == 0) g_register_by_index[REG_INDEX_FLOW_FILTER_SET] = &g_register_map[i];
            else if (strcmp(desc, "流量超限报警使能") == 0) g_register_by_index[REG_INDEX_FLOW_LIMIT_ALARM_EN] = &g_register_map[i];
            else if (strcmp(desc, "流量上限报警阈值") == 0) g_register_by_index[REG_INDEX_FLOW_HIGH_THRESHOLD] = &g_register_map[i];
            else if (strcmp(desc, "流量下限报警阈值") == 0) g_register_by_index[REG_INDEX_FLOW_LOW_THRESHOLD] = &g_register_map[i];
            else if (strcmp(desc, "空管报警使能") == 0) g_register_by_index[REG_INDEX_EMPTY_PIPE_ALARM_EN] = &g_register_map[i];
            else if (strcmp(desc, "空管报警阈值") == 0) g_register_by_index[REG_INDEX_EMPTY_PIPE_THRESHOLD] = &g_register_map[i];
            else if (strcmp(desc, "励磁方式选择") == 0) g_register_by_index[REG_INDEX_EXCITATION_MODE] = &g_register_map[i];
            else if (strcmp(desc, "时段1起始小时数") == 0) g_register_by_index[REG_INDEX_PERIOD1_START_HOUR] = &g_register_map[i];
            else if (strcmp(desc, "时段2起始小时数") == 0) g_register_by_index[REG_INDEX_PERIOD2_START_HOUR] = &g_register_map[i];
            else if (strcmp(desc, "时段1间隔测量时间") == 0) g_register_by_index[REG_INDEX_PERIOD1_INTERVAL] = &g_register_map[i];
            else if (strcmp(desc, "时段2间隔测量时间") == 0) g_register_by_index[REG_INDEX_PERIOD2_INTERVAL] = &g_register_map[i];
            else if (strcmp(desc, "测量模式切换") == 0) g_register_by_index[REG_INDEX_MEASURE_MODE] = &g_register_map[i];
            else if (strcmp(desc, "流体密度") == 0) g_register_by_index[REG_INDEX_FLUID_DENSITY] = &g_register_map[i];
            else if (strcmp(desc, "数字输入2功能") == 0) g_register_by_index[REG_INDEX_DIGITAL_INPUT2_FUNC] = &g_register_map[i];
            else if (strcmp(desc, "转换器系数") == 0) g_register_by_index[REG_INDEX_CONVERTER_FACTOR] = &g_register_map[i];
            else if (strcmp(desc, "大流量点实际流量值") == 0) g_register_by_index[REG_INDEX_LARGE_FLOW_ACTUAL] = &g_register_map[i];
            else if (strcmp(desc, "大流量点仪表测量值") == 0) g_register_by_index[REG_INDEX_LARGE_FLOW_METER] = &g_register_map[i];
            else if (strcmp(desc, "小流量的实际流量值") == 0) g_register_by_index[REG_INDEX_SMALL_FLOW_ACTUAL] = &g_register_map[i];
            else if (strcmp(desc, "小流量点仪表测量值") == 0) g_register_by_index[REG_INDEX_SMALL_FLOW_METER] = &g_register_map[i];
            else if (strcmp(desc, "零点的仪表测量值") == 0) g_register_by_index[REG_INDEX_ZERO_POINT_METER] = &g_register_map[i];
            else if (strcmp(desc, "数字输出1类型设置") == 0) g_register_by_index[REG_INDEX_DIGITAL_OUTPUT1_TYPE] = &g_register_map[i];
            else if (strcmp(desc, "脉冲单位当量") == 0) g_register_by_index[REG_INDEX_PULSE_UNIT_EQUIVALENT] = &g_register_map[i];
            else if (strcmp(desc, "脉冲测量对象") == 0) g_register_by_index[REG_INDEX_PULSE_MEASURE_OBJECT] = &g_register_map[i];
            else if (strcmp(desc, "频率输出满量程") == 0) g_register_by_index[REG_INDEX_FREQ_OUTPUT_FULL_SCALE] = &g_register_map[i];
            else if (strcmp(desc, "频率输出占空比") == 0) g_register_by_index[REG_INDEX_FREQ_OUTPUT_DUTY_CYCLE] = &g_register_map[i];
            else if (strcmp(desc, "电池电量") == 0) g_register_by_index[REG_INDEX_BATTERY_VOLTAGE] = &g_register_map[i];
            else if (strcmp(desc, "电池低电量报警") == 0) g_register_by_index[REG_INDEX_BATTERY_LOW_ALARM] = &g_register_map[i];
            else if (strcmp(desc, "存储器故障报警") == 0) g_register_by_index[REG_INDEX_MEMORY_FAULT_ALARM] = &g_register_map[i];
            else if (strcmp(desc, "压力测量使能") == 0) g_register_by_index[REG_INDEX_PRESSURE_MEASURE_EN] = &g_register_map[i];
            else if (strcmp(desc, "压力测量单位") == 0) g_register_by_index[REG_INDEX_PRESSURE_UNIT] = &g_register_map[i];
            else if (strcmp(desc, "压力测量数值") == 0) g_register_by_index[REG_INDEX_PRESSURE_VALUE] = &g_register_map[i];
            else if (strcmp(desc, "压力上限报警阈值") == 0) g_register_by_index[REG_INDEX_PRESSURE_HIGH_THRESHOLD] = &g_register_map[i];
            else if (strcmp(desc, "压力下限报警阈值") == 0) g_register_by_index[REG_INDEX_PRESSURE_LOW_THRESHOLD] = &g_register_map[i];
            else if (strcmp(desc, "压力上限报警") == 0) g_register_by_index[REG_INDEX_PRESSURE_HIGH_ALARM] = &g_register_map[i];
            else if (strcmp(desc, "压力下限报警") == 0) g_register_by_index[REG_INDEX_PRESSURE_LOW_ALARM] = &g_register_map[i];
            else if (strcmp(desc, "温度测量使能") == 0) g_register_by_index[REG_INDEX_TEMPERATURE_MEASURE_EN] = &g_register_map[i];
            else if (strcmp(desc, "温度测量数值") == 0) g_register_by_index[REG_INDEX_TEMPERATURE_VALUE] = &g_register_map[i];
            else if (strcmp(desc, "温度上限报警阈值") == 0) g_register_by_index[REG_INDEX_TEMPERATURE_HIGH_THRESHOLD] = &g_register_map[i];
            else if (strcmp(desc, "温度下限报警阈值") == 0) g_register_by_index[REG_INDEX_TEMPERATURE_LOW_THRESHOLD] = &g_register_map[i];
            else if (strcmp(desc, "温度上限报警") == 0) g_register_by_index[REG_INDEX_TEMPERATURE_HIGH_ALARM] = &g_register_map[i];
            else if (strcmp(desc, "温度下限报警") == 0) g_register_by_index[REG_INDEX_TEMPERATURE_LOW_ALARM] = &g_register_map[i];
            else if (strcmp(desc, "空管测量值") == 0) g_register_by_index[REG_INDEX_EMPTY_PIPE_VALUE] = &g_register_map[i];
            else if (strcmp(desc, "工作总小时数") == 0) g_register_by_index[REG_INDEX_WORK_HOURS] = &g_register_map[i];
            else if (strcmp(desc, "剩余快速测量次数") == 0) g_register_by_index[REG_INDEX_REMAINING_QUICK_MEASURE] = &g_register_map[i];
            else if (strcmp(desc, "转换器编码") == 0) g_register_by_index[REG_INDEX_CONVERTER_CODE] = &g_register_map[i];
            else if (strcmp(desc, "传感器编码") == 0) g_register_by_index[REG_INDEX_SENSOR_CODE] = &g_register_map[i];
            else if (strcmp(desc, "Modbus通讯地址") == 0) g_register_by_index[REG_INDEX_MODBUS_ADDR] = &g_register_map[i];
            else if (strcmp(desc, "Modbus通讯类型") == 0) g_register_by_index[REG_INDEX_MODBUS_TYPE] = &g_register_map[i];
            else if (strcmp(desc, "Modbus通讯数据位") == 0) g_register_by_index[REG_INDEX_MODBUS_DATA_BITS] = &g_register_map[i];
            else if (strcmp(desc, "Modbus通讯校验位") == 0) g_register_by_index[REG_INDEX_MODBUS_PARITY] = &g_register_map[i];
            else if (strcmp(desc, "Modbus通讯停止位") == 0) g_register_by_index[REG_INDEX_MODBUS_STOP_BITS] = &g_register_map[i];
            else if (strcmp(desc, "Modbus通讯波特率") == 0) g_register_by_index[REG_INDEX_MODBUS_BAUDRATE] = &g_register_map[i];
            else if (strcmp(desc, "台位号") == 0) g_register_by_index[REG_INDEX_STATION_ID] = &g_register_map[i];
            else if (strcmp(desc, "时间戳") == 0) g_register_by_index[REG_INDEX_TIMESTAMP] = &g_register_map[i];
            else if (strcmp(desc, "权限1密码") == 0) g_register_by_index[REG_INDEX_PERMISSION1_PWD] = &g_register_map[i];
            else if (strcmp(desc, "权限2密码") == 0) g_register_by_index[REG_INDEX_PERMISSION2_PWD] = &g_register_map[i];
            else if (strcmp(desc, "累积量设置密码") == 0) g_register_by_index[REG_INDEX_TOTAL_SET_PWD] = &g_register_map[i];
            else if (strcmp(desc, "用户输入密码") == 0) g_register_by_index[REG_INDEX_USER_INPUT_PWD] = &g_register_map[i];
            else if (strcmp(desc, "累积器显示") == 0) g_register_by_index[REG_INDEX_ACCUMULATOR_DISPLAY] = &g_register_map[i];
            else if (strcmp(desc, "参数锁") == 0) g_register_by_index[REG_INDEX_PARAMETER_LOCK] = &g_register_map[i];
            else if (strcmp(desc, "重启显示通信板") == 0) g_register_by_index[REG_INDEX_RESTART_DISPLAY] = &g_register_map[i];
            else if (strcmp(desc, "BLE设备编号的低4位数字") == 0) g_register_by_index[REG_INDEX_BLE_DEVICE_ID] = &g_register_map[i];
            else if (strcmp(desc, "显示通信板语言选择") == 0) g_register_by_index[REG_INDEX_DISPLAY_LANGUAGE] = &g_register_map[i];
            else if (strcmp(desc, "状态输出功能配置") == 0) g_register_by_index[REG_INDEX_STATUS_OUTPUT_FUNC] = &g_register_map[i];
            else if (strcmp(desc, "数字压力传感器类型") == 0) g_register_by_index[REG_INDEX_DIGITAL_PRESSURE_SENSOR_TYPE] = &g_register_map[i];
            else if (strcmp(desc, "软件合法授权") == 0) g_register_by_index[REG_INDEX_SOFTWARE_LICENSE] = &g_register_map[i];
            else if (strcmp(desc, "软件序列号") == 0) g_register_by_index[REG_INDEX_SOFTWARE_SERIAL] = &g_register_map[i];
            else if (strcmp(desc, "OEM选择") == 0) g_register_by_index[REG_INDEX_OEM_SELECT] = &g_register_map[i];
            else if (strcmp(desc, "仪表固件版本号") == 0) g_register_by_index[REG_INDEX_FIRMWARE_VERSION] = &g_register_map[i];
            else if (strcmp(desc, "出厂ID号高位") == 0) g_register_by_index[REG_INDEX_FACTORY_ID_HIGH] = &g_register_map[i];
            else if (strcmp(desc, "出厂ID号低位") == 0) g_register_by_index[REG_INDEX_FACTORY_ID_LOW] = &g_register_map[i];
            else if (strcmp(desc, "从机Modbus地址") == 0) g_register_by_index[REG_INDEX_SLAVE_MODBUS_ADDR] = &g_register_map[i];
            else if (strcmp(desc, "瞬时流量备份") == 0) g_register_by_index[REG_INDEX_INSTANT_FLOW_BACKUP] = &g_register_map[i];
            else if (strcmp(desc, "正向总量整数位备份") == 0) g_register_by_index[REG_INDEX_POSITIVE_TOTAL_INT_BACKUP] = &g_register_map[i];
            else if (strcmp(desc, "正向总量小数位备份") == 0) g_register_by_index[REG_INDEX_POSITIVE_TOTAL_FRAC_BACKUP] = &g_register_map[i];
            else if (strcmp(desc, "反向总量整数位备份") == 0) g_register_by_index[REG_INDEX_NEGATIVE_TOTAL_INT_BACKUP] = &g_register_map[i];
            else if (strcmp(desc, "反向总量小数位备份") == 0) g_register_by_index[REG_INDEX_NEGATIVE_TOTAL_FRAC_BACKUP] = &g_register_map[i];
            else if (strcmp(desc, "流速备份") == 0) g_register_by_index[REG_INDEX_FLOW_VELOCITY_BACKUP] = &g_register_map[i];
            else if (strcmp(desc, "压力备份") == 0) g_register_by_index[REG_INDEX_PRESSURE_BACKUP] = &g_register_map[i];
            else if (strcmp(desc, "仪表电池电压") == 0) g_register_by_index[REG_INDEX_METER_BATTERY_VOLTAGE] = &g_register_map[i];
            else if (strcmp(desc, "状态位") == 0) g_register_by_index[REG_INDEX_STATUS_WORD] = &g_register_map[i];
            else if (strcmp(desc, "温度备份") == 0) g_register_by_index[REG_INDEX_TEMPERATURE_BACKUP] = &g_register_map[i];
            else if (strcmp(desc, "测量模式备份") == 0) g_register_by_index[REG_INDEX_MEASURE_MODE_BACKUP] = &g_register_map[i];
            else if (strcmp(desc, "空管测量值备份") == 0) g_register_by_index[REG_INDEX_EMPTY_PIPE_VALUE_BACKUP] = &g_register_map[i];
            else if (strcmp(desc, "时间戳备份") == 0) g_register_by_index[REG_INDEX_TIMESTAMP_BACKUP] = &g_register_map[i];
            else if (strcmp(desc, "液位") == 0) g_register_by_index[REG_INDEX_LIQUID_LEVEL] = &g_register_map[i];
            else if (strcmp(desc, "截面积") == 0) g_register_by_index[REG_INDEX_CROSS_SECTION_AREA] = &g_register_map[i];
            else if (strcmp(desc, "修正点A实际流量值") == 0) g_register_by_index[REG_INDEX_CORRECT_POINT_A_ACTUAL] = &g_register_map[i];
            else if (strcmp(desc, "修正点A仪表测量值") == 0) g_register_by_index[REG_INDEX_CORRECT_POINT_A_METER] = &g_register_map[i];
            else if (strcmp(desc, "修正点B实际流量值") == 0) g_register_by_index[REG_INDEX_CORRECT_POINT_B_ACTUAL] = &g_register_map[i];
            else if (strcmp(desc, "修正点B仪表测量值") == 0) g_register_by_index[REG_INDEX_CORRECT_POINT_B_METER] = &g_register_map[i];
            else if (strcmp(desc, "累积器报警阈值") == 0) g_register_by_index[REG_INDEX_ACCUMULATOR_ALARM_THRESHOLD] = &g_register_map[i];
            else if (strcmp(desc, "明渠类别") == 0) g_register_by_index[REG_INDEX_CHANNEL_TYPE] = &g_register_map[i];
            else if (strcmp(desc, "渠深h") == 0) g_register_by_index[REG_INDEX_CHANNEL_DEPTH] = &g_register_map[i];
            else if (strcmp(desc, "上底a") == 0) g_register_by_index[REG_INDEX_CHANNEL_TOP_WIDTH] = &g_register_map[i];
            else if (strcmp(desc, "下底b") == 0) g_register_by_index[REG_INDEX_CHANNEL_BOTTOM_WIDTH] = &g_register_map[i];
            else if (strcmp(desc, "参数c") == 0) g_register_by_index[REG_INDEX_CHANNEL_PARAM_C] = &g_register_map[i];
            else if (strcmp(desc, "装置号") == 0) g_register_by_index[REG_INDEX_DEVICE_NUMBER] = &g_register_map[i];
            else if (strcmp(desc, "数据记录间隔时间") == 0) g_register_by_index[REG_INDEX_DATA_RECORD_INTERVAL] = &g_register_map[i];
            else if (strcmp(desc, "后台上送时间间隔") == 0) g_register_by_index[REG_INDEX_UPLOAD_INTERVAL] = &g_register_map[i];
            else if (strcmp(desc, "测试发送邮件") == 0) g_register_by_index[REG_INDEX_TEST_EMAIL] = &g_register_map[i];
            else if (strcmp(desc, "增量单位") == 0) g_register_by_index[REG_INDEX_INCREMENT_UNIT] = &g_register_map[i];
            else if (strcmp(desc, "正向增量") == 0) g_register_by_index[REG_INDEX_POSITIVE_INCREMENT] = &g_register_map[i];
            else if (strcmp(desc, "负向增量") == 0) g_register_by_index[REG_INDEX_NEGATIVE_INCREMENT] = &g_register_map[i];
            else if (strcmp(desc, "正向累计流量整数部分备份1") == 0) g_register_by_index[REG_INDEX_POSITIVE_TOTAL_INT_BACKUP1] = &g_register_map[i];
            else if (strcmp(desc, "正向累计流量小数部分备份1") == 0) g_register_by_index[REG_INDEX_POSITIVE_TOTAL_FRAC_BACKUP1] = &g_register_map[i];
            else if (strcmp(desc, "正向累计流量整数部分备份2") == 0) g_register_by_index[REG_INDEX_POSITIVE_TOTAL_INT_BACKUP2] = &g_register_map[i];
            else if (strcmp(desc, "正向累计流量小数部分备份2") == 0) g_register_by_index[REG_INDEX_POSITIVE_TOTAL_FRAC_BACKUP2] = &g_register_map[i];
            else if (strcmp(desc, "负向累计流量整数部分备份1") == 0) g_register_by_index[REG_INDEX_NEGATIVE_TOTAL_INT_BACKUP1] = &g_register_map[i];
            else if (strcmp(desc, "负向累计流量小数部分备份1") == 0) g_register_by_index[REG_INDEX_NEGATIVE_TOTAL_FRAC_BACKUP1] = &g_register_map[i];
            else if (strcmp(desc, "负向累计流量整数部分备份2") == 0) g_register_by_index[REG_INDEX_NEGATIVE_TOTAL_INT_BACKUP2] = &g_register_map[i];
            else if (strcmp(desc, "负向累计流量小数部分备份2") == 0) g_register_by_index[REG_INDEX_NEGATIVE_TOTAL_FRAC_BACKUP2] = &g_register_map[i];
            else if (strcmp(desc, "4-20mA校准") == 0) g_register_by_index[REG_INDEX_CALIBRATE_4_20MA] = &g_register_map[i];
            else if (strcmp(desc, "4mA校准值") == 0) g_register_by_index[REG_INDEX_CALIBRATE_4MA] = &g_register_map[i];
            else if (strcmp(desc, "20mA校准值") == 0) g_register_by_index[REG_INDEX_CALIBRATE_20MA] = &g_register_map[i];
            else if (strcmp(desc, "电流输出极性模式") == 0) g_register_by_index[REG_INDEX_CURRENT_OUTPUT_POLARITY] = &g_register_map[i];
            else if (strcmp(desc, "显示板出厂ID号的高位") == 0) g_register_by_index[REG_INDEX_DISPLAY_FACTORY_ID_HIGH] = &g_register_map[i];
            else if (strcmp(desc, "显示板出厂ID号的低位") == 0) g_register_by_index[REG_INDEX_DISPLAY_FACTORY_ID_LOW] = &g_register_map[i];
            else if (strcmp(desc, "显示板软件合法授权") == 0) g_register_by_index[REG_INDEX_DISPLAY_SOFTWARE_LICENSE] = &g_register_map[i];
            else if (strcmp(desc, "显示板软件序列号") == 0) g_register_by_index[REG_INDEX_DISPLAY_SOFTWARE_SERIAL] = &g_register_map[i];
        }
    }
}


// ==================== 辅助函数 ====================

/**
 * @brief  检查当前权限是否足够
 * @param  required_perm: 需要的权限等级
 * @retval 1: 权限足够, 0: 权限不足
 */
uint8_t check_permission(RegisterPermission required_perm) {
    return (current_permission >= required_perm);
}

/**
 * @brief  设置当前权限等级
 * @param  perm: 新的权限等级
 * @retval 无
 */
void emdcb_set_permission(uint8_t perm) 
{
    if (perm <= PERM_LEVEL_03) 
    {
        current_permission = perm;
    }
}

/**
 * @brief  获取当前权限等级
 * @retval 当前权限等级
 */
uint8_t emdcb_get_permission(void) {
    return current_permission;
}

// ==================== 通过索引查找寄存器信息 ====================

/**
 * @brief  通过索引获取寄存器映射项
 * @param  index: 寄存器索引
 * @retval 寄存器映射项指针，NULL表示未找到
 */
const RegisterMap_t* emdcb_get_register_by_index(uint16_t index) 
{
    if (index >= REG_TOTAL_COUNT) 
    {
        return NULL;
    }
    return g_register_by_index[index];
}

/**
 * @brief  通过索引获取寄存器地址
 * @param  index: 寄存器索引
 * @retval 寄存器地址，0xFFFF表示未找到
 */
uint16_t emdcb_get_addr_by_index(uint16_t index) {
    const RegisterMap_t* reg = emdcb_get_register_by_index(index);
    if (reg != NULL) {
        return reg->addr;
    }
    return 0xFFFF;
}

/**
 * @brief  通过索引获取寄存器描述
 * @param  index: 寄存器索引
 * @retval 寄存器描述字符串，NULL表示未找到
 */
const char* emdcb_get_desc_by_index(uint16_t index) {
    const RegisterMap_t* reg = emdcb_get_register_by_index(index);
    if (reg != NULL) {
        return reg->desc;
    }
    return NULL;
}

// ==================== 通过索引读取寄存器参数 ====================
// 字节序转换函数
uint16_t swap_uint16(uint16_t value) 
{
   // return (value << 8) | (value >> 8);
    return value;
}

uint32_t swap_uint32(uint32_t value) 
{
   /* return ((value << 24) & 0xFF000000) |
           ((value << 8)  & 0x00FF0000) |
           ((value >> 8)  & 0x0000FF00) |
           ((value >> 24) & 0x000000FF);*/
  return value;
}

float swap_float(float value) 
{
   /*union 
    {
        float f;
        uint32_t i;
    } u;
    u.f = value;
    u.i = swap_uint32(u.i);
    return u.f;*/
  return value;
}


/**
 * @brief  通过索引读取SHORT类型寄存器参数
 * @param  index: 寄存器索引
 * @param  value: SHORT值输出指针
 * @retval 0: 成功, 1: 寄存器未找到, 2: 类型不匹配
 */
uint8_t emdcb_get_short_by_index(uint16_t index, SHORT* value) 
{
    const RegisterMap_t* reg = emdcb_get_register_by_index(index);
    if (reg == NULL) {
        return 1;  // 寄存器未找到
    }
    
    if (reg->size != 1) {
        return 2;  // 类型不匹配
    }
    
    if (value != NULL) 
    {
        SHORT* source = (SHORT*)reg->data_ptr;
        *value = *source;
    }
    
    return 0;
}

/**
 * @brief  通过索引读取LONG类型寄存器参数
 * @param  index: 寄存器索引
 * @param  value: LONG值输出指针
 * @retval 0: 成功, 1: 寄存器未找到, 2: 类型不匹配
 */
uint8_t emdcb_get_long_by_index(uint16_t index, LONG* value) 
{
    const RegisterMap_t* reg = emdcb_get_register_by_index(index);
    if (reg == NULL) {
        return 1;  // 寄存器未找到
    }
    
    if (reg->size != 2) {
        return 2;  // 类型不匹配
    }
    
    if (value != NULL) 
    {
        LONG* data_ptr = (LONG*)reg->data_ptr;
        *value = swap_uint32(*data_ptr);
                
       // LONG* source = (LONG*)reg->data_ptr;
        //*value = *source;
    }
    
    return 0;
}

/**
 * @brief  通过索引读取FLOAT类型寄存器参数
 * @param  index: 寄存器索引
 * @param  value: FLOAT值输出指针
 * @retval 0: 成功, 1: 寄存器未找到, 2: 类型不匹配
 */
uint8_t emdcb_get_float_by_index(uint16_t index, FLOAT* value) 
{
    const RegisterMap_t* reg = emdcb_get_register_by_index(index);
    if (reg == NULL) {
        return 1;  // 寄存器未找到
    }
    
    if (reg->size != 2) {
        return 2;  // 类型不匹配
    }
    
    if (value != NULL) 
    {
        FLOAT* data_ptr = (FLOAT*)reg->data_ptr;
        FLOAT swapped_val = swap_float(*data_ptr);
        *value = swapped_val;
        //uint32_t int_val = *((uint32_t*)&swapped_val);
        //return (int_val >> 16) & 0xFFFF;
       /*         
        FLOAT* source = (FLOAT*)reg->data_ptr;
        *value = *source;*/
    }
    
    return 0;
}

/**
 * @brief  通用读取函数（通过索引）
 * @param  index: 寄存器索引
 * @retval 读取到的浮点数值，0表示读取失败
 */
FLOAT emdcb_get_param_by_index(uint16_t index) 
{
    const RegisterMap_t* reg = emdcb_get_register_by_index(index);
    if (reg == NULL) {
        return 0.0f;  // 寄存器未找到
    }
    
    if (reg->size == 1) 
    {
        // SHORT类型
        SHORT* source = (SHORT*)reg->data_ptr;
        return (FLOAT)(*source);
    } else if (reg->size == 2) 
    {
        // 假设是FLOAT类型
        FLOAT* data_ptr = (FLOAT*)reg->data_ptr;
        FLOAT swapped_val = swap_float(*data_ptr);
       // *source = swapped_val;
        
        return swapped_val;
    }
    
    return 0.0f;  // 类型不匹配
}

// ==================== 通过索引设置寄存器参数 ====================

/**
 * @brief  通过索引设置SHORT类型寄存器参数
 * @param  index: 寄存器索引
 * @param  value: 要设置的SHORT值
 * @retval 0: 成功, 1: 寄存器未找到, 2: 权限不足, 3: 类型不匹配
 */
uint8_t emdcb_set_short_by_index(uint16_t index, SHORT value)
{
    const RegisterMap_t* reg = emdcb_get_register_by_index(index);
    if (reg == NULL) {
        return 1;  // 寄存器未找到
    }
    
  /*  if (!check_permission(reg->permission)) {
        return 2;  // 权限不足
    }*/
    
    if (reg->size != 1) {
        return 3;  // 类型不匹配
    }
    
    SHORT* target = (SHORT*)reg->data_ptr;
    *target = value;
    
    return 0;
}

/**
 * @brief  通过索引设置LONG类型寄存器参数
 * @param  index: 寄存器索引
 * @param  value: 要设置的LONG值
 * @retval 0: 成功, 1: 寄存器未找到, 2: 权限不足, 3: 类型不匹配
 */
uint8_t emdcb_set_long_by_index(uint16_t index, LONG value) 
{
    const RegisterMap_t* reg = emdcb_get_register_by_index(index);
    if (reg == NULL) {
        return 1;  // 寄存器未找到
    }
    
 /*   if (!check_permission(reg->permission)) {
        return 2;  // 权限不足
    }*/
    
    if (reg->size != 2) {
        return 3;  // 类型不匹配
    }
    
    /*LONG* target = (LONG*)reg->data_ptr;
    LONG current_val = *target;
    *target = swap_uint32(current_val);
     */       
    LONG* target = (LONG*)reg->data_ptr;
    *target = value;
    
    return 0;
}

/**
 * @brief  通过索引设置FLOAT类型寄存器参数
 * @param  index: 寄存器索引
 * @param  value: 要设置的FLOAT值
 * @retval 0: 成功, 1: 寄存器未找到, 2: 权限不足, 3: 类型不匹配
 */
uint8_t emdcb_set_float_by_index(uint16_t index, FLOAT value) 
{
    const RegisterMap_t* reg = emdcb_get_register_by_index(index);
    if (reg == NULL) 
    {
        return 1;  // 寄存器未找到
    }
    
  /*  if (!check_permission(reg->permission)) 
    {
        return 2;  // 权限不足
    }*/
    
    if (reg->size != 2) 
    {
        return 3;  // 类型不匹配
    }
    
        // 32位数据，
          // 假设是FLOAT
            FLOAT* data_ptr = (FLOAT*)reg->data_ptr;
            //FLOAT current_val = *data_ptr;
           // FLOAT swapped_val = swap_float(current_val);
            //uint32_t int_val = *((uint32_t*)&swapped_val);
            
            //if (offset == 0) 
            {
                // 修改高16位
           //     int_val = (value << 16) | (int_val & 0xFFFF);
            } 
            //else if (offset == 1) 
            {
                // 修改低16位
           //     int_val = (int_val & 0xFFFF0000) | value;
            }
            
           // FLOAT new_swapped = swap_float(value);
           // FLOAT new_val = *((FLOAT*)&new_swapped);
            *data_ptr = value;
            return 1;  // 写入成功

    return 0;
}

/**
 * @brief  通用设置函数（通过索引）
 * @param  index: 寄存器索引
 * @param  value: 要设置的浮点数值
 * @retval 0: 成功, 1: 寄存器未找到, 2: 权限不足, 3: 类型不匹配
 */
uint8_t emdcb_set_param_by_index(uint16_t index, FLOAT value) 
{
    const RegisterMap_t* reg = emdcb_get_register_by_index(index);
    if (reg == NULL) 
    {
        return 1;  // 寄存器未找到
    }
    
    if (!check_permission(reg->permission)) {
        return 2;  // 权限不足
    }
    
    if (reg->size == 1)
    {
        // SHORT类型
        SHORT* target = (SHORT*)reg->data_ptr;
        *target = (SHORT)value;
    } 
    else if (reg->size == 2)
    {
        // 假设是FLOAT类型
        FLOAT* target = (FLOAT*)reg->data_ptr;
        *target = value;
    } else {
        return 3;  // 类型不匹配
    }
    
    return 0;
}

// ==================== 批量操作函数 ====================

/**
 * @brief  通过索引数组批量读取寄存器参数
 * @param  indices: 索引数组
 * @param  count: 索引数量
 * @param  values: 值输出数组
 * @retval 成功读取的参数数量
 */
uint16_t emdcb_batch_get_by_indices(uint16_t* indices, uint16_t count, FLOAT* values) 
{
    uint16_t success_count = 0;
    
    for (uint16_t i = 0; i < count; i++) 
    {
        FLOAT value = emdcb_get_param_by_index(indices[i]);
        if (value != 0.0f || indices[i] == REG_INDEX_INSTANT_FLOW) 
        {
            // 值为0可能是有效值，所以我们检查是否是瞬时流量（可能为0）
            values[i] = value;
            success_count++;
        }
    }
    
    return success_count;
}

/**
 * @brief  通过索引数组批量设置寄存器参数
 * @param  indices: 索引数组
 * @param  count: 索引数量
 * @param  values: 值输入数组
 * @retval 成功设置的参数数量
 */
uint16_t emdcb_batch_set_by_indices(uint16_t* indices, uint16_t count, FLOAT* values) {
    uint16_t success_count = 0;
    
    for (uint16_t i = 0; i < count; i++) {
        uint8_t result = emdcb_set_param_by_index(indices[i], values[i]);
        if (result == 0) {
            success_count++;
        }
    }
    
    return success_count;
}

// ==================== 初始化函数 ====================





// ==================== 测试函数 ====================

/**
 * @brief  测试通过索引设置和读取参数
 * @retval 无
 */
void emdcb_test_index_access(void) {
    printf("=== 测试通过索引设置和读取参数 ===\n");
    
    // 1. 初始化参数
    emdcb_init_all_params();
    printf("1. 参数初始化完成\n");
    
    // 2. 设置权限
    emdcb_set_permission(PERM_LEVEL_03);
    printf("2. 设置权限等级为: %d\n", emdcb_get_permission());
    
    // 3. 测试通过索引设置参数
    printf("3. 测试通过索引设置参数:\n");
    
    // 设置瞬时流量
    uint8_t result = emdcb_set_float_by_index(REG_INDEX_INSTANT_FLOW, 25.5f);
    if (result == 0) {
        printf("   - 设置瞬时流量成功: 25.5\n");
    } else {
        printf("   - 设置瞬时流量失败: 错误码=%d\n", result);
    }
    
    // 设置仪表量程
    result = emdcb_set_float_by_index(REG_INDEX_METER_RANGE, 150.0f);
    if (result == 0) {
        printf("   - 设置仪表量程成功: 150.0\n");
    } else {
        printf("   - 设置仪表量程失败: 错误码=%d\n", result);
    }
    
    // 设置Modbus地址
    result = emdcb_set_short_by_index(REG_INDEX_MODBUS_ADDR, 2);
    if (result == 0) {
        printf("   - 设置Modbus地址成功: 2\n");
    } else {
        printf("   - 设置Modbus地址失败: 错误码=%d\n", result);
    }
    
    // 4. 测试通过索引读取参数
    printf("4. 测试通过索引读取参数:\n");
    
    // 读取瞬时流量
    FLOAT flow = emdcb_get_param_by_index(REG_INDEX_INSTANT_FLOW);
    printf("   - 读取瞬时流量: %.2f\n", flow);
    
    // 读取仪表口径
    SHORT caliber;
    result = emdcb_get_short_by_index(REG_INDEX_METER_CALIBER, &caliber);
    if (result == 0) {
        printf("   - 读取仪表口径: %d\n", caliber);
    } else {
        printf("   - 读取仪表口径失败: 错误码=%d\n", result);
    }
    
    // 读取Modbus地址
    SHORT addr;
    result = emdcb_get_short_by_index(REG_INDEX_MODBUS_ADDR, &addr);
    if (result == 0) {
        printf("   - 读取Modbus地址: %d\n", addr);
    } else {
        printf("   - 读取Modbus地址失败: 错误码=%d\n", result);
    }
    
    // 5. 测试批量操作
    printf("5. 测试批量操作:\n");
    
    uint16_t batch_indices[] = {
        REG_INDEX_INSTANT_FLOW,
        REG_INDEX_METER_RANGE, 
        REG_INDEX_MODBUS_ADDR,
        REG_INDEX_METER_CALIBER
    };
    
    FLOAT batch_values[] = {30.0f, 200.0f, 3.0f, 200.0f};
    
    uint16_t set_count = emdcb_batch_set_by_indices(batch_indices, 4, batch_values);
    printf("   - 批量设置成功数量: %d\n", set_count);
    
    FLOAT read_values[4];
    uint16_t get_count = emdcb_batch_get_by_indices(batch_indices, 4, read_values);
    printf("   - 批量读取成功数量: %d\n", get_count);
    
    for (uint16_t i = 0; i < get_count; i++) {
        const char* desc = emdcb_get_desc_by_index(batch_indices[i]);
        printf("   - [索引%d] %s: %.2f\n", batch_indices[i], desc, read_values[i]);
    }
    
    // 6. 测试查找地址
    printf("6. 测试通过索引查找地址:\n");
    uint16_t addr1 = emdcb_get_addr_by_index(REG_INDEX_INSTANT_FLOW);
    uint16_t addr2 = emdcb_get_addr_by_index(REG_INDEX_METER_RANGE);
    printf("   - 瞬时流量地址: %d (0x%04X)\n", addr1, addr1);
    printf("   - 仪表量程设置地址: %d (0x%04X)\n", addr2, addr2);
    
    // 7. 测试权限控制
    printf("7. 测试权限控制:\n");
    emdcb_set_permission(PERM_READ_ONLY);
    result = emdcb_set_float_by_index(REG_INDEX_INSTANT_FLOW, 50.0f);
    if (result == 2) {
        printf("   - 权限不足测试通过 (返回错误码2)\n");
    } else {
        printf("   - 权限不足测试失败 (返回错误码%d)\n", result);
    }
    
    printf("=== 测试完成 ===\n");
}

/**
 * @brief  主测试函数
 * @retval 无
 */
void emdcb_main_test_index(void) {
    printf("\n电磁流量计参数访问系统测试（索引版本）\n");
    printf("==========================================\n\n");
    
    emdcb_test_index_access();
    
    printf("\n所有测试完成！\n");
}

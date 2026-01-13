#ifndef EMDCB_MODBUS_PARAMS_H
#define EMDCB_MODBUS_PARAMS_H

#include <stdint.h>
#include "Modbus_index.h"




// ==================== 电磁流量计参数结构体 ====================
// 注意：此结构体包含所有电磁流量计的参数
typedef struct {
    // ==================== 实时测量参数 ====================
    FLOAT instant_flow;              // 地址0: 瞬时流量
    SHORT flow_unit;                 // 地址2: 瞬时流量单位
    LONG positive_total_int;         // 地址3: 正向总量整数位
    FLOAT positive_total_frac;      // 地址5: 正向总量小数位
    SHORT total_unit;                // 地址7: 总量累积单位
    LONG negative_total_int;         // 地址8: 反向总量整数位
    FLOAT negative_total_frac;       // 地址10: 反向总量小数位
    SHORT pressure_zero;             // 地址12: 压力零点值
    
    // ==================== 报警状态参数 ====================
    SHORT excitation_alarm;          // 地址13: 励磁报警
    SHORT empty_pipe_alarm;          // 地址14: 空管报警
    SHORT reverse_flow_alarm;        // 地址15: 流量反向报警
    SHORT flow_high_alarm;           // 地址16: 流量上限报警
    SHORT flow_low_alarm;            // 地址17: 流量下限报警
    
    // ==================== 测量参数 ====================
    FLOAT flow_velocity;             // 地址18: 流速
    FLOAT flow_percentage;           // 地址20: 流量百分比
    
    // ==================== 设备参数 ====================
    FLOAT meter_range;               // 地址22: 仪表量程设置
    SHORT meter_caliber;             // 地址24: 仪表口径
    SHORT flow_direction_set;        // 地址25: 流量方向设置
    SHORT reverse_flow_alarm_en;     // 地址26: 流量反向报警使能
    FLOAT start_flow_velocity;       // 地址27: 始动流速
    SHORT flow_filter_set;           // 地址29: 流量滤波设置
    SHORT flow_limit_alarm_en;       // 地址30: 流量超限报警使能
    FLOAT flow_high_threshold;       // 地址31: 流量上限报警阈值
    FLOAT flow_low_threshold;        // 地址33: 流量下限报警阈值
    SHORT empty_pipe_alarm_en;       // 地址35: 空管报警使能
    FLOAT empty_pipe_threshold;      // 地址36: 空管报警阈值
    SHORT excitation_mode;           // 地址38: 励磁方式选择
    
    // ==================== 时段设置参数 ====================
    SHORT period1_start_hour;        // 地址39: 时段1起始小时数
    SHORT period2_start_hour;        // 地址40: 时段2起始小时数
    SHORT period1_interval;          // 地址41: 时段1间隔测量时间
    SHORT period2_interval;          // 地址42: 时段2间隔测量时间
    SHORT measure_mode;              // 地址43: 测量模式切换
    
    // ==================== 转换器参数 ====================
    FLOAT fluid_density;             // 地址44: 流体密度
    SHORT digital_input2_func;       // 地址46: 数字输入2功能
    FLOAT converter_factor;          // 地址47: 转换器系数
    FLOAT large_flow_actual;         // 地址49: 大流量点实际流量值
    FLOAT large_flow_meter;          // 地址51: 大流量点仪表测量值
    FLOAT small_flow_actual;         // 地址53: 小流量的实际流量值
    FLOAT small_flow_meter;          // 地址55: 小流量点仪表测量值
    FLOAT zero_point_meter;          // 地址57: 零点的仪表测量值
    SHORT digital_output1_type;      // 地址59: 数字输出1类型设置
    FLOAT pulse_unit_equivalent;     // 地址60: 脉冲单位当量
    SHORT pulse_measure_object;      // 地址62: 脉冲测量对象
    FLOAT freq_output_full_scale;    // 地址63: 频率输出满量程
    FLOAT freq_output_duty_cycle;    // 地址65: 频率输出占空比
    
    // ==================== 电源参数 ====================
    SHORT battery_voltage;           // 地址67: 电池电量
    SHORT battery_low_alarm;         // 地址68: 电池低电量报警
    SHORT memory_fault_alarm;        // 地址69: 存储器故障报警
    
    // ==================== 压力参数 ====================
    SHORT pressure_measure_en;       // 地址70: 压力测量使能
    SHORT pressure_unit;             // 地址71: 压力测量单位
    FLOAT pressure_value;            // 地址72: 压力测量数值
    FLOAT pressure_high_threshold;   // 地址74: 压力上限报警阈值
    FLOAT pressure_low_threshold;    // 地址76: 压力下限报警阈值
    SHORT pressure_high_alarm;       // 地址78: 压力上限报警
    SHORT pressure_low_alarm;        // 地址79: 压力下限报警
    
    // ==================== 温度参数 ====================
    SHORT temperature_measure_en;    // 地址80: 温度测量使能
    FLOAT temperature_value;         // 地址81: 温度测量数值
    FLOAT temperature_high_threshold;// 地址83: 温度上限报警阈值
    FLOAT temperature_low_threshold; // 地址85: 温度下限报警阈值
    SHORT temperature_high_alarm;    // 地址87: 温度上限报警
    SHORT temperature_low_alarm;     // 地址88: 温度下限报警
    
    // ==================== 系统参数 ====================
    SHORT empty_pipe_value;          // 地址89: 空管测量值
    LONG work_hours;                 // 地址90: 工作总小时数
    SHORT remaining_quick_measure;   // 地址92: 剩余快速测量次数
    LONG converter_code;             // 地址93: 转换器编码
    LONG sensor_code;                // 地址95: 传感器编码
    
    // ==================== 通信参数 ====================
    SHORT modbus_addr;               // 地址97: Modbus通讯地址
    SHORT modbus_type;               // 地址98: Modbus通讯类型
    SHORT modbus_data_bits;          // 地址99: Modbus通讯数据位
    SHORT modbus_parity;             // 地址100: Modbus通讯校验位
    SHORT modbus_stop_bits;          // 地址101: Modbus通讯停止位
    SHORT modbus_baudrate;           // 地址102: Modbus通讯波特率
    
    // ==================== 系统标识参数 ====================
    LONG station_id;                 // 地址103: 台位号
    LONG timestamp;                  // 地址105: 时间戳
    LONG permission1_pwd;            // 地址107: 权限1密码
    LONG permission2_pwd;            // 地址109: 权限2密码
    LONG total_set_pwd;              // 地址111: 累积量设置密码
    LONG user_input_pwd;             // 地址113: 用户输入密码
    
    // ==================== 显示板参数 ====================
    SHORT accumulator_display;       // 地址115: 累积器显示
    SHORT parameter_lock;            // 地址116: 参数锁
    SHORT restart_display;           // 地址117: 重启显示通信板
    SHORT ble_device_id;             // 地址118: BLE设备编号的低4位数字
    SHORT display_language;          // 地址119: 显示通信板语言选择
    SHORT status_output_func;        // 地址120: 状态输出功能配置
    SHORT digital_pressure_sensor_type; // 地址121: 数字压力传感器类型
    SHORT software_license;          // 地址122: 软件合法授权
    LONG software_serial;            // 地址123: 软件序列号
    SHORT oem_select;                // 地址125: OEM选择
    SHORT firmware_version;          // 地址126: 仪表固件版本号
    LONG factory_id_high;            // 地址127: 出厂ID号高位
    LONG factory_id_low;             // 地址129: 出厂ID号低位
    SHORT slave_modbus_addr;         // 地址131: 从机Modbus地址
    
    // ==================== 备份参数 ====================
    FLOAT instant_flow_backup;       // 地址132: 瞬时流量备份
    LONG positive_total_int_backup;  // 地址134: 正向总量整数位备份
    FLOAT positive_total_frac_backup; // 地址136: 正向总量小数位备份
    LONG negative_total_int_backup;  // 地址138: 反向总量整数位备份
    FLOAT negative_total_frac_backup;// 地址140: 反向总量小数位备份
    FLOAT flow_velocity_backup;      // 地址142: 流速备份
    FLOAT pressure_backup;           // 地址144: 压力备份
    SHORT meter_battery_voltage;     // 地址146: 仪表电池电压
    LONG status_word;                // 地址147: 状态位
    FLOAT temperature_backup;        // 地址149: 温度备份
    SHORT measure_mode_backup;       // 地址151: 测量模式备份
    SHORT empty_pipe_value_backup;   // 地址152: 空管测量值备份
    LONG timestamp_backup;           // 地址153: 时间戳备份
    
    // ==================== 明渠参数 ====================
    FLOAT liquid_level;              // 地址155: 液位
    FLOAT cross_section_area;        // 地址157: 截面积
    SHORT reserved_159;              // 地址159: 预留
    FLOAT correct_point_a_actual;    // 地址160: 修正点A实际流量值
    FLOAT correct_point_a_meter;     // 地址162: 修正点A仪表测量值
    FLOAT correct_point_b_actual;    // 地址164: 修正点B实际流量值
    FLOAT correct_point_b_meter;     // 地址166: 修正点B仪表测量值
    FLOAT accumulator_alarm_threshold; // 地址168: 累积器报警阈值
    SHORT channel_type;              // 地址170: 明渠类别
    FLOAT channel_depth;             // 地址171: 渠深h
    FLOAT channel_top_width;         // 地址173: 上底a
    FLOAT channel_bottom_width;      // 地址175: 下底b
    FLOAT channel_param_c;           // 地址177: 参数c
    SHORT reserved_179;              // 地址179: 预留
    
    // ==================== 系统功能参数 ====================
    LONG device_number;              // 地址180: 装置号
    SHORT data_record_interval;      // 地址182: 数据记录间隔时间
    SHORT upload_interval;           // 地址183: 后台上送时间间隔
    SHORT test_email;                // 地址184: 测试发送邮件
    SHORT increment_unit;            // 地址185: 增量单位
    FLOAT positive_increment;        // 地址186: 正向增量
    FLOAT negative_increment;        // 地址188: 负向增量
    
    // ==================== 备份数据参数 ====================
    LONG positive_total_int_backup1; // 地址190: 正向累计流量整数部分备份1
    FLOAT positive_total_frac_backup1; // 地址192: 正向累计流量小数部分备份1
    LONG positive_total_int_backup2; // 地址194: 正向累计流量整数部分备份2
    FLOAT positive_total_frac_backup2; // 地址196: 正向累计流量小数部分备份2
    LONG negative_total_int_backup1; // 地址198: 负向累计流量整数部分备份1
    FLOAT negative_total_frac_backup1; // 地址200: 负向累计流量小数部分备份1
    LONG negative_total_int_backup2; // 地址202: 负向累计流量整数部分备份2
    FLOAT negative_total_frac_backup2; // 地址204: 负向累计流量小数部分备份2
    
    // ==================== 预留区域 ====================
    SHORT reserved_206;              // 地址206: 预留
    SHORT reserved_208;              // 地址208: 预留
    SHORT reserved_210;              // 地址210: 预留
    SHORT reserved_212;              // 地址212: 预留
    SHORT reserved_214;              // 地址214: 预留
    SHORT reserved_216;              // 地址216: 预留
    SHORT reserved_218;              // 地址218: 预留
    SHORT reserved_220;              // 地址220: 预留
    SHORT reserved_222;              // 地址222: 预留
    SHORT reserved_224;              // 地址224: 预留
    SHORT reserved_226;              // 地址226: 预留
    SHORT reserved_228;              // 地址228: 预留
    
    // ==================== 校准参数 ====================
    SHORT calibrate_4_20ma;          // 地址230: 4-20mA校准
    FLOAT calibrate_4ma;             // 地址231: 4mA校准值
    FLOAT calibrate_20ma;            // 地址233: 20mA校准值
    SHORT current_output_polarity;   // 地址235: 电流输出极性模式
    SHORT reserved_236;              // 地址236: 预留
    SHORT reserved_238;              // 地址238: 预留
    
    // ==================== 显示板信息 ====================
    LONG display_factory_id_high;    // 地址240: 显示板出厂ID号的高位
    LONG display_factory_id_low;     // 地址242: 显示板出厂ID号的低位
    SHORT display_software_license;  // 地址244: 显示板软件合法授权
    LONG display_software_serial;    // 地址245: 显示板软件序列号
    SHORT reserved_247;              // 地址247: 预留
    
    
    // ==================== 程序内部使用参数，不公开与modbus.  反正这里方便统一管理参数 ====================
    LONG Internal_zero_point; //流量计的零点，adc的滤波值
    FLOAT Internal_K_small; //小流量的K系数
    FLOAT Internal_K_large; //大流量的K系数
} EMDCB_Params_t;

// 全局参数实例声明

// 函数声明
void emdcb_params_init(void);
uint8_t emdcb_register_read(uint16_t addr, uint16_t *value);
uint8_t emdcb_register_write(uint16_t addr, uint16_t value);

void emdcb_init_default_params(EMDCB_Params_t* params) ;


void  emdcb_ModbusSetting(uint16_t reg_addr, uint16_t reg_count);

//static uint8_t check_permission(RegisterPermission required_perm) ;
//void emdcb_set_permission(uint8_t perm) ;
//uint8_t emdcb_get_permission(void) ;



#endif // EMDCB_MODBUS_PARAMS_H

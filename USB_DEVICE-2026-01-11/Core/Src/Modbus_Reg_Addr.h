#ifndef MODBUS_REGISTER_MAP_H
#define MODBUS_REGISTER_MAP_H

/*
 * 寄存器地址宏定义
 * 说明: 
 * - 地址值为Modbus协议中的相对地址（或称协议地址），通常为16位无符号整数[4](@ref)。
 * - 此地址用于在eMBRegHoldingCB等回调函数中定位具体寄存器[1,2](@ref)。
 * - 保持寄存器（Holding Register）通常对应功能码03H（读）、06H（写单个）、10H（写多个）[6,7](@ref)。
 */

// ==================== 流量测量相关寄存器 ====================
#define REG_ADDR_INSTANT_FLOW          0   // 瞬时流量
#define REG_ADDR_FLOW_UNIT             2   // 瞬时流量单位
#define REG_ADDR_POSITIVE_TOTAL_INT    3   // 正向总量整数位
#define REG_ADDR_POSITIVE_TOTAL_FRAC   5   // 正向总量小数位
#define REG_ADDR_TOTAL_UNIT            7   // 总量累积单位（正向总量、反向总量、净量、绝对总量）
#define REG_ADDR_NEGATIVE_TOTAL_INT    8   // 反向总量整数位
#define REG_ADDR_NEGATIVE_TOTAL_FRAC   10  // 反向总量小数位
#define REG_ADDR_FLOW_VELOCITY         18  // 流速
#define REG_ADDR_FLOW_PERCENTAGE       20  // 流量百分比
#define REG_ADDR_POSITIVE_INCREMENT    186 // 正向增量
#define REG_ADDR_NEGATIVE_INCREMENT    188 // 负向增量

// ==================== 报警状态寄存器 ====================
#define REG_ADDR_EXCITATION_ALARM      13  // 励磁报警
#define REG_ADDR_EMPTY_PIPE_ALARM      14  // 空管报警
#define REG_ADDR_REVERSE_FLOW_ALARM    15  // 流量反向报警
#define REG_ADDR_FLOW_HIGH_ALARM       16  // 流量上限报警
#define REG_ADDR_FLOW_LOW_ALARM        17  // 流量下限报警
#define REG_ADDR_BATTERY_LOW_ALARM     68  // 电池低电量报警
#define REG_ADDR_MEMORY_FAULT_ALARM    69  // 存储器故障报警
#define REG_ADDR_PRESSURE_HIGH_ALARM   78  // 压力上限报警
#define REG_ADDR_PRESSURE_LOW_ALARM    79  // 压力下限报警
#define REG_ADDR_TEMPERATURE_HIGH_ALARM 87 // 温度上限报警
#define REG_ADDR_TEMPERATURE_LOW_ALARM 88  // 温度下限报警

// ==================== 设备参数与配置寄存器 ====================
#define REG_ADDR_METER_RANGE           22  // 仪表量程设置
#define REG_ADDR_METER_CALIBER         24  // 仪表口径
#define REG_ADDR_FLOW_DIRECTION_SET    25  // 流量方向设置
#define REG_ADDR_REVERSE_FLOW_ALARM_EN 26  // 流量反向报警使能
#define REG_ADDR_START_FLOW_VELOCITY   27  // 始动流速
#define REG_ADDR_FLOW_FILTER_SET       29  // 流量滤波设置
#define REG_ADDR_FLOW_LIMIT_ALARM_EN   30  // 流量超限报警使能
#define REG_ADDR_FLOW_HIGH_THRESHOLD   31  // 流量上限报警阈值
#define REG_ADDR_FLOW_LOW_THRESHOLD    33  // 流量下限报警阈值
#define REG_ADDR_EMPTY_PIPE_ALARM_EN   35  // 空管报警使能
#define REG_ADDR_EMPTY_PIPE_THRESHOLD  36  // 空管报警阈值
#define REG_ADDR_EXCITATION_MODE       38  // 励磁方式选择
#define REG_ADDR_MEASURE_MODE          43  // 测量模式切换
#define REG_ADDR_MEASURE_MODE_BACKUP   151 // 测量模式（备份）

// ==================== 压力与温度测量寄存器 ====================
#define REG_ADDR_PRESSURE_ZERO         12  // 压力零点值
#define REG_ADDR_PRESSURE_MEASURE_EN   70  // 压力测量使能
#define REG_ADDR_PRESSURE_UNIT         71  // 压力测量单位
#define REG_ADDR_PRESSURE_VALUE        72  // 压力测量数值
#define REG_ADDR_PRESSURE_HIGH_THRESHOLD 74 // 压力上限报警阈值
#define REG_ADDR_PRESSURE_LOW_THRESHOLD 76 // 压力下限报警阈值
#define REG_ADDR_TEMPERATURE_MEASURE_EN 80 // 温度测量使能
#define REG_ADDR_TEMPERATURE_VALUE     81  // 温度测量数值
#define REG_ADDR_TEMPERATURE_HIGH_THRESHOLD 83 // 温度上限报警阈值
#define REG_ADDR_TEMPERATURE_LOW_THRESHOLD 85 // 温度下限报警阈值
#define REG_ADDR_PRESSURE_BACKUP      144  // 压力（备份）
#define REG_ADDR_TEMPERATURE_BACKUP   149  // 温度（备份）

// ==================== 系统与标识寄存器 ====================
#define REG_ADDR_WORK_HOURS            90  // 工作总小时数
#define REG_ADDR_REMAINING_QUICK_MEASURE 92 // 剩余快速测量次数
#define REG_ADDR_CONVERTER_CODE        93  // 转换器编码
#define REG_ADDR_SENSOR_CODE           95  // 传感器编码
#define REG_ADDR_STATION_ID            103 // 台位号
#define REG_ADDR_TIMESTAMP             105 // 时间戳
#define REG_ADDR_FIRMWARE_VERSION      126 // 仪表固件版本号
#define REG_ADDR_FACTORY_ID_HIGH       127 // 出厂ID号高位
#define REG_ADDR_FACTORY_ID_LOW        129 // 出厂ID号低位
#define REG_ADDR_TIMESTAMP_BACKUP      153 // 时间戳（备份）
#define REG_ADDR_DEVICE_NUMBER         180 // 装置号

// ==================== 通信参数寄存器 ====================
#define REG_ADDR_MODBUS_ADDR           97  // Modbus通讯地址
#define REG_ADDR_MODBUS_TYPE           98  // Modbus通讯类型
#define REG_ADDR_MODBUS_DATA_BITS      99  // Modbus通讯数据位
#define REG_ADDR_MODBUS_PARITY         100 // Modbus通讯校验位
#define REG_ADDR_MODBUS_STOP_BITS      101 // Modbus通讯停止位
#define REG_ADDR_MODBUS_BAUDRATE       102 // Modbus通讯波特率
#define REG_ADDR_SLAVE_MODBUS_ADDR     131 // 从机Modbus地址

// ==================== 校准与修正寄存器 ====================
#define REG_ADDR_FLUID_DENSITY         44  // 流体密度
#define REG_ADDR_CONVERTER_FACTOR      47  // 转换器系数
#define REG_ADDR_LARGE_FLOW_ACTUAL     49  // 大流量点实际流量值
#define REG_ADDR_LARGE_FLOW_METER      51  // 大流量点仪表测量值
#define REG_ADDR_SMALL_FLOW_ACTUAL     53  // 小流量的实际流量值
#define REG_ADDR_SMALL_FLOW_METER      55  // 小流量点仪表测量值
#define REG_ADDR_ZERO_POINT_METER      57  // 零点的仪表测量值
#define REG_ADDR_CORRECT_POINT_A_ACTUAL 160 // 修正点A实际流量值
#define REG_ADDR_CORRECT_POINT_A_METER 162 // 修正点A仪表测量值
#define REG_ADDR_CORRECT_POINT_B_ACTUAL 164 // 修正点B实际流量值
#define REG_ADDR_CORRECT_POINT_B_METER 166 // 修正点B仪表测量值
#define REG_ADDR_CALIBRATE_4_20MA      230 // 4-20mA校准
#define REG_ADDR_CALIBRATE_4MA         231 // 4mA校准值
#define REG_ADDR_CALIBRATE_20MA        233 // 20mA校准值
#define REG_ADDR_CURRENT_OUTPUT_POLARITY 235 // 电流输出极性模式

// ==================== 显示与界面相关寄存器 ====================
#define REG_ADDR_ACCUMULATOR_DISPLAY   115 // 累积器显示
#define REG_ADDR_PARAMETER_LOCK       116 // 参数锁
#define REG_ADDR_RESTART_DISPLAY      117 // 重启显示通信板
#define REG_ADDR_BLE_DEVICE_ID        118 // BLE设备编号的低4位数字
#define REG_ADDR_DISPLAY_LANGUAGE     119 // 显示通信板语言选择
#define REG_ADDR_DISPLAY_FACTORY_ID_HIGH 240 // 显示板出厂ID号的高位
#define REG_ADDR_DISPLAY_FACTORY_ID_LOW 242 // 显示板出厂ID号的低位
#define REG_ADDR_DISPLAY_SOFTWARE_LICENSE 244 // 显示板软件合法授权
#define REG_ADDR_DISPLAY_SOFTWARE_SERIAL 245 // 显示板软件序列号

// ==================== 安全与权限寄存器 ====================
#define REG_ADDR_PERMISSION1_PWD       107 // 权限1密码
#define REG_ADDR_PERMISSION2_PWD       109 // 权限2密码
#define REG_ADDR_TOTAL_SET_PWD         111 // 累积量设置密码
#define REG_ADDR_USER_INPUT_PWD        113 // 用户输入密码
#define REG_ADDR_SOFTWARE_LICENSE      122 // 软件合法授权
#define REG_ADDR_SOFTWARE_SERIAL       123 // 软件序列号
#define REG_ADDR_OEM_SELECT            125 // OEM选择

// ==================== 明渠测量相关寄存器 ====================
#define REG_ADDR_LIQUID_LEVEL         155 // 液位
#define REG_ADDR_CROSS_SECTION_AREA    157 // 截面积
#define REG_ADDR_CHANNEL_TYPE          170 // 明渠类别
#define REG_ADDR_CHANNEL_DEPTH         171 // 渠深h
#define REG_ADDR_CHANNEL_TOP_WIDTH     173 // 上底a
#define REG_ADDR_CHANNEL_BOTTOM_WIDTH  175 // 下底b
#define REG_ADDR_CHANNEL_PARAM_C       177 // 参数c

// ==================== 数据记录与备份寄存器 ====================
#define REG_ADDR_DATA_RECORD_INTERVAL  182 // 数据记录间隔时间
#define REG_ADDR_UPLOAD_INTERVAL       183 // 后台上送时间间隔
#define REG_ADDR_TEST_EMAIL            184 // 测试发送邮件
#define REG_ADDR_INCREMENT_UNIT        185 // 增量单位
#define REG_ADDR_ACCUMULATOR_ALARM_THRESHOLD 168 // 累积器报警阈值

// 正向总量备份
#define REG_ADDR_POSITIVE_TOTAL_INT_BACKUP1   190 // 正向累计流量整数部分备份1
#define REG_ADDR_POSITIVE_TOTAL_FRAC_BACKUP1  192 // 正向累计流量小数部分备份1
#define REG_ADDR_POSITIVE_TOTAL_INT_BACKUP2   194 // 正向累计流量整数部分备份2
#define REG_ADDR_POSITIVE_TOTAL_FRAC_BACKUP2  196 // 正向累计流量小数部分备份2
// 负向总量备份
#define REG_ADDR_NEGATIVE_TOTAL_INT_BACKUP1   198 // 负向累计流量整数部分备份1
#define REG_ADDR_NEGATIVE_TOTAL_FRAC_BACKUP1  200 // 负向累计流量小数部分备份1
#define REG_ADDR_NEGATIVE_TOTAL_INT_BACKUP2   202 // 负向累计流量整数部分备份2
#define REG_ADDR_NEGATIVE_TOTAL_FRAC_BACKUP2  204 // 负向累计流量小数部分备份2

// ==================== 数字I/O与脉冲输出寄存器 ====================
#define REG_ADDR_DIGITAL_INPUT2_FUNC    46  // 数字输入2功能
#define REG_ADDR_DIGITAL_OUTPUT1_TYPE  59  // 数字输出1类型设置
#define REG_ADDR_PULSE_UNIT_EQUIVALENT 60  // 脉冲单位当量
#define REG_ADDR_PULSE_MEASURE_OBJECT  62  // 脉冲测量对象
#define REG_ADDR_FREQ_OUTPUT_FULL_SCALE 63 // 频率输出满量程
#define REG_ADDR_FREQ_OUTPUT_DUTY_CYCLE 65 // 频率输出占空比
#define REG_ADDR_STATUS_OUTPUT_FUNC    120 // 状态输出功能配置
#define REG_ADDR_DIGITAL_PRESSURE_SENSOR_TYPE 121 // 数字压力传感器类型

// ==================== 时段设置寄存器 ====================
#define REG_ADDR_PERIOD1_START_HOUR    39  // 时段1起始小时数
#define REG_ADDR_PERIOD2_START_HOUR    40  // 时段2起始小时数
#define REG_ADDR_PERIOD1_INTERVAL      41  // 时段1间隔测量时间
#define REG_ADDR_PERIOD2_INTERVAL      42  // 时段2间隔测量时间

// ==================== 电源管理寄存器 ====================
#define REG_ADDR_BATTERY_VOLTAGE       67  // 电池电量
#define REG_ADDR_METER_BATTERY_VOLTAGE 146 // 仪表电池电压

// ==================== 备份数据寄存器 ====================
#define REG_ADDR_INSTANT_FLOW_BACKUP   132 // 瞬时流量（备份）
#define REG_ADDR_POSITIVE_TOTAL_INT_BACKUP 134 // 正向总量整数位（备份）
#define REG_ADDR_POSITIVE_TOTAL_FRAC_BACKUP 136 // 正向总量小数位（备份）
#define REG_ADDR_NEGATIVE_TOTAL_INT_BACKUP 138 // 反向总量整数位（备份）
#define REG_ADDR_NEGATIVE_TOTAL_FRAC_BACKUP 140 // 反向总量小数位（备份）
#define REG_ADDR_FLOW_VELOCITY_BACKUP  142 // 流速（备份）
#define REG_ADDR_EMPTY_PIPE_VALUE_BACKUP 152 // 空管测量值（备份）

// ==================== 状态与杂项寄存器 ====================
#define REG_ADDR_EMPTY_PIPE_VALUE      89  // 空管测量值
#define REG_ADDR_STATUS_WORD           147 // 状态位

// ==================== 预留寄存器区域 ====================
// 地址 159, 179, 206, 208, 210, 212, 214, 216, 218, 220, 222, 224, 226, 228, 236, 238, 247 为预留
// 在实际应用中，这些地址可能被用于未来功能扩展或特殊用途[2](@ref)。
#define REG_ADDR_RESERVED_159          159
#define REG_ADDR_RESERVED_179          179
#define REG_ADDR_RESERVED_206          206
#define REG_ADDR_RESERVED_208          208
#define REG_ADDR_RESERVED_210          210
#define REG_ADDR_RESERVED_212          212
#define REG_ADDR_RESERVED_214          214
#define REG_ADDR_RESERVED_216          216
#define REG_ADDR_RESERVED_218          218
#define REG_ADDR_RESERVED_220          220
#define REG_ADDR_RESERVED_222          222
#define REG_ADDR_RESERVED_224          224
#define REG_ADDR_RESERVED_226          226
#define REG_ADDR_RESERVED_228          228
#define REG_ADDR_RESERVED_236          236
#define REG_ADDR_RESERVED_238          238
#define REG_ADDR_RESERVED_247          247


#endif
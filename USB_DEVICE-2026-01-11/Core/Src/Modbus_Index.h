#ifndef MODBUS_INDEX_H
#define MODBUS_INDEX_H

#include "main.h"
// ==================== 基本数据类型定义 ====================

typedef int16_t  SHORT;
typedef int32_t  LONG;
typedef float    FLOAT;

// ==================== 寄存器索引宏定义 ====================
// 流量测量相关
#define REG_INDEX_INSTANT_FLOW          0   // 瞬时流量
#define REG_INDEX_FLOW_UNIT             1   // 瞬时流量单位
#define REG_INDEX_POSITIVE_TOTAL_INT    2   // 正向总量整数位
#define REG_INDEX_POSITIVE_TOTAL_FRAC   3   // 正向总量小数位
#define REG_INDEX_TOTAL_UNIT            4   // 总量累积单位
#define REG_INDEX_NEGATIVE_TOTAL_INT    5   // 反向总量整数位
#define REG_INDEX_NEGATIVE_TOTAL_FRAC   6   // 反向总量小数位
#define REG_INDEX_PRESSURE_ZERO         7   // 压力零点值

// 报警状态相关
#define REG_INDEX_EXCITATION_ALARM      8   // 励磁报警
#define REG_INDEX_EMPTY_PIPE_ALARM      9   // 空管报警
#define REG_INDEX_REVERSE_FLOW_ALARM    10  // 流量反向报警
#define REG_INDEX_FLOW_HIGH_ALARM       11  // 流量上限报警
#define REG_INDEX_FLOW_LOW_ALARM        12  // 流量下限报警

// 测量参数相关
#define REG_INDEX_FLOW_VELOCITY         13  // 流速
#define REG_INDEX_FLOW_PERCENTAGE       14  // 流量百分比

// 设备参数相关
#define REG_INDEX_METER_RANGE           15  // 仪表量程设置
#define REG_INDEX_METER_CALIBER         16  // 仪表口径
#define REG_INDEX_FLOW_DIRECTION_SET    17  // 流量方向设置
#define REG_INDEX_REVERSE_FLOW_ALARM_EN 18  // 流量反向报警使能
#define REG_INDEX_START_FLOW_VELOCITY   19  // 始动流速
#define REG_INDEX_FLOW_FILTER_SET       20  // 流量滤波设置
#define REG_INDEX_FLOW_LIMIT_ALARM_EN   21  // 流量超限报警使能
#define REG_INDEX_FLOW_HIGH_THRESHOLD   22  // 流量上限报警阈值
#define REG_INDEX_FLOW_LOW_THRESHOLD    23  // 流量下限报警阈值
#define REG_INDEX_EMPTY_PIPE_ALARM_EN   24  // 空管报警使能
#define REG_INDEX_EMPTY_PIPE_THRESHOLD  25  // 空管报警阈值
#define REG_INDEX_EXCITATION_MODE       26  // 励磁方式选择

// 时段设置相关
#define REG_INDEX_PERIOD1_START_HOUR    27  // 时段1起始小时数
#define REG_INDEX_PERIOD2_START_HOUR    28  // 时段2起始小时数
#define REG_INDEX_PERIOD1_INTERVAL      29  // 时段1间隔测量时间
#define REG_INDEX_PERIOD2_INTERVAL      30  // 时段2间隔测量时间
#define REG_INDEX_MEASURE_MODE          31  // 测量模式切换

// 转换器参数相关
#define REG_INDEX_FLUID_DENSITY         32  // 流体密度
#define REG_INDEX_DIGITAL_INPUT2_FUNC   33  // 数字输入2功能
#define REG_INDEX_CONVERTER_FACTOR      34  // 转换器系数
#define REG_INDEX_LARGE_FLOW_ACTUAL     35  // 大流量点实际流量值
#define REG_INDEX_LARGE_FLOW_METER      36  // 大流量点仪表测量值
#define REG_INDEX_SMALL_FLOW_ACTUAL     37  // 小流量的实际流量值
#define REG_INDEX_SMALL_FLOW_METER      38  // 小流量点仪表测量值
#define REG_INDEX_ZERO_POINT_METER      39  // 零点的仪表测量值
#define REG_INDEX_DIGITAL_OUTPUT1_TYPE  40  // 数字输出1类型设置
#define REG_INDEX_PULSE_UNIT_EQUIVALENT 41  // 脉冲单位当量
#define REG_INDEX_PULSE_MEASURE_OBJECT  42  // 脉冲测量对象
#define REG_INDEX_FREQ_OUTPUT_FULL_SCALE 43 // 频率输出满量程
#define REG_INDEX_FREQ_OUTPUT_DUTY_CYCLE 44 // 频率输出占空比

// 电源参数相关
#define REG_INDEX_BATTERY_VOLTAGE       45  // 电池电量
#define REG_INDEX_BATTERY_LOW_ALARM     46  // 电池低电量报警
#define REG_INDEX_MEMORY_FAULT_ALARM    47  // 存储器故障报警

// 压力参数相关
#define REG_INDEX_PRESSURE_MEASURE_EN   48  // 压力测量使能
#define REG_INDEX_PRESSURE_UNIT         49  // 压力测量单位
#define REG_INDEX_PRESSURE_VALUE        50  // 压力测量数值
#define REG_INDEX_PRESSURE_HIGH_THRESHOLD 51 // 压力上限报警阈值
#define REG_INDEX_PRESSURE_LOW_THRESHOLD 52  // 压力下限报警阈值
#define REG_INDEX_PRESSURE_HIGH_ALARM   53  // 压力上限报警
#define REG_INDEX_PRESSURE_LOW_ALARM    54  // 压力下限报警

// 温度参数相关
#define REG_INDEX_TEMPERATURE_MEASURE_EN 55 // 温度测量使能
#define REG_INDEX_TEMPERATURE_VALUE     56  // 温度测量数值
#define REG_INDEX_TEMPERATURE_HIGH_THRESHOLD 57 // 温度上限报警阈值
#define REG_INDEX_TEMPERATURE_LOW_THRESHOLD 58 // 温度下限报警阈值
#define REG_INDEX_TEMPERATURE_HIGH_ALARM 59  // 温度上限报警
#define REG_INDEX_TEMPERATURE_LOW_ALARM 60  // 温度下限报警

// 系统参数相关
#define REG_INDEX_EMPTY_PIPE_VALUE      61  // 空管测量值
#define REG_INDEX_WORK_HOURS            62  // 工作总小时数
#define REG_INDEX_REMAINING_QUICK_MEASURE 63 // 剩余快速测量次数
#define REG_INDEX_CONVERTER_CODE        64  // 转换器编码
#define REG_INDEX_SENSOR_CODE           65  // 传感器编码

// 通信参数相关
#define REG_INDEX_MODBUS_ADDR           66  // Modbus通讯地址
#define REG_INDEX_MODBUS_TYPE           67  // Modbus通讯类型
#define REG_INDEX_MODBUS_DATA_BITS      68  // Modbus通讯数据位
#define REG_INDEX_MODBUS_PARITY         69  // Modbus通讯校验位
#define REG_INDEX_MODBUS_STOP_BITS      70  // Modbus通讯停止位
#define REG_INDEX_MODBUS_BAUDRATE       71  // Modbus通讯波特率

// 系统标识相关
#define REG_INDEX_STATION_ID            72  // 台位号
#define REG_INDEX_TIMESTAMP             73  // 时间戳
#define REG_INDEX_PERMISSION1_PWD       74  // 权限1密码
#define REG_INDEX_PERMISSION2_PWD       75  // 权限2密码
#define REG_INDEX_TOTAL_SET_PWD         76  // 累积量设置密码
#define REG_INDEX_USER_INPUT_PWD        77  // 用户输入密码

// 显示板参数相关
#define REG_INDEX_ACCUMULATOR_DISPLAY   78  // 累积器显示
#define REG_INDEX_PARAMETER_LOCK        79  // 参数锁
#define REG_INDEX_RESTART_DISPLAY       80  // 重启显示通信板
#define REG_INDEX_BLE_DEVICE_ID         81  // BLE设备编号的低4位数字
#define REG_INDEX_DISPLAY_LANGUAGE      82  // 显示通信板语言选择
#define REG_INDEX_STATUS_OUTPUT_FUNC    83  // 状态输出功能配置
#define REG_INDEX_DIGITAL_PRESSURE_SENSOR_TYPE 84 // 数字压力传感器类型
#define REG_INDEX_SOFTWARE_LICENSE      85  // 软件合法授权
#define REG_INDEX_SOFTWARE_SERIAL       86  // 软件序列号
#define REG_INDEX_OEM_SELECT            87  // OEM选择
#define REG_INDEX_FIRMWARE_VERSION      88  // 仪表固件版本号
#define REG_INDEX_FACTORY_ID_HIGH       89  // 出厂ID号高位
#define REG_INDEX_FACTORY_ID_LOW        90  // 出厂ID号低位
#define REG_INDEX_SLAVE_MODBUS_ADDR     91  // 从机Modbus地址

// 备份参数相关
#define REG_INDEX_INSTANT_FLOW_BACKUP   92  // 瞬时流量备份
#define REG_INDEX_POSITIVE_TOTAL_INT_BACKUP 93 // 正向总量整数位备份
#define REG_INDEX_POSITIVE_TOTAL_FRAC_BACKUP 94 // 正向总量小数位备份
#define REG_INDEX_NEGATIVE_TOTAL_INT_BACKUP 95 // 反向总量整数位备份
#define REG_INDEX_NEGATIVE_TOTAL_FRAC_BACKUP 96 // 反向总量小数位备份
#define REG_INDEX_FLOW_VELOCITY_BACKUP  97  // 流速备份
#define REG_INDEX_PRESSURE_BACKUP       98  // 压力备份
#define REG_INDEX_METER_BATTERY_VOLTAGE 99  // 仪表电池电压
#define REG_INDEX_STATUS_WORD           100 // 状态位
#define REG_INDEX_TEMPERATURE_BACKUP    101 // 温度备份
#define REG_INDEX_MEASURE_MODE_BACKUP   102 // 测量模式备份
#define REG_INDEX_EMPTY_PIPE_VALUE_BACKUP 103 // 空管测量值备份
#define REG_INDEX_TIMESTAMP_BACKUP      104 // 时间戳备份

// 明渠参数相关
#define REG_INDEX_LIQUID_LEVEL          105 // 液位
#define REG_INDEX_CROSS_SECTION_AREA    106 // 截面积
#define REG_INDEX_CORRECT_POINT_A_ACTUAL 107 // 修正点A实际流量值
#define REG_INDEX_CORRECT_POINT_A_METER 108 // 修正点A仪表测量值
#define REG_INDEX_CORRECT_POINT_B_ACTUAL 109 // 修正点B实际流量值
#define REG_INDEX_CORRECT_POINT_B_METER 110 // 修正点B仪表测量值
#define REG_INDEX_ACCUMULATOR_ALARM_THRESHOLD 111 // 累积器报警阈值
#define REG_INDEX_CHANNEL_TYPE          112 // 明渠类别
#define REG_INDEX_CHANNEL_DEPTH         113 // 渠深h
#define REG_INDEX_CHANNEL_TOP_WIDTH     114 // 上底a
#define REG_INDEX_CHANNEL_BOTTOM_WIDTH  115 // 下底b
#define REG_INDEX_CHANNEL_PARAM_C       116 // 参数c

// 系统功能相关
#define REG_INDEX_DEVICE_NUMBER         117 // 装置号
#define REG_INDEX_DATA_RECORD_INTERVAL  118 // 数据记录间隔时间
#define REG_INDEX_UPLOAD_INTERVAL       119 // 后台上送时间间隔
#define REG_INDEX_TEST_EMAIL            120 // 测试发送邮件
#define REG_INDEX_INCREMENT_UNIT        121 // 增量单位
#define REG_INDEX_POSITIVE_INCREMENT    122 // 正向增量
#define REG_INDEX_NEGATIVE_INCREMENT    123 // 负向增量

// 备份数据相关
#define REG_INDEX_POSITIVE_TOTAL_INT_BACKUP1 124 // 正向累计流量整数部分备份1
#define REG_INDEX_POSITIVE_TOTAL_FRAC_BACKUP1 125 // 正向累计流量小数部分备份1
#define REG_INDEX_POSITIVE_TOTAL_INT_BACKUP2 126 // 正向累计流量整数部分备份2
#define REG_INDEX_POSITIVE_TOTAL_FRAC_BACKUP2 127 // 正向累计流量小数部分备份2
#define REG_INDEX_NEGATIVE_TOTAL_INT_BACKUP1 128 // 负向累计流量整数部分备份1
#define REG_INDEX_NEGATIVE_TOTAL_FRAC_BACKUP1 129 // 负向累计流量小数部分备份1
#define REG_INDEX_NEGATIVE_TOTAL_INT_BACKUP2 130 // 负向累计流量整数部分备份2
#define REG_INDEX_NEGATIVE_TOTAL_FRAC_BACKUP2 131 // 负向累计流量小数部分备份2

// 校准参数相关
#define REG_INDEX_CALIBRATE_4_20MA      132 // 4-20mA校准
#define REG_INDEX_CALIBRATE_4MA         133 // 4mA校准值
#define REG_INDEX_CALIBRATE_20MA        134 // 20mA校准值
#define REG_INDEX_CURRENT_OUTPUT_POLARITY 135 // 电流输出极性模式

// 显示板信息相关
#define REG_INDEX_DISPLAY_FACTORY_ID_HIGH 136 // 显示板出厂ID号的高位
#define REG_INDEX_DISPLAY_FACTORY_ID_LOW 137 // 显示板出厂ID号的低位
#define REG_INDEX_DISPLAY_SOFTWARE_LICENSE 138 // 显示板软件合法授权
#define REG_INDEX_DISPLAY_SOFTWARE_SERIAL 139 // 显示板软件序列号

// 寄存器总数
#define REG_TOTAL_COUNT                 140


// 瞬时流量单位
#define FLOW_UNIT_M3H           0x02
// 总量单位
#define TOTAL_UNIT_M3           0x00

// 测量模式定义
typedef enum {
    MEASURE_MODE_CYCLE = 0,        // 周期测量
    MEASURE_MODE_CALIBRATION = 6,   // 标定模式
    MEASURE_MODE_VERIFICATION = 8,  // 检定模式
    MEASURE_MODE_FREQ_1HZ = 10,     // 频率仿真1Hz
    MEASURE_MODE_FREQ_10HZ = 20,    // 频率仿真10Hz
    MEASURE_MODE_FREQ_100HZ = 30,   // 频率仿真100Hz
    MEASURE_MODE_FREQ_1000HZ = 40   // 频率仿真1000Hz
} MeasureMode_t;

// 明渠类别
typedef enum {
    CHANNEL_TYPE_NONE = 0,     // 非明渠
    CHANNEL_TYPE_CIRCLE = 1,   // 圆形
    CHANNEL_TYPE_RECT = 2,     // 矩形
    CHANNEL_TYPE_TRAPEZOID = 3,// 梯形
    CHANNEL_TYPE_TRIANGLE = 4, // 三角形
    CHANNEL_TYPE_U = 5         // U型
} ChannelType_t;

// ==================== 寄存器权限定义 ====================
typedef enum {
    PERM_READ_ONLY   = 0,  // 只读权限
    PERM_WRITE_ONLY   = 1,  // 只写权限
    PERM_READ_WRITE  = 2,  // 可读读权限
    PERM_LEVEL_01    = 3,  // 权限1可写
    PERM_LEVEL_02    = 4,  // 权限2可写
    PERM_LEVEL_03    = 5,  // 权限3可写
    PERM_RESERVED    = 6   // 预留寄存器
} RegisterPermission;


// ==================== 寄存器映射结构体 ====================
typedef struct {
    uint16_t addr;           // Modbus寄存器地址（十进制）
    uint8_t  size;           // 占用寄存器数量（1=16位，2=32位）
    RegisterPermission permission; // 访问权限
    void     *data_ptr;      // 数据指针
    const char *desc;        // 寄存器描述
} RegisterMap_t;

// ==================== 函数声明 ====================

// 权限管理函数
void emdcb_set_permission(uint8_t perm);
uint8_t emdcb_get_permission(void);

// 通过索引快速读取函数
uint8_t emdcb_get_short_by_index(uint16_t index, SHORT* value);
uint8_t emdcb_get_long_by_index(uint16_t index, LONG* value);
uint8_t emdcb_get_float_by_index(uint16_t index, FLOAT* value);
FLOAT emdcb_get_param_by_index(uint16_t index);

// 通过索引快速设置函数
uint8_t emdcb_set_short_by_index(uint16_t index, SHORT value);
uint8_t emdcb_set_long_by_index(uint16_t index, LONG value);
uint8_t emdcb_set_float_by_index(uint16_t index, FLOAT value);
uint8_t emdcb_set_param_by_index(uint16_t index, FLOAT value);

// 批量操作函数
uint16_t emdcb_batch_get_by_indices(uint16_t* indices, uint16_t count, FLOAT* values);
uint16_t emdcb_batch_set_by_indices(uint16_t* indices, uint16_t count, FLOAT* values);

// 通过索引查找寄存器信息
const RegisterMap_t* emdcb_get_register_by_index(uint16_t index);
uint16_t emdcb_get_addr_by_index(uint16_t index);
const char* emdcb_get_desc_by_index(uint16_t index);

// 初始化函数
void emdcb_init_all_params(void);
void init_index_table(void) ;

//转换函数
uint16_t swap_uint16(uint16_t value) ;
uint32_t swap_uint32(uint32_t value) ;
float swap_float(float value) ;


// 测试函数
void emdcb_test_index_access(void);
void emdcb_main_test_index(void);

#endif // EMDCB_MODBUS_INDEX_H

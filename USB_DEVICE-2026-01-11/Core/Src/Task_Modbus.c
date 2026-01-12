
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "string.h"
#include <stdio.h>
#include <string.h>
#include "Modbus_Param.h"
#include "Modbus_index.h"
#include "usart_bsp.h"

#include "in_flash_bsp.h"



// 设备地址
#define MODBUS_DEVICE_ADDR 0x01

// Modbus异常码定义
#define MODBUS_EX_ILLEGAL_FUNCTION 0x01
#define MODBUS_EX_ILLEGAL_DATA_ADDRESS 0x02
#define MODBUS_EX_ILLEGAL_DATA_VALUE 0x03
#define MODBUS_EX_SLAVE_DEVICE_FAILURE 0x04
#define MODBUS_EX_ACKNOWLEDGE 0x05
#define MODBUS_EX_SLAVE_DEVICE_BUSY 0x06
#define MODBUS_EX_MEMORY_PARITY_ERROR 0x08
#define MODBUS_EX_GATEWAY_PATH_UNAVAILABLE 0x0A
#define MODBUS_EX_GATEWAY_TARGET_FAILED 0x0B

/*Modbus任务*/
void Modbus_Task_Proc(void const * argument);


extern const RegisterMap_t g_register_map[];
extern QueueHandle_t xRs485Semaphore ;
//extern QueueHandle_t xAdcFinishForFlashSemaphore;
extern osSemaphoreId xTask_LC_Mutex ;

uint8_t g_rx_buffer[256];
uint16_t g_rx_len;
uint8_t g_tx_buffer[256];
uint16_t g_tx_len;

uint8_t g_now_permission = PERM_LEVEL_03; //最高权限 测试用

// CRC16计算
static uint16_t modbus_crc16(uint8_t *ptr, uint16_t len) 
{
    uint16_t wcrc=0XFFFF;  //预置16位crc寄存器，初值全部为1  
    uint8_t temp = 0; //定义中间变量 
    int16_t i=0,j=0;    //定义计数  

    for(i=0;i<len;i++)//循环计算每个数据  
    {  
        temp=*ptr & 0X00FF; //将八位数据与crc寄存器亦或 
        
        ptr++;  //指针地址增加，指向下个数据  
        wcrc ^= temp;//将数据存入crc寄存器  
        for(j=0;j<8;j++) //循环计算数据的  
        {  
            if(wcrc&0X0001) //判断右移出的是不是1，如果是1则与多项式进行异或。  
            {
                wcrc>>=1; //先将数据右移一位  
                wcrc^=0XA001; //与上面的多项式进行异或  
            }  
            else //如果不是1，则直接移出  
            {  
                wcrc>>=1;//直接移出  
            }  
        }  
    }  
    
    return wcrc;
}





// 查找包含指定地址的寄存器
static const RegisterMap_t* find_register_by_addr(uint16_t reg_addr) 
{
    for (int i = 0; i < REG_TOTAL_COUNT; i++) 
    {
        const RegisterMap_t* reg = &g_register_map[i];
        
        // 检查是否为结束标记
        if (reg->addr == 0xFFFF) {
            break;
        }
        
        // 检查地址是否在该寄存器范围内
        if (reg_addr >= reg->addr && reg_addr < reg->addr + reg->size) {
            return reg;
        }
    }
    
    return NULL;
}

// 检查写权限
static int check_write_permission(RegisterPermission permission, uint8_t now_permission) 
{
    // PERM_READ_ONLY (0x00) 只读，不可写
    if (permission == PERM_READ_ONLY) 
    {
        return 0;
    }
    
    // PERM_RESERVED (0xFF) 保留，不可写
    if (permission == PERM_RESERVED) 
    {
        return 0;
    }
    
    // 检查权限级别
    // 实际项目中，这里应该根据当前操作权限级别来判断
    // 这里简化处理，假设只要有写权限标志就可以写
   // if (permission == PERM_LEVEL_01 || permission == PERM_LEVEL_02 || permission == PERM_LEVEL_03) 
    if(now_permission >= permission)
    {
        return 1;
    }
    
    if (permission == PERM_READ_WRITE)
    {
        return 1;
    }
    
    if (permission == PERM_WRITE_ONLY)
    {
        return 1;
    }    
    
    return 0;
}

// 检查读权限
static int check_read_permission(RegisterPermission permission, uint8_t now_permission) 
{
    // PERM_RESERVED (0xFF) 保留，不可读
    if (permission == PERM_RESERVED) 
    {
        return 0;
    }
    
    if (permission == PERM_WRITE_ONLY)
    {
        return 0;
    } 
    // 检查权限级别
    // 实际项目中，这里应该根据当前操作权限级别来判断
    // 这里简化处理，假设只要有写权限标志就可以写
   // if (permission == PERM_LEVEL_01 || permission == PERM_LEVEL_02 || permission == PERM_LEVEL_03) 
    if(now_permission >= permission)
    {
        return 1;
    }
        
    return 0;
}

// 读取单个寄存器的16位值
static uint16_t read_single_register(uint16_t reg_addr) 
{
    const RegisterMap_t* reg = find_register_by_addr(reg_addr);
    
    if (reg == NULL || reg->data_ptr == NULL) 
    {
        return 0x0000;  // 返回0表示读取失败
    }
    
        // 检查权限
    if (!check_read_permission(reg->permission, g_now_permission)) 
    {
        return 0;  // 权限不足
    }
    
    // 计算在寄存器项内的偏移
    uint16_t offset = reg_addr - reg->addr;
    
    if (reg->size == 1) 
    {
        // 16位数据
        SHORT* data_ptr = (SHORT*)reg->data_ptr;
        return swap_uint16(*data_ptr);
    } 
    else if (reg->size == 2) 
    {
        // 32位数据
        if (offset == 0) 
        {
            // 读取高16位
            if (reg_addr <= REG_TOTAL_COUNT) 
            {  // 假设是FLOAT
                FLOAT* data_ptr = (FLOAT*)reg->data_ptr;
                FLOAT swapped_val = swap_float(*data_ptr);
                uint32_t int_val = *((uint32_t*)&swapped_val);
                return (int_val >> 16) & 0xFFFF;
            } else {
                // 假设是LONG
                LONG* data_ptr = (LONG*)reg->data_ptr;
                uint32_t swapped_val = swap_uint32(*data_ptr);
                return (swapped_val >> 16) & 0xFFFF;
            }
        } else if (offset == 1) 
        {
            // 读取低16位
            if (reg_addr <= REG_TOTAL_COUNT) 
            {  // 假设是FLOAT
                FLOAT* data_ptr = (FLOAT*)reg->data_ptr;
                FLOAT swapped_val = swap_float(*data_ptr);
                uint32_t int_val = *((uint32_t*)&swapped_val);
                return int_val & 0xFFFF;
            } else {
                // 假设是LONG
                LONG* data_ptr = (LONG*)reg->data_ptr;
                uint32_t swapped_val = swap_uint32(*data_ptr);
                return swapped_val & 0xFFFF;
            }
        }
    }
    
    return 0x0000;
}

// 写入单个寄存器的16位值
static int write_single_register(uint16_t reg_addr, uint16_t value) 
{
    const RegisterMap_t* reg = find_register_by_addr(reg_addr);
    if (reg == NULL || reg->data_ptr == NULL) 
    {
        return 0;  // 写入失败
    }
    
    // 检查写权限
    if (!check_write_permission(reg->permission, g_now_permission)) 
    {
        return 0;  // 权限不足
    }
    
    // 计算在寄存器项内的偏移
    uint16_t offset = reg_addr - reg->addr;
    
    if (reg->size == 1) 
    {
        // 16位数据
        SHORT* data_ptr = (SHORT*)reg->data_ptr;
        *data_ptr = (SHORT)swap_uint16(value);
        return 1;  // 写入成功
    } 
    else if (reg->size == 2) 
    {
        // 32位数据，需要读取当前值，修改部分，再写回
        if (reg_addr <= REG_TOTAL_COUNT) 
        {  // 假设是FLOAT
            FLOAT* data_ptr = (FLOAT*)reg->data_ptr;
            FLOAT current_val = *data_ptr;
            FLOAT swapped_val = current_val;//swap_float(current_val);
            uint32_t int_val = *((uint32_t*)&swapped_val);
            
            if (offset == 0) 
            {
                // 修改高16位
                int_val = (value << 16) | (int_val & 0xFFFF);
            } else if (offset == 1) 
            {
                // 修改低16位
                int_val = (int_val & 0xFFFF0000) | value;
            }
            
            uint32_t new_swapped = int_val;//swap_uint32(int_val);
            FLOAT new_val = *((FLOAT*)&new_swapped);
            *data_ptr = new_val;
            return 1;  // 写入成功
        } else {
            // 假设是LONG
            LONG* data_ptr = (LONG*)reg->data_ptr;
            LONG current_val = *data_ptr;
            uint32_t int_val = current_val;//swap_uint32(current_val);
            
            if (offset == 0) {
                // 修改高16位
                int_val = (value << 16) | (int_val & 0xFFFF);
            } else if (offset == 1) {
                // 修改低16位
                int_val = (int_val & 0xFFFF0000) | value;
            }
            
            uint32_t new_swapped = int_val;//swap_uint32(int_val);
            *data_ptr = (LONG)new_swapped;
            return 1;  // 写入成功
        }
    }
    
    return 0;  // 不支持的寄存器大小
}

// 处理03功能码 - 读取保持寄存器
static int process_read_holding_registers(uint8_t* frame, uint16_t len, 
                                          uint8_t* sendbuff, uint16_t* sendlen) 
{
    if (len < 8) 
    {  // 地址(1) + 功能码(1) + 起始地址(2) + 寄存器数量(2) + CRC(2)
        return 0;
    }
    
    // 解析请求
    uint16_t start_addr = (frame[2] << 8) | frame[3];
    uint16_t reg_count = (frame[4] << 8) | frame[5];
    
    // 检查寄存器数量
    if (reg_count == 0 || reg_count > REG_TOTAL_COUNT) 
    {
        // 返回非法数据值异常
        sendbuff[0] = frame[0];  // 从机地址
        sendbuff[1] = 0x83;      // 异常功能码 (0x80 + 0x03)
        sendbuff[2] = MODBUS_EX_ILLEGAL_DATA_VALUE;
        *sendlen = 3;
        return 1;
    }
    
    // 计算响应数据长度
    uint16_t data_len = reg_count * 2;
  /*  uint16_t response_len = 3 + data_len;  // 地址(1) + 功能码(1) + 字节数(1) + 数据(data_len) + CRC(2)
    
    if (response_len + 2 > *sendlen) 
    {  // +2 for CRC
        return 0;
    }*/
    
    // 构建响应头
    sendbuff[0] = frame[0];  // 从机地址
    sendbuff[1] = 0x03;      // 功能码
    sendbuff[2] = data_len;  // 字节数
    
    // 读取所有请求的寄存器
    uint8_t* data_ptr = &sendbuff[3];
    for (uint16_t i = 0; i < reg_count; i++) 
    {
        uint16_t reg_addr = start_addr + i;
        
        // 检查寄存器是否存在
        const RegisterMap_t* reg = find_register_by_addr(reg_addr);
        if (reg == NULL) 
        {
            // 寄存器不存在
            sendbuff[0] = frame[0];
            sendbuff[1] = 0x83;
            sendbuff[2] = MODBUS_EX_ILLEGAL_DATA_ADDRESS;
            *sendlen = 3;
            return 1;
        }
        
        // 读取寄存器值
        uint16_t reg_value = read_single_register(reg_addr);
        
        // 大端字节序存储
        data_ptr[i * 2] = (reg_value >> 8) & 0xFF;
        data_ptr[i * 2 + 1] = reg_value & 0xFF;
    }
    
    // 计算CRC
    uint16_t crc = modbus_crc16(sendbuff, 3 + data_len);
    sendbuff[3 + data_len] = crc & 0xFF;
    sendbuff[3 + data_len + 1] = (crc >> 8) & 0xFF;
    
    *sendlen = 3 + data_len + 2;
    return 1;
}

// 处理06功能码 - 写单个寄存器
static int process_write_single_register(uint8_t* frame, uint16_t len,
                                        uint8_t* sendbuff, uint16_t* sendlen) {
    if (len < 8) {  // 地址(1) + 功能码(1) + 寄存器地址(2) + 寄存器值(2) + CRC(2)
        return 0;
    }
    
    // 解析请求
    uint16_t reg_addr = (frame[2] << 8) | frame[3];
    uint16_t reg_value = (frame[4] << 8) | frame[5];
    
    // 检查寄存器是否存在
    const RegisterMap_t* reg = find_register_by_addr(reg_addr);
    if (reg == NULL) {
        // 寄存器不存在
        sendbuff[0] = frame[0];
        sendbuff[1] = 0x86;  // 异常功能码 (0x80 + 0x06)
        sendbuff[2] = MODBUS_EX_ILLEGAL_DATA_ADDRESS;
        *sendlen = 3;
        return 1;
    }
    
    // 写入寄存器
    if (!write_single_register(reg_addr, reg_value)) {
        // 写入失败
        sendbuff[0] = frame[0];
        sendbuff[1] = 0x86;
        sendbuff[2] = MODBUS_EX_SLAVE_DEVICE_FAILURE;
        *sendlen = 3;
        return 1;
    }
    

    
    memcpy(sendbuff, frame, 6);  // 复制请求的前6个字节
    sendbuff[1] = 0x06;  // 确保功能码正确
    
    // 计算CRC
    uint16_t crc = modbus_crc16(sendbuff, 6);
    sendbuff[6] = crc & 0xFF;
    sendbuff[7] = (crc >> 8) & 0xFF;
    
    *sendlen = 8;
    return 1;
}

// 处理16功能码 - 写多个寄存器
static int process_write_multiple_registers(uint8_t* frame, uint16_t len,
                                           uint8_t* sendbuff, uint16_t* sendlen) 
{
    if (len < 9) {  // 最小长度检查
        return 0;
    }
    
    // 解析请求
    uint16_t start_addr = (frame[2] << 8) | frame[3];
    uint16_t reg_count = (frame[4] << 8) | frame[5];
    uint8_t byte_count = frame[6];
    
    // 验证字节数
    if (byte_count != reg_count * 2) {
        sendbuff[0] = frame[0];
        sendbuff[1] = 0x90;  // 异常功能码 (0x80 + 0x10)
        sendbuff[2] = MODBUS_EX_ILLEGAL_DATA_VALUE;
        *sendlen = 3;
        return 1;
    }
    
    // 检查寄存器数量
    if (reg_count == 0 || reg_count > 123) {
        sendbuff[0] = frame[0];
        sendbuff[1] = 0x90;
        sendbuff[2] = MODBUS_EX_ILLEGAL_DATA_VALUE;
        *sendlen = 3;
        return 1;
    }
    
    // 检查所有寄存器是否存在并验证权限
    for (uint16_t i = 0; i < reg_count; i++) 
    {
        uint16_t reg_addr = start_addr + i;
        const RegisterMap_t* reg = find_register_by_addr(reg_addr);
        
        if (reg == NULL) {
            // 寄存器不存在
            sendbuff[0] = frame[0];
            sendbuff[1] = 0x90;
            sendbuff[2] = MODBUS_EX_ILLEGAL_DATA_ADDRESS;
            *sendlen = 3;
            return 1;
        }
        
        // 检查写权限
        if (!check_write_permission(reg->permission, g_now_permission))
        {
            // 无写权限
            sendbuff[0] = frame[0];
            sendbuff[1] = 0x90;
            sendbuff[2] = MODBUS_EX_ILLEGAL_DATA_VALUE;
            *sendlen = 3;
            return 1;
        }
    }
    
    // 写入所有寄存器
    for (uint16_t i = 0; i < reg_count; i++) 
    {
        uint16_t reg_addr = start_addr + i;
        uint16_t reg_value = (frame[7 + i * 2] << 8  |  frame[8 + i * 2]);
        
        if (!write_single_register(reg_addr, reg_value)) 
        {
            // 写入失败
            sendbuff[0] = frame[0];
            sendbuff[1] = 0x90;
            sendbuff[2] = MODBUS_EX_SLAVE_DEVICE_FAILURE;
            *sendlen = 3;
            return 1;
        }
    }
    
    // 构建成功响应
   /* if (*sendlen < 8) 
    {
        return 0;
    }*/
    
    // 响应格式：地址 + 功能码 + 起始地址 + 寄存器数量
    sendbuff[0] = frame[0];  // 从机地址
    sendbuff[1] = 0x10;      // 功能码
    sendbuff[2] = frame[2];  // 起始地址高字节
    sendbuff[3] = frame[3];  // 起始地址低字节
    sendbuff[4] = frame[4];  // 寄存器数量高字节
    sendbuff[5] = frame[5];  // 寄存器数量低字节
    
    // 计算CRC
    uint16_t crc = modbus_crc16(sendbuff, 6);
    sendbuff[6] = crc & 0xFF;
    sendbuff[7] = (crc >> 8) & 0xFF;
    
    *sendlen = 8;
    return 1;
}

// 主处理函数
int  modbus_process_frame(uint8_t* frame, uint16_t len, uint8_t* sendbuff, uint16_t* sendlen)
{
    int ret =0;
    // 检查基本帧长度
    if (len < 4) 
    {  // 最小帧：地址(1) + 功能码(1) + CRC(2)
        *sendlen = 0;
        return ret;
    }
    
    // 检查从机地址
    if (frame[0] != MODBUS_DEVICE_ADDR) 
    {
        // 不是发给本设备的报文，不响应
        *sendlen = 0;
        return ret;
    }
    
    // 验证CRC
    uint16_t received_crc = (frame[len - 1] << 8) | frame[len - 2];
    uint16_t calculated_crc = modbus_crc16(frame, len - 2);
    
    if (received_crc != calculated_crc) 
    {
        // CRC校验失败，不响应
        *sendlen = 0;
        return ret;
    }
    
    // 根据功能码处理
    uint8_t function_code = frame[1];
    int result = 0;
    
    switch (function_code) {
        case 0x03:  // 读取保持寄存器
            result = process_read_holding_registers(frame, len, sendbuff, sendlen);
            ret = 3;//
            break;
            
        case 0x06:  // 写单个寄存器
            result = process_write_single_register(frame, len, sendbuff, sendlen);
            
            ret = 6;
            break;
            
        case 0x10:  // 写多个寄存器
            result = process_write_multiple_registers(frame, len, sendbuff, sendlen);
            ret = 0x10;
            break;
            
        default:
            // 不支持的函数码
            sendbuff[0] = frame[0];
            sendbuff[1] = function_code | 0x80;  // 设置异常标志
            sendbuff[2] = MODBUS_EX_ILLEGAL_FUNCTION;
            *sendlen = 3;
            result = 1;
            break;
    }
    
    if (!result) 
    {
        // 处理失败
        *sendlen = 0;
    }
    return ret;
}

// 测试代码
#ifdef MODBUS_HANDLER_TEST
#include <stdint.h>

// 定义测试用的参数结构体
EMDCB_Params_t g_emdcb_params = {0};

int modbus_test() {
    // 初始化一些测试数据
    g_emdcb_params.instant_flow = 25.5f;  // 32位浮点数，地址0-1
    g_emdcb_params.flow_unit = 2;         // 16位整数，地址2
    g_emdcb_params.flow_velocity = 1.5f;  // 32位浮点数，地址18-19
    g_emdcb_params.pressure_value = 1.2f; // 32位浮点数，地址72-73
    
    uint8_t send_buffer[256];
    uint16_t send_len = sizeof(send_buffer);
    
    printf("=== Modbus处理函数测试 ===\n");
    
    // 测试1: 读取单个16位寄存器 (地址2, 瞬时流量单位)
    printf("\n测试1: 读取单个16位寄存器 (瞬时流量单位)\n");
    uint8_t read_16bit_frame[] = {
        0x01, 0x03,  // 地址, 功能码
        0x00, 0x02,  // 寄存器地址: 2
        0x00, 0x01,  // 寄存器数量: 1
        0xB9, 0xC9   // CRC
    };
    
    send_len = sizeof(send_buffer);
    modbus_process_frame(read_16bit_frame, sizeof(read_16bit_frame), send_buffer, &send_len);
    printf("响应长度: %u\n", send_len);
    if (send_len > 0) {
        printf("响应数据: ");
        for (int i = 0; i < send_len; i++) {
            printf("%02X ", send_buffer[i]);
        }
        printf("\n");
    }
    
    // 测试2: 读取32位寄存器 (地址0, 瞬时流量)
    printf("\n测试2: 读取32位寄存器 (瞬时流量)\n");
    uint8_t read_32bit_frame[] = {
        0x01, 0x03,
        0x00, 0x00,  // 地址: 0
        0x00, 0x02,  // 数量: 2
        0x84, 0x0A   // CRC
    };
    
    send_len = sizeof(send_buffer);
    modbus_process_frame(read_32bit_frame, sizeof(read_32bit_frame), send_buffer, &send_len);
    printf("响应长度: %u\n", send_len);
    if (send_len > 0) {
        printf("响应数据: ");
        for (int i = 0; i < send_len; i++) {
            printf("%02X ", send_buffer[i]);
        }
        printf("\n");
    }
    
    // 测试3: 读取跨多个寄存器的连续范围 (地址0-2, 共3个寄存器)
    printf("\n测试3: 读取跨多个寄存器的连续范围 (地址0-2)\n");
    uint8_t read_multi_frame[] = {
        0x01, 0x03,
        0x00, 0x00,  // 起始地址: 0
        0x00, 0x03,  // 数量: 3 (包含瞬时流量的2个寄存器和流量单位的1个寄存器)
        0x05, 0xCB   // CRC
    };
    
    send_len = sizeof(send_buffer);
    modbus_process_frame(read_multi_frame, sizeof(read_multi_frame), send_buffer, &send_len);
    printf("响应长度: %u\n", send_len);
    if (send_len > 0) {
        printf("响应数据: ");
        for (int i = 0; i < send_len; i++) {
            printf("%02X ", send_buffer[i]);
        }
        printf("\n");
    }
    
    // 测试4: 读取不连续的寄存器范围 (地址2-19, 共18个寄存器)
    printf("\n测试4: 读取不连续的寄存器范围 (地址2-19)\n");
    uint8_t read_range_frame[] = {
        0x01, 0x03,
        0x00, 0x02,  // 起始地址: 2
        0x00, 0x12,  // 数量: 18 (从地址2到地址19)
        0xE4, 0x0F   // CRC
    };
    
    send_len = sizeof(send_buffer);
    modbus_process_frame(read_range_frame, sizeof(read_range_frame), send_buffer, &send_len);
    printf("响应长度: %u\n", send_len);
    if (send_len > 0) {
        printf("响应数据: ");
        for (int i = 0; i < send_len; i++) {
            printf("%02X ", send_buffer[i]);
        }
        printf("\n");
    }
    
    // 测试5: 写入16位寄存器 (地址2, 瞬时流量单位)
    printf("\n测试5: 写入16位寄存器 (瞬时流量单位)\n");
    uint8_t write_16bit_frame[] = {
        0x01, 0x06,
        0x00, 0x02,  // 地址: 2
        0x00, 0x05,  // 值: 5
        0x99, 0xDE   // CRC
    };
    
    send_len = sizeof(send_buffer);
    modbus_process_frame(write_16bit_frame, sizeof(write_16bit_frame), send_buffer, &send_len);
    printf("响应长度: %u\n", send_len);
    if (send_len > 0) {
        printf("响应数据: ");
        for (int i = 0; i < send_len; i++) {
            printf("%02X ", send_buffer[i]);
        }
        printf("\n");
    }
    
    // 测试6: 写入32位寄存器 (地址0, 瞬时流量)
    printf("\n测试6: 写入32位寄存器 (瞬时流量)\n");
    uint8_t write_32bit_frame[] = {
        0x01, 0x10,
        0x00, 0x00,  // 起始地址: 0
        0x00, 0x02,  // 寄存器数量: 2
        0x04,        // 字节数: 4
        0x41, 0xCC,  // 第一个寄存器值 (浮点数 25.5 的大端表示)
        0xCC, 0xCD,  // 第二个寄存器值
        0x1F, 0x2F   // CRC
    };
    
    send_len = sizeof(send_buffer);
    modbus_process_frame(write_32bit_frame, sizeof(write_32bit_frame), send_buffer, &send_len);
    printf("响应长度: %u\n", send_len);
    if (send_len > 0) {
        printf("响应数据: ");
        for (int i = 0; i < send_len; i++) {
            printf("%02X ", send_buffer[i]);
        }
        printf("\n");
    }
    
    // 测试7: 错误测试 - 读取不存在的地址
    printf("\n测试7: 读取不存在的地址\n");
    uint8_t read_invalid_frame[] = {
        0x01, 0x03,
        0x00, 0xFF,  // 不存在的地址
        0x00, 0x01,  // 数量: 1
        0xD0, 0x0B   // CRC
    };
    
    send_len = sizeof(send_buffer);
    modbus_process_frame(read_invalid_frame, sizeof(read_invalid_frame), send_buffer, &send_len);
    printf("响应长度: %u\n", send_len);
    if (send_len > 0) {
        printf("响应数据: ");
        for (int i = 0; i < send_len; i++) {
            printf("%02X ", send_buffer[i]);
        }
        printf("\n");
    }
    
    // 测试8: 错误测试 - 写入只读寄存器
    printf("\n测试8: 写入只读寄存器\n");
    uint8_t write_readonly_frame[] = {
        0x01, 0x06,
        0x00, 0x00,  // 地址: 0 (瞬时流量, 只读)
        0x12, 0x34,  // 值: 0x1234
        0x8C, 0x1A   // CRC
    };
    
    send_len = sizeof(send_buffer);
    modbus_process_frame(write_readonly_frame, sizeof(write_readonly_frame), send_buffer, &send_len);
    printf("响应长度: %u\n", send_len);
    if (send_len > 0) {
        printf("响应数据: ");
        for (int i = 0; i < send_len; i++) {
            printf("%02X ", send_buffer[i]);
        }
        printf("\n");
    }
    
    printf("\n=== 测试完成 ===\n");
    
    return 0;
}
#endif



void Rs485Rx_Isr_handle(uint8_t* padata ,uint16_t len)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE; // 必须初始化为 pdFALSE
    
    if(len < sizeof(g_rx_buffer))
    {
        memcpy(g_rx_buffer,padata,len);
    }
    g_rx_len = len;
    xSemaphoreGiveFromISR(xRs485Semaphore, &xHigherPriorityTaskWoken);
}

void Modbus_Task_Proc(void const * argument)
{
  int ret = 0;
  //init_index_table();
  //初始化参数
   EMCDB_Flash_Init();
   init_index_table();
   Start_RS485_DMA_Reception(); 

  for(;;)
  {
    // 等待信号量
        if (xSemaphoreTake(xRs485Semaphore, portMAX_DELAY) == pdTRUE) 
        {
            ret = modbus_process_frame(g_rx_buffer, g_rx_len, g_tx_buffer, &g_tx_len);
            if(g_tx_len >0 )
            {
               RS485_Send_Data_DMA(g_tx_buffer, g_tx_len) ;
            }
            //写入了寄存器
            if((0x06 == ret   || 0x10 == ret) && g_tx_len >= 8)
            {
              //等待采样完成再写FLASH
               // if(xSemaphoreTake(xAdcFinishForFlashSemaphore, portMAX_DELAY) == pdTRUE) 
                 // 无限等待，直到LC任务释放锁；一旦获取锁，LC任务将无法执行
                if(xSemaphoreTake(xTask_LC_Mutex, portMAX_DELAY) == pdPASS)
                {
                    EMCDB_Flash_SaveParameters(&g_emdcb_params);
                    // 4. 释放互斥锁，LC任务可继续执行
                    xSemaphoreGive(xTask_LC_Mutex);
                }
            }
        }

  }
}

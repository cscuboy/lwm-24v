#include "in_flash_bsp.h"
#include "string.h"
#include "stdio.h"
#include "calibration.h"

// 全局参数实例
extern EMDCB_Params_t g_emdcb_params;

// CRC16-CCITT计算函数
uint16_t EMCDB_Flash_CalculateCRC(uint8_t* data, uint32_t length) {
    uint16_t crc = PARAM_CRC_INIT;
    
    for (uint32_t i = 0; i < length; i++) {
        crc ^= (uint16_t)data[i] << 8;
        
        for (uint32_t j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc <<= 1;
            }
        }
    }
    
    return crc;
}

// 设置参数默认值
/*
void EMCDB_Flash_SetDefaults(EMDCB_Params_t* params) {
    if (params == NULL) return;
    
    memset(params, 0, sizeof(EMDCB_Params_t));
    
    // 设置合理的默认值
    params->instant_flow = 0.0f;
    params->flow_unit = 5;              // m3/h
    params->meter_caliber = 100;         // 100mm口径
    params->meter_range = 100.0f;        // 100 m3/h量程
    params->modbus_addr = 1;             // Modbus地址1
    params->modbus_baudrate = 115200;      // 波特率9600
    params->flow_direction_set = 0;      // 正向流量
    params->flow_filter_set = 1;         // 滤波使能
    params->empty_pipe_alarm_en = 1;    // 空管报警使能
    params->measure_mode = 0;            // 周期测量模式
    
    //小流量点实际流量值
    params->small_flow_meter  =0.1;
    params->small_flow_actual =0.1;
    //大流量点实际流量值
    params->large_flow_meter = 10.0;
    params->small_flow_actual= 10.0;
    //零点的仪表测量值
    params->zero_point_meter= 0.01;
    
    
    LONG Internal_zero_point; //流量计的零点，adc的滤波值
    FLOAT Internal_K_small; //小流量的K系数
    FLOAT Internal_K_large; //大流量的K系数
}
*/
// 擦除参数存储区域
bool EMCDB_Flash_EraseParamArea(void) {
    FLASH_EraseInitTypeDef erase_init;
    uint32_t page_error = 0;
    HAL_StatusTypeDef status;
    
    // 解锁Flash
    HAL_FLASH_Unlock();
    
    // 清除所有Flash标志
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);
    
    // 配置擦除参数 - STM32L4系列使用Page而不是Sector
    erase_init.TypeErase = FLASH_TYPEERASE_PAGES;
    erase_init.Banks = FLASH_BANK_1;  // L431只有Bank1
    erase_init.Page = PARAM_STORE_START_PAGE;
    erase_init.NbPages = PARAM_STORE_PAGE_COUNT;
    
    // 执行擦除
    status = HAL_FLASHEx_Erase(&erase_init, &page_error);
    
    // 锁定Flash
    HAL_FLASH_Lock();
    
    if (status != HAL_OK) {
        printf("Flash擦除失败! 错误页: %lu, 状态: %d\n", page_error, status);
        return false;
    }
    
    printf("Flash擦除成功，擦除页数: %d\n", PARAM_STORE_PAGE_COUNT);
    return true;
}

// 保存参数到Flash
bool EMCDB_Flash_SaveParameters(EMDCB_Params_t* params) {
    Param_Flash_Header_t header;
    uint32_t flash_addr = PARAM_STORE_ADDR;
    HAL_StatusTypeDef status;
    uint8_t* data_ptr;
    uint32_t data_size;
    
    if (params == NULL) {
        printf("错误: 参数指针为NULL\n");
        return false;
    }
    
    // 准备参数头
    header.valid_flag = PARAM_VALID_FLAG;
    header.version = PARAM_VERSION;
    header.timestamp = HAL_GetTick();
    header.crc16 = EMCDB_Flash_CalculateCRC((uint8_t*)params, sizeof(EMDCB_Params_t));
    memcpy(&header.emdcb_params, params, sizeof(EMDCB_Params_t));
    memset(header.reserved, 0, sizeof(header.reserved));
    
    // 先擦除Flash区域
    if (!EMCDB_Flash_EraseParamArea()) {
        return false;
    }
    
    // 解锁Flash进行写入
    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);
    
    data_ptr = (uint8_t*)&header;
    data_size = sizeof(header);
    
    // STM32L4必须按双字(64位)写入
    for (uint32_t i = 0; i < data_size; i += 8) {
        // 确保地址8字节对齐
        uint32_t aligned_addr = flash_addr + i;
        if (aligned_addr % 8 != 0) {
            aligned_addr = (aligned_addr + 7) & ~0x07; // 向上对齐到8字节
        }
        
        // 提取64位数据
        uint64_t data = 0;
        if (i + 8 <= data_size) {
            memcpy(&data, data_ptr + i, 8);
        } else {
            // 处理末尾不足8字节的情况
            uint8_t temp[8] = {0};
            uint32_t remaining = data_size - i;
            memcpy(temp, data_ptr + i, remaining);
            memcpy(&data, temp, 8);
        }
        
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, aligned_addr, data);
        
        if (status != HAL_OK) {
            printf("Flash写入失败在地址: 0x%08lX, 状态: %d\n", aligned_addr, status);
            HAL_FLASH_Lock();
            return false;
        }
    }
    
    HAL_FLASH_Lock();
    
    printf("参数保存成功! CRC: 0x%04X, 时间戳: %lu\n", header.crc16, header.timestamp);
    return true;
}

// 从Flash加载参数
bool EMCDB_Flash_LoadParameters(EMDCB_Params_t* params) {
    Param_Flash_Header_t* header = (Param_Flash_Header_t*)PARAM_STORE_ADDR;
    uint16_t calculated_crc;
    
    if (params == NULL) {
        printf("错误: 参数指针为NULL\n");
        return false;
    }
    
    // 检查有效标志
    if (header->valid_flag != PARAM_VALID_FLAG) {
        printf("参数无效: 标志不匹配 (期望: 0x%08lX, 实际: 0x%08lX)\n", 
               PARAM_VALID_FLAG, header->valid_flag);
        return false;
    }
    
    // 检查版本
    if (header->version != PARAM_VERSION) {
        printf("参数版本不匹配 (期望: %d, 实际: %d)\n", 
               PARAM_VERSION, header->version);
        // 可以在这里添加版本迁移逻辑
    }
    
    // 计算并验证CRC
    calculated_crc = EMCDB_Flash_CalculateCRC((uint8_t*)&header->emdcb_params, 
                                            sizeof(EMDCB_Params_t));
    
    if (calculated_crc != header->crc16) {
        printf("CRC校验失败! (期望: 0x%04X, 实际: 0x%04X)\n", 
               header->crc16, calculated_crc);
        return false;
    }
    
    // 拷贝参数数据
    memcpy(params, &header->emdcb_params, sizeof(EMDCB_Params_t));
    
    printf("参数加载成功! 版本: %d, 时间戳: %lu\n", 
           header->version, header->timestamp);
    return true;
}

// 验证参数完整性
bool EMCDB_Flash_VerifyParameters(void) {
    Param_Flash_Header_t* header = (Param_Flash_Header_t*)PARAM_STORE_ADDR;
    uint16_t calculated_crc;
    
    if (header->valid_flag != PARAM_VALID_FLAG) {
        return false;
    }
    
    calculated_crc = EMCDB_Flash_CalculateCRC((uint8_t*)&header->emdcb_params, 
                                            sizeof(EMDCB_Params_t));
    return (calculated_crc == header->crc16);
}

// 初始化Flash参数系统
bool EMCDB_Flash_Init(void)
{
  bool ret;
  
    dprintf("=== EMDCB Flash参数系统初始化 ===\n");
    dprintf("参数存储地址: 0x%08lX\n", PARAM_STORE_ADDR);
    dprintf("参数存储大小: %lu页 (%lu字节)\n", 
           PARAM_STORE_PAGE_COUNT, PARAM_STORE_PAGE_COUNT * FLASH_PAGE_SIZE);
    
    // 尝试加载参数
    if (EMCDB_Flash_LoadParameters(&g_emdcb_params)) 
    {
        printf("参数加载成功\n");
        ret =  true;
    } 
    else 
    {
        dprintf("参数加载失败，使用默认值\n");
        
       // EMCDB_Flash_SetDefaults(&g_emdcb_params);
        emdcb_init_default_params(&g_emdcb_params);
        // 保存默认值
        if (EMCDB_Flash_SaveParameters(&g_emdcb_params))
        {
            dprintf("默认参数保存成功\n");
            ret =  true;
        } else {
            dprintf("默认参数保存失败\n");
            ret = false;
        }
    }
      
        
    //初始化校准系数
    Init_Calibration_Param(&g_emdcb_params);
    
    return ret;

}


#ifndef IN_FLASH_PARAMS_H
#define IN_FLASH_PARAMS_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "stm32l4xx_hal.h"        // 主HAL头文件
#include "stm32l4xx_hal_flash.h"  // Flash基本操作
#include "stm32l4xx_hal_flash_ex.h" // Flash扩展功能（包含FLASH_TYPEERASE_SECTORS）
#include "Modbus_index.h"
#include "Modbus_Param.h"


// Flash配置
#define FLASH_BASE_ADDR         0x08000000
//#define FLASH_PAGE_SIZE         2048        // L4系列每页2KB
#define FLASH_TOTAL_SIZE        (128 * 1024) // STM32L433RC有128KB Flash

// 参数存储区域定义（使用最后8页，避免与程序代码冲突）
#define PARAM_STORE_START_PAGE  120         // 从第120页开始
#define PARAM_STORE_PAGE_COUNT  8           // 使用8页（16KB）
#define PARAM_STORE_ADDR        (FLASH_BASE_ADDR + (PARAM_STORE_START_PAGE * FLASH_PAGE_SIZE))

// 参数标志和版本
#define PARAM_VALID_FLAG        0xAA55AA55
#define PARAM_VERSION           1
#define PARAM_CRC_INIT          0xFFFF

// 参数存储头结构
#pragma pack(push, 1)
typedef struct {
    uint32_t valid_flag;        // 参数有效标志
    uint16_t version;           // 参数版本号
    uint32_t timestamp;         // 存储时间戳
    uint16_t crc16;             // CRC16校验值
    uint8_t reserved[6];        // 保留字段（对齐到8字节）
    EMDCB_Params_t emdcb_params; // 实际的参数数据
} Param_Flash_Header_t;
#pragma pack(pop)

// 函数声明
bool EMCDB_Flash_Init(void);
bool EMCDB_Flash_SaveParameters(EMDCB_Params_t* params);
bool EMCDB_Flash_LoadParameters(EMDCB_Params_t* params);
bool EMCDB_Flash_EraseParamArea(void);
void EMCDB_Flash_SetDefaults(EMDCB_Params_t* params);
uint16_t EMCDB_Flash_CalculateCRC(uint8_t* data, uint32_t length);
bool EMCDB_Flash_VerifyParameters(void);

// 全局参数实例
extern EMDCB_Params_t g_emdcb_params;

#endif // EMDCB_FLASH_STORE_H

#ifndef __SSD1315_H
#define __SSD1315_H

#include "main.h"
#include "i2c.h"

// I2C地址定义 (根据SA0引脚电平)
#define SSD1315_I2C_ADDR_SA0_LOW    0x78  // SA0 = 0
#define SSD1315_I2C_ADDR_SA0_HIGH   0x7A  // SA0 = 1

// 默认使用SA0低电平地址
#define SSD1315_I2C_ADDR SSD1315_I2C_ADDR_SA0_LOW

// 控制字节定义
#define SSD1315_CONTROL_BYTE_CMD     0x00  // 命令
#define SSD1315_CONTROL_BYTE_DATA    0x40  // 数据

// 基本命令定义
#define SSD1315_CMD_DISPLAY_OFF      0xAE
#define SSD1315_CMD_DISPLAY_ON       0xAF
#define SSD1315_CMD_SET_DISP_START_LINE 0x40
#define SSD1315_CMD_SET_PAGE_ADDR    0xB0
#define SSD1315_CMD_SET_COL_ADDR_H   0x10
#define SSD1315_CMD_SET_COL_ADDR_L   0x00
#define SSD1315_CMD_SET_CONTRAST     0x81
#define SSD1315_CMD_SEG_REMAP        0xA0  // 0xA0: normal, 0xA1: remap
#define SSD1315_CMD_SET_ENTIRE_ON    0xA4
#define SSD1315_CMD_SET_NORM_DISP    0xA6
#define SSD1315_CMD_SET_MUX_RATIO    0xA8
#define SSD1315_CMD_SET_COM_OUT_DIR  0xC0  // 0xC0: normal, 0xC8: remap
#define SSD1315_CMD_SET_DISP_OFFSET  0xD3
#define SSD1315_CMD_SET_DISP_CLK_DIV 0xD5
#define SSD1315_CMD_SET_PRECHARGE    0xD9
#define SSD1315_CMD_SET_COM_PINS     0xDA
#define SSD1315_CMD_SET_VCOM_DESEL   0xDB
#define SSD1315_CMD_SET_CHARGE_PUMP  0x8D

// 显示尺寸定义
#define SSD1315_WIDTH       128
#define SSD1315_HEIGHT      64
#define SSD1315_PAGES       8       // 64/8 = 8页

// 函数声明
void SSD1315_Init(void);
void SSD1315_WriteCommand(uint8_t cmd);
void SSD1315_WriteData(uint8_t data);
void SSD1315_Clear(void);
void SSD1315_Update(void);
void SSD1315_SetCursor(uint8_t page, uint8_t col);
void SSD1315_PutChar(uint8_t page, uint8_t col, char ch);
void SSD1315_PrintString(uint8_t page, uint8_t col, const char *str);
void SSD1315_DrawPixel(uint8_t x, uint8_t y, uint8_t color);
void SSD1315_DisplayOff(void);
void SSD1315_DisplayOn(void);
void SSD1315_SetRotation(uint8_t rotation);
void SSD1315_SetDisplayDirection(uint8_t flip_horizontal, uint8_t flip_vertical);


#endif /* __SSD1315_H */
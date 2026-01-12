#include "lcd_ssd1315.h"
#include "font_8x16.h"  // 8x16字体文件
#include <string.h>
#include "main.h"
#include "i2c.h"
// 显示缓冲区 (128x64 bits = 128x8 bytes)
static uint8_t SSD1315_Buffer[SSD1315_PAGES][SSD1315_WIDTH];

// I2C发送函数
static HAL_StatusTypeDef SSD1315_I2C_Write(uint8_t *data, uint16_t size) {
    return HAL_I2C_Master_Transmit(&hi2c2, SSD1315_I2C_ADDR, data, size, HAL_MAX_DELAY);
}

// 写命令函数
void SSD1315_WriteCommand(uint8_t cmd) {
    uint8_t buffer[2] = {SSD1315_CONTROL_BYTE_CMD, cmd};
    SSD1315_I2C_Write(buffer, 2);
}

// 写数据函数
void SSD1315_WriteData(uint8_t data) {
    uint8_t buffer[2] = {SSD1315_CONTROL_BYTE_DATA, data};
    SSD1315_I2C_Write(buffer, 2);
}

// 初始化函数（严格按照文档8.1节的上电序列）
void SSD1315_Init(void) {
    // 1. 硬件复位（如果RES#引脚由MCU控制）
    // HAL_GPIO_WritePin(OLED_RESET_GPIO_Port, OLED_RESET_Pin, GPIO_PIN_RESET);
    // HAL_Delay(10);
    // HAL_GPIO_WritePin(OLED_RESET_GPIO_Port, OLED_RESET_Pin, GPIO_PIN_SET);
    // HAL_Delay(100);  // 等待复位完成
    OLED_RESET(IO_ON);
    HAL_Delay(10);
    OLED_RESET(IO_OFF);
    HAL_Delay(100);  // 等待复位完成

    // 2. 关闭显示
    SSD1315_WriteCommand(SSD1315_CMD_DISPLAY_OFF);
    
    // 3. 设置时钟分频和振荡器频率
    SSD1315_WriteCommand(SSD1315_CMD_SET_DISP_CLK_DIV);
    SSD1315_WriteCommand(0x80);  // 默认值
    
    // 4. 设置多路复用比率
    SSD1315_WriteCommand(SSD1315_CMD_SET_MUX_RATIO);
    SSD1315_WriteCommand(0x3F);  // 64MUX
    
    // 5. 设置显示偏移
    SSD1315_WriteCommand(SSD1315_CMD_SET_DISP_OFFSET);
    SSD1315_WriteCommand(0x00);  // 无偏移
    
    // 6. 设置显示起始行
    SSD1315_WriteCommand(SSD1315_CMD_SET_DISP_START_LINE | 0x00);
    
    // 7. 启用电荷泵（关键步骤）
    SSD1315_WriteCommand(SSD1315_CMD_SET_CHARGE_PUMP);
    SSD1315_WriteCommand(0x14);  // 启用电荷泵
    
    // 8. 设置内存地址模式
    SSD1315_WriteCommand(0x20);   // 设置内存模式
    SSD1315_WriteCommand(0x00);   // 水平地址模式
    
        // 设置显示方向 - 根据您的硬件连接选择
    SSD1315_WriteCommand(0xA0);  // 段重映射：正常方向
    SSD1315_WriteCommand(0xC8);  // COM扫描方向：正常方向
    
    
    // 9. 段重映射
    SSD1315_WriteCommand(0xA1);   // 段重映射 (A0-A1)
    
    // 10. COM输出扫描方向
    SSD1315_WriteCommand(0xC8);   // 从COM63到COM0
    
    // 11. 设置COM引脚硬件配置
    SSD1315_WriteCommand(SSD1315_CMD_SET_COM_PINS);
    SSD1315_WriteCommand(0x12);   // 顺序COM引脚配置
    
    // 12. 设置对比度
    SSD1315_WriteCommand(SSD1315_CMD_SET_CONTRAST);
    SSD1315_WriteCommand(0xCF);   // 对比度值
    
    // 13. 设置预充电周期
    SSD1315_WriteCommand(SSD1315_CMD_SET_PRECHARGE);
    SSD1315_WriteCommand(0xF1);   // 预充电周期
    
    // 14. 设置VCOMH电压
    SSD1315_WriteCommand(SSD1315_CMD_SET_VCOM_DESEL);
    SSD1315_WriteCommand(0x40);   // ~0.77 x VCC
    
    // 15. 整个显示打开
    SSD1315_WriteCommand(0xA4);   // 整个显示打开
    
    // 16. 设置正常显示（非反色）
    SSD1315_WriteCommand(SSD1315_CMD_SET_NORM_DISP);
    
    // 17. 去抖动设置
    SSD1315_WriteCommand(0xD6);   // 去抖动模式设置
    SSD1315_WriteCommand(0xA1);   // 自动去抖动
    
    // 18. 打开显示
    SSD1315_WriteCommand(SSD1315_CMD_DISPLAY_ON);
    
    // 19. 清空显示缓冲区
    SSD1315_Clear();
}

// 清屏函数
void SSD1315_Clear(void) {
    memset(SSD1315_Buffer, 0, sizeof(SSD1315_Buffer));
    SSD1315_Update();
}

// 更新整个显示
void SSD1315_Update(void) {
    for (uint8_t page = 0; page < SSD1315_PAGES; page++) 
    {
        // 设置页地址
        SSD1315_WriteCommand(SSD1315_CMD_SET_PAGE_ADDR | page);
        // 设置列地址低4位
        SSD1315_WriteCommand(SSD1315_CMD_SET_COL_ADDR_L | 0);
        // 设置列地址高4位
        SSD1315_WriteCommand(SSD1315_CMD_SET_COL_ADDR_H | 0);
        
        // 发送该页的所有数据
        for (uint8_t col = 0; col < SSD1315_WIDTH; col++) 
        {
            SSD1315_WriteData(SSD1315_Buffer[page][col]);
        }
    }
}

// 设置光标位置
void SSD1315_SetCursor(uint8_t page, uint8_t col) {
    if (page >= SSD1315_PAGES) page = SSD1315_PAGES - 1;
    if (col >= SSD1315_WIDTH) col = SSD1315_WIDTH - 1;
    
    SSD1315_WriteCommand(SSD1315_CMD_SET_PAGE_ADDR | page);
    SSD1315_WriteCommand(SSD1315_CMD_SET_COL_ADDR_L | (col & 0x0F));
    SSD1315_WriteCommand(SSD1315_CMD_SET_COL_ADDR_H | ((col >> 4) & 0x0F));
}
/*
// 显示单个字符（使用8x16字体）
void SSD1315_PutChar(uint8_t page, uint8_t col, char ch) {
    if (page >= SSD1315_PAGES || col >= SSD1315_WIDTH) return;
    
    // 检查字符是否在字体范围内
    if (ch < 32 || ch > 127) ch = ' ';  // 替换为空格
    
    // 获取字符点阵数据（每个字符16字节）
    const uint8_t *font_data = font_8x16[(ch - 32) * 16];  // 直接获取字符数据指针
    
    // 绘制字符到缓冲区（每个字符占用2页高度）
    for (uint8_t i = 0; i < 16; i++) {
        if (col + i/8 < SSD1315_WIDTH) {
            if (i < 8) {
                // 上半部分（第一页）
                SSD1315_Buffer[page][col + i/8] |= (font_data[i] << (i % 8));
            } else {
                // 下半部分（第二页）
                if (page + 1 < SSD1315_PAGES) {
                    SSD1315_Buffer[page + 1][col + (i-8)/8] |= (font_data[i] << ((i-8) % 8));
                }
            }
        }
    }
}*/

// 显示字符串
/*
void SSD1315_PrintString(uint8_t page, uint8_t col, const char *str) {
    while (*str && col < SSD1315_WIDTH) {
        SSD1315_PutChar(page, col, *str++);
        col += 2;  // 字符宽度+间距
    }
    SSD1315_Update();
}*/

/**
 * @brief  在指定位置显示一个字符（8x16字体）
 * @param  page: 页地址（0-7），每页8行像素
 * @param  col: 列地址（0-127）
 * @param  ch: 要显示的字符
 * @retval 无
 */
void SSD1315_PutChar(uint8_t page, uint8_t col, char ch)
{
    if (page >= SSD1315_PAGES || col >= SSD1315_WIDTH) 
        return;
    
    // 检查字符是否在可显示范围内（ASCII 32-127）
    if (ch < 32 || ch > 127) 
        ch = ' ';  // 替换为空格
    
    // 获取字符在字体数组中的索引
    uint8_t char_index = ch - 32;
    
    // 确保不越界
    if (char_index >= 96) 
        char_index = 0;  // 空格字符
    
    // 获取字符点阵数据指针
    const uint8_t *font_data = &font_8x16[char_index][0];

    // 显示字符（每个字符16字节，分两页显示）
    for (uint8_t i = 0; i < 16; i++) 
    {
        if (col < SSD1315_WIDTH)
        {
            if (i < 8) 
            {
                // 字符上半部分（第一页）
                SSD1315_Buffer[page][col] = font_data[i];
            } 
            else 
            {
                // 字符下半部分（第二页）
                if (page + 1 < SSD1315_PAGES) 
                {
                    SSD1315_Buffer[page + 1][col-8] = font_data[i];
                }
            }
            col++;  // 移动到下一列
        }
    }
}

/**
 * @brief  显示字符串
 * @param  page: 起始页（0-7）
 * @param  col: 起始列（0-127）
 * @param  str: 要显示的字符串
 * @retval 无
 */
void SSD1315_PrintString(uint8_t page, uint8_t col, const char *str)
{
    uint8_t current_page = page*2;
    uint8_t current_col = col;
    
    while (*str != '\0' && current_col < SSD1315_WIDTH) {
        // 处理换行符
        if (*str == '\n') {
            current_page += 2;  // 8x16字体占用2页高度
            current_col = col;  // 回到起始列
            str++;
            continue;
        }
        
        // 检查是否需要换页
        if (current_col + 8 >= SSD1315_WIDTH) {
            current_page += 2;
            current_col = col;
        }
        
        // 检查页边界
        if (current_page >= SSD1315_PAGES) {
            break;  // 超出屏幕范围
        }
        
        // 显示字符
        SSD1315_PutChar(current_page, current_col, *str);
        
        // 更新位置（字符宽度8像素 + 字间距1像素）
        current_col += 9;
        str++;
    }
    

}



// 绘制像素点
void SSD1315_DrawPixel(uint8_t x, uint8_t y, uint8_t color) {
    if (x >= SSD1315_WIDTH || y >= SSD1315_HEIGHT) return;
    
    uint8_t page = y / 8;
    uint8_t bit = y % 8;
    
    if (color) {
        SSD1315_Buffer[page][x] |= (1 << bit);
    } else {
        SSD1315_Buffer[page][x] &= ~(1 << bit);
    }
}



/**
 * @brief  旋转屏幕显示
 * @param  rotation: 旋转角度（0°, 90°, 180°, 270°）
 * @retval 无
 */
void SSD1315_SetRotation(uint8_t rotation)
{
    switch (rotation) {
        case 0:   // 0° - 正常
            SSD1315_WriteCommand(0xA0);  // 正常水平
            SSD1315_WriteCommand(0xC8);  // 正常垂直
            break;
            
        case 90:  // 90° - 需要软件坐标转换
            // 硬件方向设置
            SSD1315_WriteCommand(0xA0);
            SSD1315_WriteCommand(0xC8);
            // 注意：90°和270°旋转需要软件处理坐标变换
            break;
            
        case 180: // 180°
            SSD1315_WriteCommand(0xA1);  // 水平翻转
            SSD1315_WriteCommand(0xC0);  // 垂直翻转
            break;
            
        case 270: // 270° - 需要软件坐标转换
            SSD1315_WriteCommand(0xA1);
            SSD1315_WriteCommand(0xC0);
            break;
            
        default:
            // 默认正常方向
            SSD1315_WriteCommand(0xA0);
            SSD1315_WriteCommand(0xC8);
            break;
    }
}

/**
 * @brief  设置OLED显示方向
 * @param  flip_horizontal: 0-正常，1-水平翻转
 * @param  flip_vertical: 0-正常，1-垂直翻转
 * @retval 无
 */
void SSD1315_SetDisplayDirection(uint8_t flip_horizontal, uint8_t flip_vertical)
{
    // 设置段重映射（左右方向）
    if (flip_horizontal) {
        SSD1315_WriteCommand(0xA1);  // 左右翻转
    } else {
        SSD1315_WriteCommand(0xA0);  // 正常方向
    }
    
    // 设置COM输出扫描方向（上下方向）
    if (flip_vertical) {
        SSD1315_WriteCommand(0xC0);  // 上下翻转
    } else {
        SSD1315_WriteCommand(0xC8);  // 正常方向
    }
}
// 关闭显示
void SSD1315_DisplayOff(void) {
    SSD1315_WriteCommand(SSD1315_CMD_DISPLAY_OFF);
}

// 打开显示
void SSD1315_DisplayOn(void) {
    SSD1315_WriteCommand(SSD1315_CMD_DISPLAY_ON);
}
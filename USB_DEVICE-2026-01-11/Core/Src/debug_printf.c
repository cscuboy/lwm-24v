#include "debug_printf.h"
#include "usbd_cdc_if.h"

// 定义发送缓冲区大小（根据您的USB端点缓冲区大小调整）
#define DEBUG_PRINTF_BUFFER_SIZE 512

int dprintf(const char *format, ...)
{
    va_list args;
    char buffer[DEBUG_PRINTF_BUFFER_SIZE];
    int len;
    
    // 使用可变参数处理格式化字符串
    va_start(args, format);
    len = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    // 检查格式化是否成功且长度未超过缓冲区
    if (len < 0) {
        return len; // 格式化错误
    }
    
    if ((size_t)len >= sizeof(buffer)) {
        len = sizeof(buffer) - 1; // 截断过长的内容
        buffer[len] = '\0'; // 确保字符串终止
    }
    
    // 使用USB CDC发送数据
    uint8_t result = CDC_Transmit_FS((uint8_t*)buffer, (uint16_t)len);
    
    // 处理发送繁忙的情况（可选：添加重试机制）
    if (result == USBD_BUSY) {
        // 可以在这里添加重试逻辑或简单的延时
        HAL_Delay(1);
        result = CDC_Transmit_FS((uint8_t*)buffer, (uint16_t)len);
    }
    
    return (result == USBD_OK) ? len : -1;
}
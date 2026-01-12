#ifndef __DEBUG_PRINTF_H
#define __DEBUG_PRINTF_H

#include "main.h"
//#include "usbd_cdc_if.h" // 确保包含CDC头文件
#include <stdarg.h>      // 用于可变参数处理
#include <stdio.h>       // 用于vsnprintf

// 调试级别控制
#define DEBUG_LEVEL_ERROR    0
#define DEBUG_LEVEL_WARNING  1
#define DEBUG_LEVEL_INFO     2
#define DEBUG_LEVEL_DEBUG    3

// 设置当前调试级别，可在开发过程中调整
#ifndef CURRENT_DEBUG_LEVEL
#define CURRENT_DEBUG_LEVEL DEBUG_LEVEL_DEBUG
#endif

// 核心dprintf函数声明
int dprintf(const char *format, ...);

// 带调试级别的打印宏（推荐使用）
#define DP_ERROR(fmt, ...)   do { if (CURRENT_DEBUG_LEVEL >= DEBUG_LEVEL_ERROR)   dprintf("[ERROR] " fmt, ##__VA_ARGS__); } while (0)
#define DP_WARN(fmt, ...)    do { if (CURRENT_DEBUG_LEVEL >= DEBUG_LEVEL_WARNING) dprintf("[WARN] " fmt, ##__VA_ARGS__); } while (0)
#define DP_INFO(fmt, ...)    do { if (CURRENT_DEBUG_LEVEL >= DEBUG_LEVEL_INFO)    dprintf("[INFO] " fmt, ##__VA_ARGS__); } while (0)
#define DP_DEBUG(fmt, ...)   do { if (CURRENT_DEBUG_LEVEL >= DEBUG_LEVEL_DEBUG)   dprintf("[DEBUG] " fmt, ##__VA_ARGS__); } while (0)

// 简化的无条件打印宏
#define DP(fmt, ...)         dprintf(fmt, ##__VA_ARGS__)

#endif /* __DEBUG_PRINTF_H */
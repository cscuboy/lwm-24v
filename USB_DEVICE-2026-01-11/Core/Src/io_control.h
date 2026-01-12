#ifndef __IO_CONTROL_H
#define __IO_CONTROL_H

#include "main.h"  // 包含STM32 HAL库定义
#include "gpio.h"

// 控制状态枚举
typedef enum {
    IO_OFF = 0,
    IO_ON = 1
} IO_State;

// 引脚状态读取枚举
typedef enum {
    PIN_RESET = 0,
    PIN_SET = 1
} PinState;

/* ===== GPIOE 引脚控制函数 ===== */
void LC_POW_EN(IO_State state);
void COIL_EN1(IO_State state);
void COIL_EN2(IO_State state);
void POWER_2_5(IO_State state);
void AD_POW_EN(IO_State state);

/* ===== GPIOC 引脚控制函数 ===== */
void CL_POW_EN(IO_State state);
void BT_MODE(IO_State state);
void BT_RESTORE(IO_State state);
void BT_WAKE(IO_State state);

/* ===== GPIOD 引脚控制函数 ===== */
void BT_RESET(IO_State state);
void AC_EN(IO_State state);
void BD_EN(IO_State state);
void LED_BT(IO_State state);
void LED_RUN(IO_State state);      // 您特别要求的运行指示灯
void LED_LC(IO_State state);
void WDI(IO_State state);
void FLASH_HOLD(IO_State state);
void FLASH_WP(IO_State state);

/* ===== GPIOA 引脚控制函数 ===== */
void RS485_RE(IO_State state);     // RS485接收使能
void RS485_DE(IO_State state);     // RS485发送使能

/* ===== GPIOB 引脚控制函数 ===== */
void OLED_RESET(IO_State state);
void OLED_VDD_EN(IO_State state);

/* ===== 输入引脚读取函数 ===== */
PinState READ_T_COIL_B1(void);
PinState READ_T_COIL_B2(void);
PinState READ_T_COIL(void);
PinState READ_BT_LINK(void);
PinState READ_ALARM_4_20MA(void);
PinState READ_BT_READY(void);
PinState READ_KEY(void);

/* ===== 高级功能函数 ===== */
void LED_RUN_Toggle(void);         // LED运行指示灯翻转
void All_LEDs_On(void);            // 所有LED点亮
void All_LEDs_Off(void);           // 所有LED熄灭


void Power_IO_All_Off(uint16_t reserved_pins);          // 关闭所有电源控制
void Power_Down_All_Modules(uint16_t reserved_pins);    //分阶段关闭所有外设电源


#endif /* __IO_CONTROL_H */
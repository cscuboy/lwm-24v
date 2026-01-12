#include "io_control.h"

// 可选的配置宏，允许您选择性地关闭某些模块
#define POWERDOWN_LEDS     1
#define POWERDOWN_COMM     1
#define POWERDOWN_BT       1
#define POWERDOWN_SENSORS  1



/* ===== GPIOE 引脚控制函数实现 ===== */
void LC_POW_EN(IO_State state) {
    HAL_GPIO_WritePin(LC_POW_EN_GPIO_Port, LC_POW_EN_Pin, 
                     (state == IO_ON) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void COIL_EN1(IO_State state) {
    HAL_GPIO_WritePin(COIL_EN1_GPIO_Port, COIL_EN1_Pin, 
                     (state == IO_ON) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void COIL_EN2(IO_State state) {
    HAL_GPIO_WritePin(COIL_EN2_GPIO_Port, COIL_EN2_Pin, 
                     (state == IO_ON) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void POWER_2_5(IO_State state) {
    HAL_GPIO_WritePin(POWER_2_5_GPIO_Port, POWER_2_5_Pin, 
                     (state == IO_ON) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void AD_POW_EN(IO_State state) {
    HAL_GPIO_WritePin(AD_POW_EN_GPIO_Port, AD_POW_EN_Pin, 
                     (state == IO_ON) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}


void R10M_EN(IO_State state) {
    HAL_GPIO_WritePin(PWM_LC3_GPIO_Port, PWM_LC3_Pin, 
                     (state == IO_ON) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/* ===== GPIOC 引脚控制函数实现 ===== */
void CL_POW_EN(IO_State state) {
    HAL_GPIO_WritePin(CL_POW_EN_GPIO_Port, CL_POW_EN_Pin, 
                     (state == IO_ON) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void BT_MODE(IO_State state) {
    HAL_GPIO_WritePin(BT_MODE_GPIO_Port, BT_MODE_Pin, 
                     (state == IO_ON) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void BT_RESTORE(IO_State state) {
    HAL_GPIO_WritePin(BT_RESTORE_GPIO_Port, BT_RESTORE_Pin, 
                     (state == IO_ON) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void BT_WAKE(IO_State state) {
    HAL_GPIO_WritePin(BT_WAKE_GPIO_Port, BT_WAKE_Pin, 
                     (state == IO_ON) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/* ===== GPIOD 引脚控制函数实现 ===== */
void BT_RESET(IO_State state) {
    HAL_GPIO_WritePin(BT_RESET_GPIO_Port, BT_RESET_Pin, 
                     (state == IO_ON) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void AC_EN(IO_State state) {
    HAL_GPIO_WritePin(AC_EN_GPIO_Port, AC_EN_Pin, 
                     (state == IO_ON) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void BD_EN(IO_State state) {
    HAL_GPIO_WritePin(BD_EN_GPIO_Port, BD_EN_Pin, 
                     (state == IO_ON) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void LED_BT(IO_State state) {
    HAL_GPIO_WritePin(LED_BT_GPIO_Port, LED_BT_Pin, 
                     (state == IO_ON) ? GPIO_PIN_RESET  : GPIO_PIN_SET);
}

// 您特别要求的运行指示灯控制函数
void LED_RUN(IO_State state) {
    HAL_GPIO_WritePin(LED_RUN_GPIO_Port, LED_RUN_Pin, 
                     (state == IO_ON) ? GPIO_PIN_RESET  : GPIO_PIN_SET);
}

void LED_LC(IO_State state) {
    HAL_GPIO_WritePin(LED_LC_GPIO_Port, LED_LC_Pin, 
                     (state == IO_ON) ? GPIO_PIN_RESET  : GPIO_PIN_SET);
}

void WDI(IO_State state) {
    HAL_GPIO_WritePin(WDI_GPIO_Port, WDI_Pin, 
                     (state == IO_ON) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void FLASH_HOLD(IO_State state) {
    HAL_GPIO_WritePin(FLASH_HOLD_GPIO_Port, FLASH_HOLD_Pin, 
                     (state == IO_ON) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void FLASH_WP(IO_State state) {
    HAL_GPIO_WritePin(FLASH_WP_GPIO_Port, FLASH_WP_Pin, 
                     (state == IO_ON) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/* ===== GPIOA 引脚控制函数实现 ===== */
void RS485_RE(IO_State state) {
    HAL_GPIO_WritePin(RS485_RE_GPIO_Port, RS485_RE_Pin, 
                     (state == IO_ON) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void RS485_DE(IO_State state) {
    HAL_GPIO_WritePin(RS_485_DE_GPIO_Port, RS_485_DE_Pin, 
                     (state == IO_ON) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}



/* ===== GPIOB 引脚控制函数实现 ===== */
void OLED_RESET(IO_State state) {
    HAL_GPIO_WritePin(OLED_RESET_GPIO_Port, OLED_RESET_Pin, 
                     (state == IO_ON) ? GPIO_PIN_RESET  : GPIO_PIN_SET);
}

void OLED_VDD_EN(IO_State state) {
    HAL_GPIO_WritePin(EN_LED_VDD_GPIO_Port, EN_LED_VDD_Pin, 
                     (state == IO_ON) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/* ===== 输入引脚读取函数实现 ===== */
PinState READ_T_COIL_B1(void) {
    return (HAL_GPIO_ReadPin(T_COIL_B1_GPIO_Port, T_COIL_B1_Pin) == GPIO_PIN_SET) ? PIN_SET : PIN_RESET;
}

PinState READ_T_COIL_B2(void) {
    return (HAL_GPIO_ReadPin(T_COIL_B2_GPIO_Port, T_COIL_B2_Pin) == GPIO_PIN_SET) ? PIN_SET : PIN_RESET;
}

PinState READ_T_COIL(void) {
    return (HAL_GPIO_ReadPin(T_COIL_GPIO_Port, T_COIL_Pin) == GPIO_PIN_SET) ? PIN_SET : PIN_RESET;
}

PinState READ_BT_LINK(void) {
    return (HAL_GPIO_ReadPin(BT_LINK_GPIO_Port, BT_LINK_Pin) == GPIO_PIN_SET) ? PIN_SET : PIN_RESET;
}

PinState READ_ALARM_4_20MA(void) {
    return (HAL_GPIO_ReadPin(ALARM_4_20MA_GPIO_Port, ALARM_4_20MA_Pin) == GPIO_PIN_SET) ? PIN_SET : PIN_RESET;
}

PinState READ_BT_READY(void) {
    return (HAL_GPIO_ReadPin(BT_READY_GPIO_Port, BT_READY_Pin) == GPIO_PIN_SET) ? PIN_SET : PIN_RESET;
}

PinState READ_KEY(void) {
    return (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_SET) ? PIN_SET : PIN_RESET;
}

/* ===== 高级功能函数实现 ===== */
void LED_RUN_Toggle(void) {
    HAL_GPIO_TogglePin(LED_RUN_GPIO_Port, LED_RUN_Pin);
}

void All_LEDs_On(void) {
    LED_BT(IO_ON);
    LED_RUN(IO_ON);
    LED_LC(IO_ON);
    //OLED_RESET(IO_ON);
    //OLED_VDD_EN(IO_ON);
}

void All_LEDs_Off(void) {
    LED_BT(IO_OFF);
    LED_RUN(IO_OFF);
    LED_LC(IO_OFF);
    //OLED_RESET(IO_OFF);
    //OLED_VDD_EN(IO_OFF);
}

void RS485_TX(IO_State state)
{
    if(state == IO_ON)
    {
        RS485_DE(IO_OFF);
        RS485_RE(IO_ON);  
    }
    else
    {
       RS485_DE(IO_ON );
        RS485_RE(IO_OFF);
    }
}
/*
void Power_IO_All_Off(void) 
{
    LC_POW_EN(IO_OFF);
    CL_POW_EN(IO_OFF);
    POWER_2_5(IO_OFF);
    AD_POW_EN(IO_OFF);
    AC_EN(IO_OFF);
    BD_EN(IO_OFF);
}*/

/**
  * @brief  关闭所有未使用的GPIO以进入低功耗模式（停机/待机模式）
  * @note   此函数会将所有IO口配置为模拟输入模式，这是最省电的状态。
  *         但必须保留唤醒引脚（如WKUP、RTC等）的配置，否则无法唤醒。
  * @param  保留的唤醒引脚掩码（例如：GPIO_PIN_13 表示保留PC13）
  * @retval 无
  */
void Power_IO_All_Off(uint16_t reserved_pins)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  /* 1. 使能所有GPIO时钟（为了能够配置它们）*/
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  // 根据您的芯片型号，可能还需要使能GPIOF、GPIOG等
  
  /* 2. 配置所有GPIO为模拟输入模式（最省电）*/
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;   // 模拟输入模式
  GPIO_InitStruct.Pull = GPIO_NOPULL;         // 关闭上拉下拉电阻
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // 低速
  
  /* 配置GPIOA（保留唤醒引脚，如PA0-WKUP） */
  GPIO_InitStruct.Pin = GPIO_PIN_All & (~reserved_pins); // 排除保留引脚
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  /* 配置GPIOB */
  GPIO_InitStruct.Pin = GPIO_PIN_All;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  /* 配置GPIOC（如果PC13用于唤醒，则需要保留） */
  GPIO_InitStruct.Pin = GPIO_PIN_All & (~reserved_pins);
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
  /* 配置GPIOD */
  GPIO_InitStruct.Pin = GPIO_PIN_All;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  
  /* 配置GPIOE */
  GPIO_InitStruct.Pin = GPIO_PIN_All;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  
  /* 3. 关闭GPIO时钟（进一步降低功耗）*/
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOE_CLK_DISABLE();
}



// 具体的关闭子函数示例
static void Power_Down_LEDs(void)
{
  All_LEDs_Off();
  // 可以在这里添加延时，确保视觉指示完成
  // HAL_Delay(10);
}

static void Power_Down_Communication_Interfaces(void)
{
  // 确保不再发送数据
  RS485_DE(IO_OFF);
  RS485_RE(IO_OFF);
  // 可以添加延时确保最后一位数据发送完成
  // HAL_Delay(1);
}

static void Power_Down_Bluetooth_Module(void)
{
  // 根据模块手册，可能需要一个特定的关机序列
  BT_WAKE(IO_OFF);
  HAL_Delay(5);
  BT_RESET(IO_ON);  // 举例：拉高复位脚
  HAL_Delay(10);
  BT_RESET(IO_OFF);
  BT_MODE(IO_OFF);
}

static void Power_Down_Sensors_And_Amplifiers(void)
{
  // 先关闭使能，再关闭电源轨，顺序可能很重要
  COIL_EN1(IO_OFF);
  COIL_EN2(IO_OFF);
  HAL_Delay(2);
  
  AD_POW_EN(IO_OFF);
  LC_POW_EN(IO_OFF);
  CL_POW_EN(IO_OFF);
  POWER_2_5(IO_OFF);
  AC_EN(IO_OFF);
  BD_EN(IO_OFF);
}

 /**
  * @brief  分阶段关闭所有外设电源
  * @param  reserved_pins: 需要保留用于唤醒的引脚掩码
  * @retval 无
  */
void Power_Down_All_Modules(uint16_t reserved_pins)
{
#if POWERDOWN_LEDS
  Power_Down_LEDs();
#endif

#if POWERDOWN_COMM
  Power_Down_Communication_Interfaces();
#endif

#if POWERDOWN_BT
  Power_Down_Bluetooth_Module();
#endif

#if POWERDOWN_SENSORS
  Power_Down_Sensors_And_Amplifiers();
#endif

  // 最终将所有GPIO置于最低功耗状态
 // Power_IO_All_Off(reserved_pins);
}



void Enter_Stop_Mode_Enhanced(void)
{
  // 1. 关闭所有外设时钟（ADC, UART, TIM等）
//  HAL_ADC_DeInit(&hadc1);
//  HAL_UART_DeInit(&huart1);
  // ... 关闭其他所有外设

  // 2. 有序关闭所有模块电源，并保留唤醒引脚（如PA0-WKUP）
  Power_Down_All_Modules(GPIO_PIN_0); // 保留PA0用于唤醒

  // 3. 清除唤醒标志，配置唤醒事件
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
  HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1); // 使能PA0唤醒

  // 4. 进入停止模式
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);

  // 5. 唤醒后重新初始化系统
  //SystemClock_Config(); // 必须重新配置时钟！
  MX_GPIO_Init();       // 重新初始化GPIO
  //MX_USART1_UART_Init();
  // ... 重新初始化其他必要的外设

  // 6. 恢复应用程序状态
  LED_RUN(IO_ON); // 例如，点亮运行灯表示已唤醒
}
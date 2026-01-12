/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "debug_printf.h"
#include "io_control.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LC_POW_EN_Pin GPIO_PIN_2
#define LC_POW_EN_GPIO_Port GPIOE
#define COIL_EN1_Pin GPIO_PIN_3
#define COIL_EN1_GPIO_Port GPIOE
#define COIL_EN2_Pin GPIO_PIN_4
#define COIL_EN2_GPIO_Port GPIOE
#define POWER_2_5_Pin GPIO_PIN_6
#define POWER_2_5_GPIO_Port GPIOE
#define CL_POW_EN_Pin GPIO_PIN_13
#define CL_POW_EN_GPIO_Port GPIOC
#define T_COIL_B1_Pin GPIO_PIN_0
#define T_COIL_B1_GPIO_Port GPIOC
#define T_COIL_B2_Pin GPIO_PIN_1
#define T_COIL_B2_GPIO_Port GPIOC
#define BT_MODE_Pin GPIO_PIN_2
#define BT_MODE_GPIO_Port GPIOC
#define BT_RESTORE_Pin GPIO_PIN_3
#define BT_RESTORE_GPIO_Port GPIOC
#define T_COIL_Pin GPIO_PIN_1
#define T_COIL_GPIO_Port GPIOA
#define PWM5_Pin GPIO_PIN_2
#define PWM5_GPIO_Port GPIOA
#define PWM6_Pin GPIO_PIN_3
#define PWM6_GPIO_Port GPIOA
#define ZJ_AIN3_Pin GPIO_PIN_4
#define ZJ_AIN3_GPIO_Port GPIOA
#define ZJ_AIN4_Pin GPIO_PIN_5
#define ZJ_AIN4_GPIO_Port GPIOA
#define ZJ_AIN11_Pin GPIO_PIN_6
#define ZJ_AIN11_GPIO_Port GPIOA
#define ZJ_AIN21_Pin GPIO_PIN_7
#define ZJ_AIN21_GPIO_Port GPIOA
#define ZJ_AIN1_Pin GPIO_PIN_0
#define ZJ_AIN1_GPIO_Port GPIOB
#define AD_POW_EN_Pin GPIO_PIN_10
#define AD_POW_EN_GPIO_Port GPIOE
#define PWM_LC3_Pin GPIO_PIN_11
#define PWM_LC3_GPIO_Port GPIOE
#define PWM_LC1_Pin GPIO_PIN_13
#define PWM_LC1_GPIO_Port GPIOE
#define PWM_LC2_Pin GPIO_PIN_14
#define PWM_LC2_GPIO_Port GPIOE
#define I2C_SCL_OLED_Pin GPIO_PIN_13
#define I2C_SCL_OLED_GPIO_Port GPIOB
#define I2C_SDA_OLED_Pin GPIO_PIN_14
#define I2C_SDA_OLED_GPIO_Port GPIOB
#define BT_LINK_Pin GPIO_PIN_15
#define BT_LINK_GPIO_Port GPIOB
#define BT_RESET_Pin GPIO_PIN_8
#define BT_RESET_GPIO_Port GPIOD
#define BT_READY_Pin GPIO_PIN_9
#define BT_READY_GPIO_Port GPIOD
#define AC_EN_Pin GPIO_PIN_10
#define AC_EN_GPIO_Port GPIOD
#define BD_EN_Pin GPIO_PIN_11
#define BD_EN_GPIO_Port GPIOD
#define LED_BT_Pin GPIO_PIN_12
#define LED_BT_GPIO_Port GPIOD
#define LED_RUN_Pin GPIO_PIN_13
#define LED_RUN_GPIO_Port GPIOD
#define LED_LC_Pin GPIO_PIN_14
#define LED_LC_GPIO_Port GPIOD
#define KEY_Pin GPIO_PIN_9
#define KEY_GPIO_Port GPIOC
#define RS485_RE_Pin GPIO_PIN_8
#define RS485_RE_GPIO_Port GPIOA
#define RS485_TX_Pin GPIO_PIN_9
#define RS485_TX_GPIO_Port GPIOA
#define RS485_RX_Pin GPIO_PIN_10
#define RS485_RX_GPIO_Port GPIOA
#define RS_485_DE_Pin GPIO_PIN_15
#define RS_485_DE_GPIO_Port GPIOA
#define BT_TX_Pin GPIO_PIN_10
#define BT_TX_GPIO_Port GPIOC
#define BT_RX_Pin GPIO_PIN_11
#define BT_RX_GPIO_Port GPIOC
#define BT_WAKE_Pin GPIO_PIN_12
#define BT_WAKE_GPIO_Port GPIOC
#define FLASH_SPI_NSS_Pin GPIO_PIN_0
#define FLASH_SPI_NSS_GPIO_Port GPIOD
#define FLASH_SPI_CLK_Pin GPIO_PIN_1
#define FLASH_SPI_CLK_GPIO_Port GPIOD
#define FLASH_SPI_MISO_Pin GPIO_PIN_3
#define FLASH_SPI_MISO_GPIO_Port GPIOD
#define FLASH_SPI_MOSI_Pin GPIO_PIN_4
#define FLASH_SPI_MOSI_GPIO_Port GPIOD
#define WDI_Pin GPIO_PIN_5
#define WDI_GPIO_Port GPIOD
#define FLASH_HOLD_Pin GPIO_PIN_6
#define FLASH_HOLD_GPIO_Port GPIOD
#define FLASH_WP_Pin GPIO_PIN_7
#define FLASH_WP_GPIO_Port GPIOD
#define OLED_RESET_Pin GPIO_PIN_3
#define OLED_RESET_GPIO_Port GPIOB
#define EN_LED_VDD_Pin GPIO_PIN_4
#define EN_LED_VDD_GPIO_Port GPIOB
#define I2C_SCL_4_20MA_Pin GPIO_PIN_6
#define I2C_SCL_4_20MA_GPIO_Port GPIOB
#define I2C_SDA_4_20MA_Pin GPIO_PIN_7
#define I2C_SDA_4_20MA_GPIO_Port GPIOB
#define ALARM_4_20MA_Pin GPIO_PIN_8
#define ALARM_4_20MA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, LC_POW_EN_Pin|COIL_EN1_Pin|COIL_EN2_Pin|POWER_2_5_Pin
                          |AD_POW_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, CL_POW_EN_Pin|BT_MODE_Pin|BT_RESTORE_Pin|BT_WAKE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, BT_RESET_Pin|AC_EN_Pin|BD_EN_Pin|LED_BT_Pin
                          |LED_RUN_Pin|LED_LC_Pin|WDI_Pin|FLASH_HOLD_Pin
                          |FLASH_WP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, RS485_RE_Pin|RS_485_DE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, OLED_RESET_Pin|EN_LED_VDD_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LC_POW_EN_Pin COIL_EN1_Pin COIL_EN2_Pin POWER_2_5_Pin
                           AD_POW_EN_Pin */
  GPIO_InitStruct.Pin = LC_POW_EN_Pin|COIL_EN1_Pin|COIL_EN2_Pin|POWER_2_5_Pin
                          |AD_POW_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : CL_POW_EN_Pin BT_MODE_Pin BT_RESTORE_Pin BT_WAKE_Pin */
  GPIO_InitStruct.Pin = CL_POW_EN_Pin|BT_MODE_Pin|BT_RESTORE_Pin|BT_WAKE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : T_COIL_B1_Pin T_COIL_B2_Pin */
  GPIO_InitStruct.Pin = T_COIL_B1_Pin|T_COIL_B2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : T_COIL_Pin */
  GPIO_InitStruct.Pin = T_COIL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(T_COIL_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BT_LINK_Pin ALARM_4_20MA_Pin */
  GPIO_InitStruct.Pin = BT_LINK_Pin|ALARM_4_20MA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : BT_RESET_Pin AC_EN_Pin BD_EN_Pin LED_BT_Pin
                           LED_RUN_Pin LED_LC_Pin WDI_Pin FLASH_HOLD_Pin
                           FLASH_WP_Pin */
  GPIO_InitStruct.Pin = BT_RESET_Pin|AC_EN_Pin|BD_EN_Pin|LED_BT_Pin
                          |LED_RUN_Pin|LED_LC_Pin|WDI_Pin|FLASH_HOLD_Pin
                          |FLASH_WP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : BT_READY_Pin */
  GPIO_InitStruct.Pin = BT_READY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BT_READY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : KEY_Pin */
  GPIO_InitStruct.Pin = KEY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RS485_RE_Pin RS_485_DE_Pin */
  GPIO_InitStruct.Pin = RS485_RE_Pin|RS_485_DE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : OLED_RESET_Pin EN_LED_VDD_Pin */
  GPIO_InitStruct.Pin = OLED_RESET_Pin|EN_LED_VDD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

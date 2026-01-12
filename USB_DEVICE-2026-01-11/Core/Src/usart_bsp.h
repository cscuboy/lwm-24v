#ifndef USART1_BSP_H
#define USART1_BSP_H
#include "io_control.h"
#include "main.h"
#include "stm32l4xx_hal.h"

void usart1_isr_handler(void);
HAL_StatusTypeDef RS485_Send_Data_DMA(uint8_t *pData, uint16_t Size) ;
void Start_RS485_DMA_Reception(void) ;

#endif

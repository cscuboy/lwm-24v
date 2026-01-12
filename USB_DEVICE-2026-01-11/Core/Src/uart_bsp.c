#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "usart_bsp.h"
#include "string.h"

// 定义方向控制引脚
#define RS485_RE_PIN RS485_RE_Pin
#define RS485_RE_PORT RS485_RE_GPIO_Port
#define RS485_DE_PIN RS_485_DE_Pin
#define RS485_DE_PORT RS_485_DE_GPIO_Port

// 控制宏：设置发送模式（DE=1, RE=1） 和 接收模式（DE=0, RE=0）
#define RS485_SET_TX() do { \
    HAL_GPIO_WritePin(RS485_DE_PORT, RS485_DE_PIN, GPIO_PIN_SET); \
    HAL_GPIO_WritePin(RS485_RE_PORT, RS485_RE_PIN, GPIO_PIN_SET); \
} while(0)

#define RS485_SET_RX() do { \
    HAL_GPIO_WritePin(RS485_DE_PORT, RS485_DE_PIN, GPIO_PIN_RESET); \
    HAL_GPIO_WritePin(RS485_RE_PORT, RS485_RE_PIN, GPIO_PIN_RESET); \
} while(0)

extern void Rs485Rx_Isr_handle(uint8_t* padata ,uint16_t len);

// 全局变量声明
extern UART_HandleTypeDef huart1; // 串口句柄
extern DMA_HandleTypeDef hdma_usart1_tx; // 发送DMA句柄
extern DMA_HandleTypeDef hdma_usart1_rx; // 接收DMA句柄

uint8_t RS485_Rx_Buffer[256]; // DMA接收缓冲区
uint8_t RS485_Tx_Buffer[256]; // 发送数据缓冲区
volatile uint8_t RS485_Tx_Complete = 1; // 发送完成标志


void usart1_isr_handler(void)
{
 //   BaseType_t xHigherPriorityTaskWoken = pdFALSE; // 必须初始化为 pdFALSE

  if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE) != RESET) 
  {
        HAL_UART_DMAStop(&huart1);
        __HAL_UART_CLEAR_IDLEFLAG(&huart1); // 清除空闲中断标志

        // 计算当前DMA接收到的数据长度
        uint16_t received_len = sizeof(RS485_Rx_Buffer) - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);

        if (received_len > 0) 
        {
            
            // 在这里处理接收到的数据帧 RS485_Rx_Buffer[0..received_len-1]
            // 例如：将数据拷贝到应用缓冲区，并设置标志通知应用层
            Rs485Rx_Isr_handle(RS485_Rx_Buffer,received_len);
            // 处理完成后，可选：重启DMA接收（循环模式通常不需要，但若处理耗时需防止覆盖）
            memset(RS485_Rx_Buffer,0x0,sizeof(RS485_Rx_Buffer));
             HAL_UART_Receive_DMA(&huart1, RS485_Rx_Buffer, sizeof(RS485_Rx_Buffer));
        }
    }
    HAL_UART_IRQHandler(&huart1); // 调用HAL库中断处理函数
}


// RS485 DMA发送函数
HAL_StatusTypeDef RS485_Send_Data_DMA(uint8_t *pData, uint16_t Size) 
{
    HAL_StatusTypeDef status;

    // 等待上一次DMA发送完成
    if (RS485_Tx_Complete == 0) 
    {
        return HAL_BUSY;
    }
    if (Size > sizeof(RS485_Tx_Buffer)) 
    {
        return HAL_ERROR;
    }

    RS485_Tx_Complete = 0; // 清除发送完成标志

    // 复制数据到发送缓冲区
    memcpy(RS485_Tx_Buffer, pData, Size);

    // 切换为发送模式
    RS485_SET_TX();
    // 可选：短暂延时，确保RS485收发器模式稳定[1,2](@ref)
    HAL_Delay(1);

    // 启动DMA发送
    status = HAL_UART_Transmit_DMA(&huart1, RS485_Tx_Buffer, Size);

    if (status != HAL_OK) 
    {
        RS485_Tx_Complete = 1;
        RS485_SET_RX(); // 如果启动失败，切回接收模式
    }
    return status;
}

// DMA发送完成中断回调函数
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) 
{
    if (huart->Instance == USART1) 
    {
        RS485_Tx_Complete = 1; // 设置发送完成标志
        RS485_SET_RX(); // 关键：发送完成，立即切回接收模式[2](@ref)
    }
}

// 启动DMA循环接收（在main初始化时调用）
void Start_RS485_DMA_Reception(void) 
{
    // 在USART1初始化后启用空闲中断
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    
    RS485_SET_RX(); // 确保处于接收模式
    HAL_UART_Receive_DMA(&huart1, RS485_Rx_Buffer, sizeof(RS485_Rx_Buffer));
}
#ifndef __UART_H__
#define __UART_H__

#include "main.h"
#include "usart.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define UART_COMM huart1       // 绑定CubeMX生成的UART句柄
#define UART_RX_BUF_SIZE 64    // 接收缓冲区大小（根据需求调整）
#define UART_FRAME_HEADER 0xAA // 协议帧头
#define UART_FRAME_TAIL 0x55   // 协议帧尾

void UART_Init(void);
void UART_SendString(const char *str);
void UART_SendArray(uint8_t *data, uint16_t len);
void UART_Printf(const char *format, ...);
uint8_t UART_GetFrame(uint8_t *rx_buf, uint16_t *rx_len);

#endif /* __UART_H__ */

#include "uart.h"
#include <stdarg.h>

// 接收缓冲区（全局变量，中断用）
static volatile uint8_t uart_rx_buf[UART_RX_BUF_SIZE];
// 接收状态标志
static volatile uint8_t uart_rx_flag = 0;
static volatile uint16_t uart_rx_len = 0;

/**
 * @brief  UART模块初始化（开启接收中断）
 */
void UART_Init(void)
{
    // 开启空闲中断+接收中断，用于接收不定长数据
    __HAL_UART_ENABLE_IT(&UART_COMM, UART_IT_IDLE);
    HAL_UART_Receive_IT(&UART_COMM, (uint8_t *)uart_rx_buf, UART_RX_BUF_SIZE);
}

/**
 * @brief  发送字符串
 */
void UART_SendString(const char *str)
{
    if (str == NULL)
        return;
    HAL_UART_Transmit(&UART_COMM, (uint8_t *)str, strlen(str), 100);
}

/**
 * @brief  发送数组
 */
void UART_SendArray(uint8_t *data, uint16_t len)
{
    if (data == NULL || len == 0)
        return;
    HAL_UART_Transmit(&UART_COMM, data, len, 100);
}

/**
 * @brief  格式化发送（类似printf）
 */
void UART_Printf(const char *format, ...)
{
    char buf[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);
    UART_SendString(buf);
}

/**
 * @brief  获取接收到的一帧数据
 */
uint8_t UART_GetFrame(uint8_t *rx_buf, uint16_t *rx_len)
{
    if (uart_rx_flag == 0)
        return 0;

    // 关闭中断，防止数据竞争
    __HAL_UART_DISABLE_IT(&UART_COMM, UART_IT_IDLE);

    // 复制接收到的数据
    memcpy(rx_buf, (uint8_t *)uart_rx_buf, uart_rx_len);
    *rx_len = uart_rx_len;

    // 清除标志，重新开启接收
    uart_rx_flag = 0;
    uart_rx_len = 0;
    __HAL_UART_ENABLE_IT(&UART_COMM, UART_IT_IDLE);
    HAL_UART_Receive_IT(&UART_COMM, (uint8_t *)uart_rx_buf, UART_RX_BUF_SIZE);

    return 1;
}

/**
 * @brief  UART空闲中断回调函数（CubeMX生成的中断服务函数会调用这个）
 * @note   当UART总线空闲时触发，说明一帧数据接收完成
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart == &UART_COMM)
    {
        // 停止当前接收，记录接收到的数据长度
        HAL_UART_AbortReceive(&UART_COMM);
        uart_rx_len = Size;
        uart_rx_flag = 1;
    }
}

/**
 * @brief  UART错误回调函数（可选，用于调试）
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart == &UART_COMM)
    {
        // 出错后重新开启接收
        uart_rx_flag = 0;
        uart_rx_len = 0;
        HAL_UART_Receive_IT(&UART_COMM, (uint8_t *)uart_rx_buf, UART_RX_BUF_SIZE);
    }
}

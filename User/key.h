#ifndef __KEY_H
#define __KEY_H

#include "main.h"
#include "gpio.h"
#include "tim.h"

// 定义按键个数
#define KEY_NUM 1 // 1个按键,可定义多个

#define KEY1 0
// #define KEY2 1
// #define KEY3 2

// 按键电平定义（上拉输入）
#define KEY_UP 1
#define KEY_DOWN 0

// 引脚定义（改成实际引脚）
#define KEY1_GPIO_PORT GPIOA
#define KEY1_GPIO_PIN GPIO_PIN_0

// #define KEY2_GPIO_PORT GPIOA
// #define KEY2_GPIO_PIN GPIO_PIN_1

// #define KEY3_GPIO_PORT GPIOC
// #define KEY3_GPIO_PIN GPIO_PIN_13

// 定时器
#define KEY_TIM htim2

// 函数声明
void Key_Scan(void);
void Key_Scan_TIM(void);
uint8_t Key_Get_Click(uint8_t Key_ID);
uint8_t Key_Get_Hold(uint8_t Key_ID);

#endif /* __KEY_H */

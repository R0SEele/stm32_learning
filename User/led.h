#ifndef __LED_H__
#define __LED_H__

#include "gpio.h"
#include "tim.h"

// 对模块的引脚进行宏定义 便于工程移植和管理

#define LED_GPIO_PORT GPIOC
#define LED_GPIO_PIN GPIO_PIN_13
#define LED_TIM htim3

void Led_Twinkle_Delay(void);
void Led_Twinkle_TIM(void);
void LED_Twinkle_Procss(void);

#endif /*__ LED_H__ */

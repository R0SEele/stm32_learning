#ifndef __PWM_H__
#define __PWM_H__

#include "gpio.h"
#include "tim.h"
#include "stm32f1xx_hal_tim.h"

// 对模块的引脚进行宏定义 便于工程移植和管理

#define PWM_LED_TIM htim1        // LED 定时器
#define PWM_LED_CH TIM_CHANNEL_1 // LED 定时器通道

extern uint16_t PWM_LED_ARR;

void PWM_Init(void);
void PWM_SetDutyCycle(TIM_HandleTypeDef *tim, uint32_t ch, uint16_t duty);
void PWM_Start(TIM_HandleTypeDef *tim, uint32_t ch);

#endif /*__ PWM_H__ */

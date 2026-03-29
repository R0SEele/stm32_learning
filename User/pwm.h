#ifndef __PWM_H__
#define __PWM_H__

#include "main.h"
#include "tim.h"

#define PWM_LED_TIM htim1

#define PWM_LED_CH TIM_CHANNEL_1

void PWM_Init(void);
void PWM_Start(TIM_HandleTypeDef *tim, uint32_t ch);
void PWM_Stop(TIM_HandleTypeDef *tim, uint32_t ch);
void PWM_SetDutyCycle(TIM_HandleTypeDef *tim, uint32_t ch, uint16_t duty);
void PWM_SetDutyCycle_Percent(TIM_HandleTypeDef *tim, uint32_t ch, float percent);
void PWM_Start_IT(TIM_HandleTypeDef *tim, uint32_t ch);
void PWM_Stop_IT(TIM_HandleTypeDef *tim, uint32_t ch);
void PWM_Start_DMA(TIM_HandleTypeDef *tim, uint32_t ch, uint32_t *pData, uint16_t length);
void PWM_Stop_DMA(TIM_HandleTypeDef *tim, uint32_t ch);

#endif /* __PWM_H__ */

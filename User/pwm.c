#include "pwm.h"

// 定义全局变量
uint16_t PWM_LED_ARR = 0;

void PWM_Init(void)
{
    // 在初始化时读取一次 ARR，存起来
    PWM_LED_ARR = __HAL_TIM_GET_AUTORELOAD(PWM_LED_TIM);
}

void PWM_SetDutyCycle(TIM_HandleTypeDef *tim, uint32_t ch, uint16_t duty)
{
    // 这里为了通用性，还是建议实时读，或者你也可以直接用 PWM_LED_ARR
    uint16_t arr = __HAL_TIM_GET_AUTORELOAD(tim);
    if (duty > arr)
        duty = arr;
    __HAL_TIM_SET_COMPARE(tim, ch, duty);
}

void PWM_Start(TIM_HandleTypeDef *tim, uint32_t ch)
{
    HAL_TIM_PWM_Start(tim, ch);
}
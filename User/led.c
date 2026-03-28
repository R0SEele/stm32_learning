#include "led.h"

/**
 * @brief  LED闪烁函数
 * @note   该函数通过切换GPIOC的13号引脚状态来实现LED的闪烁效果，每次切换后延时500毫秒。使用简单的HAL库函数来控制GPIO引脚和延时。
 * @param  None
 */

void Led_Twinkle_Delay(void)
{
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PIN);
    HAL_Delay(500);
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PIN);
    HAL_Delay(500);
}

/**
 * @brief  LED闪烁函数（基于定时器中断）
 * @note   该函数通过启动定时器3的中断来实现LED的闪烁效果。定时器3的中断服务函数会调用回调函数来切换GPIOC的13号引脚状态，从而实现LED的闪烁。使用定时器中断可以让LED在后台自动闪烁，而不需要占用CPU资源进行延时。
 * @param  None
 */

// 定时器中断有两个函数，一个是中断服务函数，放在stm32f1xx_it.c中，另一个是回调函数，放在main.c中，便于统一管理移植。
// 中断服务函数会调用回调函数来处理具体的操作。
// 中断触发间隔 （PSC + 1）* (ARR + 1) / 系统时钟频率

void Led_Twinkle_TIM(void)
{
    if (HAL_TIM_Base_Start_IT(&LED_TIM) != HAL_OK) // HAL_TIM_Base_Start_IT(&htim3)开启定时器中断，这时候才能正常工作
    {
        // 中断开启失败的错误处理
        Error_Handler();
    }
}

/**
 * @brief LED翻转函数
 * @note
 * @param None

*/
void LED_Twinkle_Procss(void)
{
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PIN);
}

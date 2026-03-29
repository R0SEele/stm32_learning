#include "pwm.h"

// 用于缓存定时器ARR自动重装载值，减少寄存器重复读取，提升执行效率
static uint16_t PWM_LED_ARR = 0;

/**
 * @brief  PWM初始化，预读取并缓存定时器ARR自动重装载值
 * @note   必须在CubeMX生成的定时器初始化函数 MX_TIMx_Init 之后调用
 */
void PWM_Init(void)
{
    // 空指针校验，防止句柄未初始化导致程序跑飞
    if (&PWM_LED_TIM == NULL)
    {
        return;
    }
    // 预缓存ARR基准值，为后续占空比计算提供依据
    PWM_LED_ARR = __HAL_TIM_GET_AUTORELOAD(&PWM_LED_TIM);
}

/**
 * @brief  启动PWM波形输出
 * @param  tim: 定时器句柄地址，例：&htim1、&htim2
 * @param  ch: PWM通道号，例：TIM_CHANNEL_1
 * @note   自动兼容通用/高级定时器，修复高级定时器无输出的高频问题
 */
void PWM_Start(TIM_HandleTypeDef *tim, uint32_t ch)
{
    // 空指针安全校验
    if (tim == NULL)
    {
        return;
    }
    // 高级定时器自动开启主输出，通用定时器调用此宏无副作用，解决新手无输出踩坑
    __HAL_TIM_MOE_ENABLE(tim);
    // 启动PWM输出
    HAL_TIM_PWM_Start(tim, ch);
}

/**
 * @brief  停止PWM波形输出
 * @param  tim: 定时器句柄地址，例：&htim1、&htim2
 * @param  ch: PWM通道号，例：TIM_CHANNEL_1
 */
void PWM_Stop(TIM_HandleTypeDef *tim, uint32_t ch)
{
    if (tim == NULL)
    {
        return;
    }
    // 停止PWM输出
    HAL_TIM_PWM_Stop(tim, ch);
    // 低功耗场景可取消注释，关闭高级定时器主输出
    // __HAL_TIM_MOE_DISABLE(tim);
}

/**
 * @brief  设置PWM占空比（直接写入CCR比较值）
 * @param  tim: 定时器句柄地址，例：&htim1、&htim2
 * @param  ch: PWM通道号，例：TIM_CHANNEL_1
 * @param  duty: CCR比较值，范围0~ARR，超出自动限幅
 */
void PWM_SetDutyCycle(TIM_HandleTypeDef *tim, uint32_t ch, uint16_t duty)
{
    if (tim == NULL)
    {
        return;
    }
    // 实时读取当前ARR值，兼容定时器动态改频场景
    uint16_t arr_current = __HAL_TIM_GET_AUTORELOAD(tim);
    // 上下限双限幅，防止异常值导致PWM输出错误
    if (duty > arr_current)
        duty = arr_current;
    if (duty < 0)
        duty = 0;
    // 写入比较寄存器，实时更新占空比
    __HAL_TIM_SET_COMPARE(tim, ch, duty);
}

/**
 * @brief  设置PWM占空比（百分比模式 0~100%）
 * @param  tim: 定时器句柄地址，例：&htim1、&htim2
 * @param  ch: PWM通道号，例：TIM_CHANNEL_1
 * @param  percent: 占空比百分比，支持浮点精度，超出自动限幅
 */
void PWM_SetDutyCycle_Percent(TIM_HandleTypeDef *tim, uint32_t ch, float percent)
{
    if (tim == NULL)
    {
        return;
    }
    // 百分比范围安全限幅
    if (percent > 100.0f)
        percent = 100.0f;
    if (percent < 0.0f)
        percent = 0.0f;
    // 计算对应CCR值，32位中间运算避免16位数据溢出
    uint16_t arr_current = __HAL_TIM_GET_AUTORELOAD(tim);
    uint32_t ccr_value = (uint32_t)((float)arr_current * percent / 100.0f);
    // 写入比较寄存器
    __HAL_TIM_SET_COMPARE(tim, ch, (uint16_t)ccr_value);
}

/**
 * @brief  启动中断模式PWM输出
 * @param  tim: 定时器句柄地址，例：&htim1、&htim2
 * @param  ch: PWM通道号，例：TIM_CHANNEL_1
 * @note   需提前在CubeMX开启对应定时器的PWM中断
 */
void PWM_Start_IT(TIM_HandleTypeDef *tim, uint32_t ch)
{
    if (tim == NULL)
    {
        return;
    }
    __HAL_TIM_MOE_ENABLE(tim);
    // 启动中断模式PWM
    HAL_TIM_PWM_Start_IT(tim, ch);
}

/**
 * @brief  停止中断模式PWM输出
 * @param  tim: 定时器句柄地址，例：&htim1、&htim2
 * @param  ch: PWM通道号，例：TIM_CHANNEL_1
 */
void PWM_Stop_IT(TIM_HandleTypeDef *tim, uint32_t ch)
{
    if (tim == NULL)
    {
        return;
    }
    // 停止中断模式PWM
    HAL_TIM_PWM_Stop_IT(tim, ch);
}

/**
 * @brief  启动DMA模式PWM输出
 * @param  tim: 定时器句柄地址，例：&htim1、&htim2
 * @param  ch: PWM通道号，例：TIM_CHANNEL_1
 * @param  pData: DMA数据源缓冲区地址
 * @param  length: 传输数据长度
 * @note   需提前在CubeMX配置定时器对应DMA通道
 */
void PWM_Start_DMA(TIM_HandleTypeDef *tim, uint32_t ch, uint32_t *pData, uint16_t length)
{
    // 全参数安全校验，防止空指针和无效长度导致的HardFault
    if (tim == NULL || pData == NULL || length == 0)
    {
        return;
    }
    __HAL_TIM_MOE_ENABLE(tim);
    // 启动DMA模式PWM
    HAL_TIM_PWM_Start_DMA(tim, ch, pData, length);
}

/**
 * @brief  停止DMA模式PWM输出
 * @param  tim: 定时器句柄地址，例：&htim1、&htim2
 * @param  ch: PWM通道号，例：TIM_CHANNEL_1
 */
void PWM_Stop_DMA(TIM_HandleTypeDef *tim, uint32_t ch)
{
    if (tim == NULL)
    {
        return;
    }
    // 停止DMA模式PWM
    HAL_TIM_PWM_Stop_DMA(tim, ch);
}

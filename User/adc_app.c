#include "adc_app.h"

/* 中断模式私有变量 */
static volatile uint16_t adc_it_val = 0;
static volatile uint8_t adc_it_flag = 0;

static volatile uint16_t adc_dma_val; // DMA 自动存放采样值

/**
 * @brief  ADC业务模块初始化
 * @retval 无
 */
void ADC_APP_Init(void)
{
    HAL_ADCEx_Calibration_Start(&ADC_APP_HANDLE);
}

/**
 * @brief  轮询模式采集电压（阻塞）
 * @param  voltage  输出实际电压值(V)
 * @retval 0=成功 1=参数错误 2=超时
 */
uint8_t ADC_APP_GetVoltage_Polling(float *voltage)
{
    if (voltage == NULL)
        return 1;

    uint16_t sample;

    HAL_ADC_Start(&ADC_APP_HANDLE);

    if (HAL_ADC_PollForConversion(&ADC_APP_HANDLE, 100) != HAL_OK)
    {
        HAL_ADC_Stop(&ADC_APP_HANDLE);
        return 2;
    }

    sample = HAL_ADC_GetValue(&ADC_APP_HANDLE);
    HAL_ADC_Stop(&ADC_APP_HANDLE);

    *voltage = (sample / ADC_APP_RESOLUTION) * ADC_APP_REF_VOLTAGE;

    return 0;
}

/**
 * @brief  启动ADC中断采集
 * @retval 无
 */
void ADC_APP_Start_IT(void)
{
    adc_it_flag = 0;
    HAL_ADC_Start_IT(&ADC_APP_HANDLE);
}

/**
 * @brief  从中断模式获取电压
 * @param  voltage 输出电压
 * @retval 0=成功 1=参数错 2=无新数据
 */
uint8_t ADC_APP_GetVoltage_IT(float *voltage)
{
    if (voltage == NULL)
        return 1;
    if (adc_it_flag == 0)
        return 2;

    *voltage = (adc_it_val / ADC_APP_RESOLUTION) * ADC_APP_REF_VOLTAGE;

    adc_it_flag = 0;
    ADC_APP_Start_IT();

    return 0;
}

/**
 * @brief  ADC转换完成回调函数
 * @param  hadc ADC句柄
 * @retval 无
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc == &ADC_APP_HANDLE)
    {
        adc_it_val = HAL_ADC_GetValue(hadc);
        adc_it_flag = 1;
    }
}

/**
 * @brief  启动ADC DMA连续采集
 * @retval 无
 */
void ADC_APP_Start_DMA(void)
{
    // 启动DMA：单次转换结果自动搬运到 adc_dma_val
    HAL_ADC_Start_DMA(&ADC_APP_HANDLE, (uint32_t *)&adc_dma_val, 1);
}

/**
 * @brief  获取DMA最新电压
 * @param  voltage 输出电压
 * @retval 0=成功 1=参数错误
 */
uint8_t ADC_APP_GetVoltage_DMA(float *voltage)
{
    if (voltage == NULL)
        return 1;

    // DMA 已自动更新数据，直接读取即可
    *voltage = (adc_dma_val / ADC_APP_RESOLUTION) * ADC_APP_REF_VOLTAGE;

    return 0;
}

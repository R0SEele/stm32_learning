#ifndef __ADC_APP_H__
#define __ADC_APP_H__

#include "main.h"
#include "adc.h"
#include "uart.h"

#define ADC_APP_HANDLE hadc1          ///< 绑定CubeMX生成的ADC句柄
#define ADC_APP_CHANNEL ADC_CHANNEL_1 ///< ADC采集通道：对应PA1引脚
#define ADC_APP_REF_VOLTAGE 3.3f      ///< ADC参考电压（开发板默认3.3V）
#define ADC_APP_RESOLUTION 4095.0f    ///< ADC分辨率最大值（12位ADC：2^12 - 1 = 4095）

void ADC_APP_Init(void);
uint8_t ADC_APP_GetVoltage_Polling(float *voltage);
void ADC_APP_Start_IT(void);
uint8_t ADC_APP_GetVoltage_IT(float *voltage);
void ADC_APP_Start_DMA(void);
uint8_t ADC_APP_GetVoltage_DMA(float *voltage);

#endif /* __ADC_APP_H__ */

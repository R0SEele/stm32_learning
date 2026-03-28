#include "key.h"

// 每个按键独立变量
uint8_t Key_State[KEY_NUM] = {KEY_UP};
uint8_t Key_CNT[KEY_NUM] = {0};          // 消抖计数
uint8_t Key_Trigger[KEY_NUM] = {0};      // 单击触发标志
uint16_t Key_Hold_CNT[KEY_NUM] = {0};    // 长按计数
uint8_t Key_Hold_Flag[KEY_NUM] = {0};    // 长按标志
uint8_t Key_Hold_Trigger[KEY_NUM] = {0}; // 长按松开触发标志

// 读取按键电平
uint8_t Key_ReadPin(uint8_t Key_ID)
{
    switch (Key_ID)
    {
    case KEY1:
        return HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN);
    // case KEY2:
    //     return HAL_GPIO_ReadPin(KEY2_GPIO_PORT, KEY2_GPIO_PIN);
    // case KEY3:
    //     return HAL_GPIO_ReadPin(KEY3_GPIO_PORT, KEY3_GPIO_PIN);
    default:
        return KEY_UP;
    }
}

// 10ms 扫描所有按键
void Key_Scan(void)
{
    for (uint8_t i = 0; i < KEY_NUM; i++)
    {
        uint8_t key_val = Key_ReadPin(i);

        if (key_val == KEY_DOWN) // 按下
        {
            Key_CNT[i]++;

            if (Key_CNT[i] >= 2) // 消抖20ms
            {
                if (Key_State[i] == KEY_UP)
                {
                    Key_State[i] = KEY_DOWN;
                    Key_Trigger[i] = 1;
                    Key_Hold_CNT[i] = 0;
                }

                Key_Hold_CNT[i]++;

                // 长按2s触发
                if (Key_Hold_CNT[i] >= 200)
                {
                    Key_Hold_Flag[i] = 1;
                    Key_Trigger[i] = 0;
                }
            }
        }
        else // 松开
        {
            if (Key_State[i] == KEY_DOWN && Key_Hold_Flag[i] == 1)
            {
                Key_Hold_Trigger[i] = 1;
            }

            Key_CNT[i] = 0;
            Key_State[i] = KEY_UP;
            Key_Hold_CNT[i] = 0;
            Key_Hold_Flag[i] = 0;
        }
    }
}

// 启动定时器
void Key_Scan_TIM(void)
{
    HAL_TIM_Base_Start_IT(&KEY_TIM);
}

// 获取单击
uint8_t Key_Get_Click(uint8_t Key_ID)
{
    if (Key_Trigger[Key_ID] == 1)
    {
        Key_Trigger[Key_ID] = 0;
        return 1;
    }
    return 0;
}

// 获取长按（松开执行）
uint8_t Key_Get_Hold(uint8_t Key_ID)
{
    if (Key_Hold_Trigger[Key_ID] == 1)
    {
        Key_Hold_Trigger[Key_ID] = 0;
        return 1;
    }
    return 0;
}

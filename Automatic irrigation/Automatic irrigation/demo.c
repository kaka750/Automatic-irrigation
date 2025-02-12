/****************************************
 * 
 *      filename    :   demo.c
 *      author      :   15359598362@163.com
 *      date        :   2024/03/13
 *      funtion     :   基于温湿度传感器的自动检测的自动灌溉
 *      note        :   当温度超过阈值或湿度低于阈值时，自动控制水泵启停
 *      COpyRight   :   2024-2025   15359598362@163.com    All Rights Reserved.
 * 
 ****************************************/


#include "stm32f1xx_hal.h"
#include "dht11.h"

#define TEMP_THRESHOLD 30  // 温度阈值，单位：摄氏度
#define HUMIDITY_THRESHOLD 60  // 湿度阈值，单位：百分比

GPIO_InitTypeDef GPIO_InitStruct;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    DHT11_Init();

    while (1)
    {
        DHT11_DataTypedef DHT11_Data;
        if (DHT11_ReadData(&DHT11_Data) == DHT11_OK)
        {
            if (DHT11_Data.Temperature > TEMP_THRESHOLD || DHT11_Data.Humidity < HUMIDITY_THRESHOLD)
            {
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); // 打开水泵
            }
            else
            {
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET); // 关闭水泵
            }
        }
        HAL_Delay(2000); // 延迟2秒
    }
}

void SystemClock_Config(void)
{
    // 系统时钟配置代码
}

static void MX_GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

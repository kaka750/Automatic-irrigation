/****************************************
 * 
 *      filename    :   dh11.c
 *      author      :   15359598362@163.com
 *      date        :   2024/03/11
 *      funtion     :   基于温湿度传感器的自动检测的自动灌溉
 *      note        :   当温度超过阈值或湿度低于阈值时，自动控制水泵启停
 *      COpyRight   :   2024-2025   15359598362@163.com    All Rights Reserved.
 * 
 ****************************************/


#include "dht11.h"


/****************************************
 * 
 *      name     :   DHT11_Init
 *      funtion  :   初始化DHT11传感器
 *      argument :   None
 *      retval   :   None
 *      author   :   15359598362@163.com
 *      date     :   2025/04/11
 * 
 ****************************************/
void DHT11_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();// 使能GPIOB时钟
    
    // 配置DHT11数据引脚为推挽输出模式
    GPIO_InitStruct.Pin = DHT11_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);// 初始化DHT11_PORT端口上的DHT11_PIN引脚

    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);// 初始化时将DHT11_PIN设置为高电平
}


/****************************************
 * 
 *      name     :   DHT11_ReadData
 *      funtion  :   从DHT11传感器读取数据
 *      argument :   DHT11_Data: 存储读取的温湿度数据
 *      retval   :   成功返回DHT11_OK，失败返回DHT11_ERROR
 *      author   :   15359598362@163.com
 *      date     :   2025/04/11
 * 
 ****************************************/


 // 从DHT11传感器读取数据
uint8_t DHT11_ReadData(DHT11_DataTypedef *DHT11_Data)
{
    uint8_t i, j, temp, sum;
    uint8_t buffer[5] = {0};

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 发送开始信号给DHT11传感器
    GPIO_InitStruct.Pin = DHT11_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_RESET);
    HAL_Delay(18);
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
    HAL_Delay(20);
   
    // 切换引脚模式为输入模式以等待DHT11的响应
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);

    HAL_Delay(40);
    if (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_RESET)
    {
        HAL_Delay(80);
        // 检测DHT11是否拉高DHT11_PIN（DHT11响应信号结束）
        if (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_SET)
        {
            HAL_Delay(80);

            for (j = 0; j < 5; j++)
            {
                for (i = 0; i < 8; i++)
                {
                    // 等待DHT11拉低信号开始发送一位数据
                    while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_RESET);
                    HAL_Delay(40);
                    if (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_SET)
                    {
                        temp = (1 << (7 - i));
                        buffer[j] |= temp;
                    }
                     // 等待DHT11拉低信号结束当前位的发送
                    while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_SET);
                }
            }
            sum = buffer[0] + buffer[1] + buffer[2] + buffer[3];
            if (sum == buffer[4])
            {
                DHT11_Data->Humidity = buffer[0];
                DHT11_Data->Temperature = buffer[2];
                return DHT11_OK;
            }
            else
            {
                return DHT11_ERROR;
            }
        }
    }
    return DHT11_ERROR;
}

/****************************************
 * 
 *      filename    :   dh11.c
 *      author      :   15359598362@163.com
 *      date        :   2024/04/11
 *      funtion     :   基于温湿度传感器的自动检测的自动灌溉
 *      note        :   当温度超过阈值或湿度低于阈值时，自动控制水泵启停
 *      COpyRight   :   2024-2025   15359598362@163.com    All Rights Reserved.
 * 
 ****************************************/


#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f1xx_hal.h"

#define DHT11_PORT GPIOB
#define DHT11_PIN GPIO_PIN_0

typedef struct
{
    uint8_t Temperature;
    uint8_t Humidity;
} DHT11_DataTypedef;

void DHT11_Init(void);
uint8_t DHT11_ReadData(DHT11_DataTypedef *DHT11_Data);

#endif
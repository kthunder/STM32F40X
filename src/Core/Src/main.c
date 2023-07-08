#include <stdint.h>
#include <stdio.h>

#include "log.h"
#include "tools.h"

#include "stm32f4xx.h"
#include "hal_flash.h"
#include "hal_gpio.h"
#include "hal_rcc.h"
#include "hal_usart.h"
#include "hal_spi.h"

#include "MinRTOS.h"

void GpioInit();

int main()
{
    GpioInit();
    USART_Init(USART1);

    vTaskStartScheduler();
}

/* private func*/
void GpioInit()
{
    GPIO_InitTypeDef usart1_config = {
        .pin = GPIO_Pin_9,
        .mode = GPIO_MODE_AF_OUT,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_FREQ_HIGH,
        .pupd = GPIO_PUPD_PULL_UP,
        .af = GPIO_AF_7,
    };
    GPIO_InitTypeDef spi1_config = {
        .pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7,
        .mode = GPIO_MODE_AF_OUT,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_FREQ_HIGH,
        .pupd = GPIO_PUPD_NONE,
        .af = GPIO_AF_5,
    };
    GPIO_InitTypeDef led_config = {
        .pin = GPIO_Pin_6 | GPIO_Pin_7,
        .mode = GPIO_MODE_OUT,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_FREQ_HIGH,
        .pupd = GPIO_PUPD_NONE,
        .af = GPIO_AF_0,
    };

    GPIO_Init(GPIOA, &usart1_config);
    // GPIO_Init(GPIOA, &spi1_config);
    GPIO_Init(GPIOA, &led_config);
}
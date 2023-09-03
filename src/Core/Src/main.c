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

/* private func*/
void LedInit()
{
    GPIO_InitTypeDef led_config = {
        .pin = GPIO_Pin_6 | GPIO_Pin_7,
        .mode = GPIO_MODE_OUT,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_FREQ_HIGH,
        .pupd = GPIO_PUPD_NONE,
        .af = GPIO_AF_0,
    };
    GPIO_Init(GPIOA, &led_config);
}
void UartInit()
{
    GPIO_InitTypeDef usart1_config = {
        .pin = GPIO_Pin_9,
        .mode = GPIO_MODE_AF_OUT,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_FREQ_HIGH,
        .pupd = GPIO_PUPD_PULL_UP,
        .af = GPIO_AF_7,
    };
    GPIO_Init(GPIOA, &usart1_config);
    USART_Init(USART1);
}

int main()
{
    // LedInit();
    UartInit();

    // test_spi();

    while (1)
    {
        delay_ms(3000);
        // GPIO_TogglePin(GPIOA, GPIO_Pin_7 | GPIO_Pin_6);
        log_info("heart beat!\n");
    }

    // vTaskStartScheduler();
}
#include <stdint.h>
#include <stdio.h>

#include "log.h"
#include "tools.h"

#include "stm32f4xx.h"
#include "hal_flash.h"
#include "hal_gpio.h"
#include "hal_rcc.h"
#include "hal_usart.h"

void SystemInit()
{
    FLASH_Init();
    RCC_Init();
}

void GpioInit()
{
    GPIO_InitTypeDef config = {
        .pin = GPIO_Pin_6 | GPIO_Pin_7,
        .mode = GPIO_MODE_OUT,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_FREQ_HIGH,
        .pupd = GPIO_PUPD_NONE,
        .af = GPIO_AF_0,
    };
    GPIO_InitTypeDef config1 = {
        .pin = GPIO_Pin_9,
        .mode = GPIO_MODE_AF_OUT,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_FREQ_HIGH,
        .pupd = GPIO_PUPD_PULL_UP,
        .af = GPIO_AF_7,
    };

    GPIO_Init(GPIOA, &config);
    GPIO_Init(GPIOA, &config1);
}

void task_heart_beat(void *pvParameters)
{
    GPIO_TogglePin(GPIOA, GPIO_Pin_6);
    while (1)
    {
        log_info("heart beat %d ms", HAL_GetTick());
        GPIO_TogglePin(GPIOA, GPIO_Pin_6);
        GPIO_TogglePin(GPIOA, GPIO_Pin_7);
        delay_ms(1000);
    }
}

uint32_t testSVC(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3)
{
    __NOP();
    __ASM __volatile("svc #1\n");
    __NOP();
    return 0;
}

int main()
{
    GpioInit();
    USART_Init(USART1);
    task_heart_beat(NULL);
}

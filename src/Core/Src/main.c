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

void SystemInit()
{
    FLASH_Init();
    RCC_Init();
}

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

    GPIO_Init(GPIOA, &usart1_config);
    GPIO_Init(GPIOA, &spi1_config);
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
    SPI_Init(SPI1);

    uint32_t res = 0;
    uint8_t ucTxBuffer[0x10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF};
    uint8_t ucRxBuffer[0x10] = {0};
    SPI_Transmit(SPI1, ucTxBuffer, ucRxBuffer, 16);

    while (1)
    {
        log_info("heart beat %d ms", HAL_GetTick());
        delay_ms(1000);
    }
}

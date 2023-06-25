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
	RCC_Init();
}

void GpioInit()
{
	// GPIO_InitTypeDef ucGpioConfig[] = {
	// 	{
	// 		.GPIO_Pin = GPIO_Pin_9,
	// 		.GPIO_Mode = GPIO_Mode_AF_PP,
	// 		.GPIO_Speed = GPIO_Speed_50MHz,
	// 	},
	// 	{
	// 		.GPIO_Pin = GPIO_Pin_8,
	// 		.GPIO_Mode = GPIO_Mode_AF_PP,
	// 		.GPIO_Speed = GPIO_Speed_50MHz,
	// 	},
	// 	{
	// 		.GPIO_Pin = GPIO_Pin_13,
	// 		.GPIO_Mode = GPIO_Mode_OUT_PP,
	// 		.GPIO_Speed = GPIO_Speed_50MHz,
	// 	},
	// };

	// GPIO_Init(GPIOA, &ucGpioConfig[0]);
	// GPIO_Init(GPIOA, &ucGpioConfig[1]);
	// GPIO_Init(GPIOC, &ucGpioConfig[2]);
	GPIOA->MODER |= GPIO_MODER_MODER6_0;
	GPIOA->MODER |= GPIO_MODER_MODER7_0;
	GPIOA->ODR = 0x0;

	GPIOA->MODER |= GPIO_MODER_MODER9_1;
	GPIOA->AFR[1] != GPIO_AFRH_AFRH1_0;
	GPIOA->AFR[1] != GPIO_AFRH_AFRH1_1;
	GPIOA->AFR[1] != GPIO_AFRH_AFRH1_2;
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

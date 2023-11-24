#include <stdint.h>
#include <stdio.h>

#include "log.h"
#include "tools.h"

#include "hal_flash.h"
#include "hal_gpio.h"
#include "hal_rcc.h"
#include "hal_spi.h"
#include "hal_usart.h"
#include "stm32f4xx.h"


#include "MinRTOS.h"

/* private func*/
void LedInit() {
  GPIO_InitTypeDef led_config = {
      .pin = GPIO_Pin_13,
      .mode = GPIO_MODE_OUT,
      .otype = GPIO_OTYPE_PP,
      .speed = GPIO_SPEED_FREQ_HIGH,
      .pupd = GPIO_PUPD_NONE,
      .af = GPIO_AF_0,
  };
  GPIO_Init(GPIOC, &led_config);
}
void McoInit() {
  GPIO_InitTypeDef config = {
      .pin = GPIO_Pin_8,
      .mode = GPIO_MODE_AF_OUT,
      .otype = GPIO_OTYPE_PP,
      .speed = GPIO_SPEED_FREQ_HIGH,
      .pupd = GPIO_PUPD_NONE,
      .af = GPIO_AF_0,
  };
  GPIO_Init(GPIOA, &config);
}
void UartInit() {
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

int main() {
  LedInit();
  McoInit();
  UartInit();

  // test_spi();

  // while (1) {
  //   delay_ms(1000);
  //   GPIO_TogglePin(GPIOC, GPIO_Pin_13);
  //   log_info("heart beat!\n");
  // }

  vTaskStartScheduler();
}
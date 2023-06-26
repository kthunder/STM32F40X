#ifndef __HAL_GPIO_H
#define __HAL_GPIO_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f4xx.h"
// GPIO speed def
#define GPIO_SPEED_FREQ_LOW 0x00000000U       /*!< IO works at 2 MHz, please refer to the product datasheet */
#define GPIO_SPEED_FREQ_MEDIUM 0x00000001U    /*!< range 12,5 MHz to 50 MHz, please refer to the product datasheet */
#define GPIO_SPEED_FREQ_HIGH 0x00000002U      /*!< range 25 MHz to 100 MHz, please refer to the product datasheet  */
#define GPIO_SPEED_FREQ_VERY_HIGH 0x00000003U /*!< range 50 MHz to 200 MHz, please refer to the product datasheet  */
// GPIO mode def
#define GPIO_MODE_Pos 0U
#define GPIO_MODE (0x3UL << GPIO_MODE_Pos)
#define MODE_INPUT (0x0UL << GPIO_MODE_Pos)
#define MODE_OUTPUT (0x1UL << GPIO_MODE_Pos)
#define MODE_AF (0x2UL << GPIO_MODE_Pos)
#define MODE_ANALOG (0x3UL << GPIO_MODE_Pos)
#define OUTPUT_TYPE_Pos 4U
#define OUTPUT_TYPE (0x1UL << OUTPUT_TYPE_Pos)
#define OUTPUT_PP (0x0UL << OUTPUT_TYPE_Pos)
#define OUTPUT_OD (0x1UL << OUTPUT_TYPE_Pos)
#define EXTI_MODE_Pos 16U
#define EXTI_MODE (0x3UL << EXTI_MODE_Pos)
#define EXTI_IT (0x1UL << EXTI_MODE_Pos)
#define EXTI_EVT (0x2UL << EXTI_MODE_Pos)
#define TRIGGER_MODE_Pos 20U
#define TRIGGER_MODE (0x7UL << TRIGGER_MODE_Pos)
#define TRIGGER_RISING (0x1UL << TRIGGER_MODE_Pos)
#define TRIGGER_FALLING (0x2UL << TRIGGER_MODE_Pos)
// 模拟输入
// 浮空输入
// 下拉输入
// 上拉输入
// 开漏输出
// 推挽输出
// 复用开漏输出
// 复用推挽输出
#define GPIO_MODE_INPUT MODE_INPUT                    /*!< Input Floating Mode                   */
#define GPIO_MODE_OUTPUT_PP (MODE_OUTPUT | OUTPUT_PP) /*!< Output Push Pull Mode                 */
#define GPIO_MODE_OUTPUT_OD (MODE_OUTPUT | OUTPUT_OD) /*!< Output Open Drain Mode                */
#define GPIO_MODE_AF_PP (MODE_AF | OUTPUT_PP)         /*!< Alternate Function Push Pull Mode     */
#define GPIO_MODE_AF_OD (MODE_AF | OUTPUT_OD)         /*!< Alternate Function Open Drain Mode    */

#define GPIO_MODE_ANALOG MODE_ANALOG /*!< Analog Mode  */

#define GPIO_MODE_IT_RISING (MODE_INPUT | EXTI_IT | TRIGGER_RISING)                           /*!< External Interrupt Mode with Rising edge trigger detection          */
#define GPIO_MODE_IT_FALLING (MODE_INPUT | EXTI_IT | TRIGGER_FALLING)                         /*!< External Interrupt Mode with Falling edge trigger detection         */
#define GPIO_MODE_IT_RISING_FALLING (MODE_INPUT | EXTI_IT | TRIGGER_RISING | TRIGGER_FALLING) /*!< External Interrupt Mode with Rising/Falling edge trigger detection  */

#define GPIO_MODE_EVT_RISING (MODE_INPUT | EXTI_EVT | TRIGGER_RISING)                           /*!< External Event Mode with Rising edge trigger detection             */
#define GPIO_MODE_EVT_FALLING (MODE_INPUT | EXTI_EVT | TRIGGER_FALLING)                         /*!< External Event Mode with Falling edge trigger detection            */
#define GPIO_MODE_EVT_RISING_FALLING (MODE_INPUT | EXTI_EVT | TRIGGER_RISING | TRIGGER_FALLING) /*!< External Event Mode with Rising/Falling edge trigger detection     */
// GPIO pin def
#define GPIO_Pin_0 ((uint16_t)1 << 0)
#define GPIO_Pin_1 ((uint16_t)1 << 1)
#define GPIO_Pin_2 ((uint16_t)1 << 2)
#define GPIO_Pin_3 ((uint16_t)1 << 3)
#define GPIO_Pin_4 ((uint16_t)1 << 4)
#define GPIO_Pin_5 ((uint16_t)1 << 5)
#define GPIO_Pin_6 ((uint16_t)1 << 6)
#define GPIO_Pin_7 ((uint16_t)1 << 7)
#define GPIO_Pin_8 ((uint16_t)1 << 8)
#define GPIO_Pin_9 ((uint16_t)1 << 9)
#define GPIO_Pin_10 ((uint16_t)1 << 10)
#define GPIO_Pin_11 ((uint16_t)1 << 11)
#define GPIO_Pin_12 ((uint16_t)1 << 12)
#define GPIO_Pin_13 ((uint16_t)1 << 13)
#define GPIO_Pin_14 ((uint16_t)1 << 14)
#define GPIO_Pin_15 ((uint16_t)1 << 15)
#define GPIO_Pin_16 ((uint16_t)1 << 16)
#define GPIO_Pin_All ((uint16_t)0xFFFF)
// GPIO AF def
#define GPIO_AF0_RTC_50Hz ((uint8_t)0x00) /* RTC_50Hz Alternate Function mapping                       */
#define GPIO_AF0_MCO ((uint8_t)0x00)      /* MCO (MCO1 and MCO2) Alternate Function mapping            */
#define GPIO_AF0_TAMPER ((uint8_t)0x00)   /* TAMPER (TAMPER_1 and TAMPER_2) Alternate Function mapping */
#define GPIO_AF0_SWJ ((uint8_t)0x00)      /* SWJ (SWD and JTAG) Alternate Function mapping             */
#define GPIO_AF0_TRACE ((uint8_t)0x00)    /* TRACE Alternate Function mapping                          */

/**
 * @brief   AF 1 selection
 */
#define GPIO_AF1_TIM1 ((uint8_t)0x01) /* TIM1 Alternate Function mapping */
#define GPIO_AF1_TIM2 ((uint8_t)0x01) /* TIM2 Alternate Function mapping */

/**
 * @brief   AF 2 selection
 */
#define GPIO_AF2_TIM3 ((uint8_t)0x02) /* TIM3 Alternate Function mapping */
#define GPIO_AF2_TIM4 ((uint8_t)0x02) /* TIM4 Alternate Function mapping */
#define GPIO_AF2_TIM5 ((uint8_t)0x02) /* TIM5 Alternate Function mapping */

/**
 * @brief   AF 3 selection
 */
#define GPIO_AF3_TIM8 ((uint8_t)0x03)  /* TIM8 Alternate Function mapping  */
#define GPIO_AF3_TIM9 ((uint8_t)0x03)  /* TIM9 Alternate Function mapping  */
#define GPIO_AF3_TIM10 ((uint8_t)0x03) /* TIM10 Alternate Function mapping */
#define GPIO_AF3_TIM11 ((uint8_t)0x03) /* TIM11 Alternate Function mapping */

/**
 * @brief   AF 4 selection
 */
#define GPIO_AF4_I2C1 ((uint8_t)0x04) /* I2C1 Alternate Function mapping */
#define GPIO_AF4_I2C2 ((uint8_t)0x04) /* I2C2 Alternate Function mapping */
#define GPIO_AF4_I2C3 ((uint8_t)0x04) /* I2C3 Alternate Function mapping */

/**
 * @brief   AF 5 selection
 */
#define GPIO_AF5_SPI1 ((uint8_t)0x05)    /* SPI1 Alternate Function mapping        */
#define GPIO_AF5_SPI2 ((uint8_t)0x05)    /* SPI2/I2S2 Alternate Function mapping   */
#define GPIO_AF5_I2S3ext ((uint8_t)0x05) /* I2S3ext_SD Alternate Function mapping  */

/**
 * @brief   AF 6 selection
 */
#define GPIO_AF6_SPI3 ((uint8_t)0x06)    /* SPI3/I2S3 Alternate Function mapping  */
#define GPIO_AF6_I2S2ext ((uint8_t)0x06) /* I2S2ext_SD Alternate Function mapping */

/**
 * @brief   AF 7 selection
 */
#define GPIO_AF7_USART1 ((uint8_t)0x07)  /* USART1 Alternate Function mapping     */
#define GPIO_AF7_USART2 ((uint8_t)0x07)  /* USART2 Alternate Function mapping     */
#define GPIO_AF7_USART3 ((uint8_t)0x07)  /* USART3 Alternate Function mapping     */
#define GPIO_AF7_I2S3ext ((uint8_t)0x07) /* I2S3ext_SD Alternate Function mapping */

/**
 * @brief   AF 8 selection
 */
#define GPIO_AF8_UART4 ((uint8_t)0x08)  /* UART4 Alternate Function mapping  */
#define GPIO_AF8_UART5 ((uint8_t)0x08)  /* UART5 Alternate Function mapping  */
#define GPIO_AF8_USART6 ((uint8_t)0x08) /* USART6 Alternate Function mapping */

/**
 * @brief   AF 9 selection
 */
#define GPIO_AF9_CAN1 ((uint8_t)0x09)  /* CAN1 Alternate Function mapping  */
#define GPIO_AF9_CAN2 ((uint8_t)0x09)  /* CAN2 Alternate Function mapping  */
#define GPIO_AF9_TIM12 ((uint8_t)0x09) /* TIM12 Alternate Function mapping */
#define GPIO_AF9_TIM13 ((uint8_t)0x09) /* TIM13 Alternate Function mapping */
#define GPIO_AF9_TIM14 ((uint8_t)0x09) /* TIM14 Alternate Function mapping */

/**
 * @brief   AF 10 selection
 */
#define GPIO_AF10_OTG_FS ((uint8_t)0x0A) /* OTG_FS Alternate Function mapping */
#define GPIO_AF10_OTG_HS ((uint8_t)0x0A) /* OTG_HS Alternate Function mapping */

/**
 * @brief   AF 11 selection
 */
#define GPIO_AF11_ETH ((uint8_t)0x0B) /* ETHERNET Alternate Function mapping */

/**
 * @brief   AF 12 selection
 */
#define GPIO_AF12_FSMC ((uint8_t)0x0C)      /* FSMC Alternate Function mapping                     */
#define GPIO_AF12_OTG_HS_FS ((uint8_t)0x0C) /* OTG HS configured in FS, Alternate Function mapping */
#define GPIO_AF12_SDIO ((uint8_t)0x0C)      /* SDIO Alternate Function mapping                     */

/**
 * @brief   AF 13 selection
 */
#define GPIO_AF13_DCMI ((uint8_t)0x0D) /* DCMI Alternate Function mapping */

/**
 * @brief   AF 15 selection
 */
#define GPIO_AF15_EVENTOUT ((uint8_t)0x0F) /* EVENTOUT Alternate Function mapping */
    // GPIO state def
    typedef enum
    {
        GPIO_PIN_RESET = 0,
        GPIO_PIN_SET
    } GPIO_PinState;

    typedef struct
    {
        uint32_t Pin; /*!< Specifies the GPIO pins to be configured.
                           This parameter can be any value of @ref GPIO_pins_define */

        uint32_t Mode; /*!< Specifies the operating mode for the selected pins.
                            This parameter can be a value of @ref GPIO_mode_define */

        uint32_t Pull; /*!< Specifies the Pull-up or Pull-Down activation for the selected pins.
                            This parameter can be a value of @ref GPIO_pull_define */

        uint32_t Speed; /*!< Specifies the speed for the selected pins.
                             This parameter can be a value of @ref GPIO_speed_define */

        uint32_t Alternate; /*!< Peripheral to be connected to the selected pins.
                                 This parameter can be a value of @ref GPIO_Alternate_function_selection */
    } GPIO_InitTypeDef;

    void GPIO_SetBits(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

    void GPIO_ResetBits(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

    void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

    void GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_InitConfig);

#ifdef __cplusplus
}
#endif

#endif
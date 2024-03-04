#include "tools.h"
#include "log.h"
#include "stm32f4xx.h"
#include <stdint.h>

static volatile uint32_t nSysTicks = 0;

// 使用systick计时
void SysTick_Handler()
{
    nSysTicks++;
}

// TODO : too short
uint32_t HAL_GetTick()
{
    // return HAL_GetUsTick_Prv() / 1000;
    return nSysTicks;
}

void delay_ms(uint32_t ms)
{
    static uint8_t init = 0;
    uint32_t currentUsTick = HAL_GetTick();
    if (!init)
    {
        init++;
        SysTick_Config(84000);
    }

    while ((HAL_GetTick() - currentUsTick) < ms)
        ;
}

void Active_Soft_EXIT(IRQn_Type nIRQ)
{
    NVIC_EnableIRQ(nIRQ);
    SET_BIT(SCB->CCR, SCB_CCR_USERSETMPEND_Msk);
    WRITE_REG(NVIC->STIR, nIRQ);
}

// 使用DWT计时
// 缺点：会占用debug组件
// uint32_t HAL_GetUsTick_Prv()
// {
//     static uint8_t init = 0;

//     if (!init)
//     {
//         init++;
//         // log_info("init");
//         // enable DWT
//         SET_BIT(CoreDebug->DEMCR, CoreDebug_DEMCR_TRCENA_Msk);
//         // clear DWT CYCCNT( not need )
//         DWT->CYCCNT = 0;
//         // enable DWT CYCCNT
//         SET_BIT(DWT->CTRL, DWT_CTRL_CYCCNTENA_Msk);
//     }

//     return DWT->CYCCNT / 84;
// }


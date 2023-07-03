#ifndef __MIN_RTOS_H
#define __MIN_RTOS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "portmacro.h"

    typedef struct tskTaskControlBlock
    {
        volatile portSTACK_TYPE *pxTopOfStack;

        portSTACK_TYPE *pxStack;

        char pcTaskName[16];
    } tskTCB;

#ifdef __cplusplus
}
#endif

#endif /* ___STM32F1xx_HAL_DEF */
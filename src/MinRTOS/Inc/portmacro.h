#ifndef __PORTMACRO_H
#define __PORTMACRO_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>

#define portCHAR char
#define portFLOAT float
#define portDOUBLE double
#define portLONG long
#define portSHORT short
#define portSTACK_TYPE uint32_t
#define portBASE_TYPE long

    typedef uint32_t TickType_t;

#define portMAX_DELAY (TickType_t)0xFFFFFFFFUL

    typedef void (*TaskFunction_t)(void *);

#ifdef __cplusplus
}
#endif

#endif /* ___STM32F1xx_HAL_DEF */
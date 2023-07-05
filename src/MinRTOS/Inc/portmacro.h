#ifndef __PORTMACRO_H
#define __PORTMACRO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

#define portCHAR char
#define portFLOAT float
#define portDOUBLE double
#define portLONG long
#define portSHORT short
#define portSTACK_TYPE uint32_t
#define portBASE_TYPE long

typedef uint32_t TickType_t;

#define portMAX_DELAY (TickType_t)0xFFFFFFFFUL

// 开关中断接口
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 191

typedef void (*TaskFunction_t)(void*);
// 关中断，不带返回值
static inline void vPortRaiseBASEPRI()
{
    uint32_t ulNewBASEPRI = configMAX_SYSCALL_INTERRUPT_PRIORITY;

    __asm volatile("msr basepri, %0  \n"
                   "dsb              \n"
                   "isb              \n"
                   :
                   : "r"(ulNewBASEPRI)
                   : "memory");
}
// 关中断，带返回值
static inline uint32_t ulPortRaiseBASEPRI()
{
    uint32_t ulNewBASEPRI = configMAX_SYSCALL_INTERRUPT_PRIORITY;
    uint32_t ulReturn;

    __asm volatile("mrs %0, basepri  \n"
                   "msr basepri, %1  \n"
                   : "=r"(ulReturn)
                   : "r"(ulNewBASEPRI)
                   : "memory");

    return ulReturn;
}
// 开中断，带参数
static inline void vPortSetBASEPRI(uint32_t ulBASEPRI)
{
    __asm volatile("msr basepri, %0  \n"
                   :
                   : "r"(ulBASEPRI)
                   : "memory");
}

#ifdef __cplusplus
}
#endif

#endif /* ___STM32F1xx_HAL_DEF */
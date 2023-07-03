#include "MinRTOS.h"
#include "cmsis_compiler.h"
#include "task.h"

// tskTCB currentTCB;
// tskTCB *pxCurrentTCB = &currentTCB;

static void TaskExitError()
{
    while (1)
        ;
}

portSTACK_TYPE *pxPortInitStack(portSTACK_TYPE *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters)
{
    pxTopOfStack--;
    *pxTopOfStack = 0x01000000;
    pxTopOfStack--;
    *pxTopOfStack = (portSTACK_TYPE)pxCode & 0xfffffffeUL;
    pxTopOfStack--;
    *pxTopOfStack = (portSTACK_TYPE)TaskExitError;
    pxTopOfStack -= 5;
    *pxTopOfStack = (portSTACK_TYPE)pvParameters;

    pxTopOfStack -= 8;

    return pxTopOfStack;
}

// void InitNewTask(TaskFunction_t pxTaskCode, const char *const pcName, const uint32_t nStackDepth, tskTCB *pxNewTCB)
// {
//     portSTACK_TYPE *pxTopOfStack;

//     pxTopOfStack = pxNewTCB->pxTopOfStack + (nStackDepth - 1);
//     pxTopOfStack = (portSTACK_TYPE *)((uint32_t)pxTopOfStack & (~((uint32_t)7)));

//     for (size_t i = 0; i < 16; i++)
//     {
//         pxNewTCB->pcTaskName[i] = pcName[i];
//     }
//     pxNewTCB->pcTaskName[16 - 1] = '\0';
// }

// 1、多寄存器寻址：
// LDMIA R0!,{R1-R4}
// 等效于
//                                ;R1<----[R0]
//                                ;R2<----[R0+4]
//                                ;R3<----[R0+8]
//                                ;R4<----[R0+12]

void SVC_Handler()
{
    __asm volatile(
        "    ldr    r3, pStackConst2                    \n" /* Restore the context. */
        "    ldr r1, [r3]                         \n" /* Use pxCurrentTCBConst to get the pxCurrentTCB address. */
        "    mov r0, r1                         \n" /* The first item in pxCurrentTCB is the task top of stack. */
        "    ldmia r1!, {r4-r11}                  \n" /* Pop the registers that are not automatically saved on exception entry and the critical nesting count. */
        "    msr psp, r1                          \n" /* Restore the task stack pointer. */
        "    isb                                  \n"
        "    mov r14, #0xFFFFFFFD                 \n"
        "    bx r14                               \n"
        "                                    \n"
        "    .align 4                        \n"
        "pStackConst2: .word pStack                \n");
}

void StartFirstTask()
{
    __asm volatile(
        "cpsie i \n"
        "cpsie f \n"
        "dsb     \n"
        "isb     \n"
        "svc 0   \n"
        "nop     \n");
}
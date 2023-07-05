#include "MinRTOS.h"
#include "cmsis_compiler.h"
#include "log.h"
#include "tools.h"
#define configMAX_PRIORITIES 5

TCB_t *pxCurrentTCB;
List_t pxReadyTaskList[configMAX_PRIORITIES];
portSTACK_TYPE taskStack[512] = {0};
portSTACK_TYPE taskStack2[512] = {0};
TCB_t task1TCB;
TCB_t task2TCB;

/* 在task.h中定义 */
#define taskYIELD() portYIELD()
/* 在portmacro.h中定义 */
/* 中断控制状态寄存器：0xe000ed04
 * Bit 28 PENDSVSET: PendSV 悬起位
 */
#define portNVIC_INT_CTRL_REG (*((volatile uint32_t *)0xe000ed04))
#define portNVIC_PENDSVSET_BIT (1UL << 28UL)

#define portSY_FULL_READ_WRITE (15)

#define portYIELD()                                     \
    {                                                   \
        /* 触发PendSV，产生上下文切换 */      \
        portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT; \
        __asm volatile("dsb"                            \
                       :                                \
                       :                                \
                       : "memory");                     \
        __asm volatile("isb");                          \
    }

void prvInitTaskLists()
{
    for (uint32_t uxPriority = 0; uxPriority < configMAX_PRIORITIES; uxPriority++)
        vListInit(&(pxReadyTaskList[uxPriority]));
}

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
        ".global pxCurrentTCB             \n"
        "    ldr r3, =pxCurrentTCB        \n"
        "    ldr r1, [r3]                 \n"
        "    ldr r0, [r1]                 \n"
        "    ldmia r0!, {r4-r11}          \n"
        "    msr psp, r0                  \n"
        "    isb                          \n"
        "    orr r14, #0xD                \n"
        "    bx r14                       \n");
}

void PendSV_Handler()
{

    __asm volatile(
        ".global pxCurrentTCB             \n"
        "    mrs r0, psp                  \n"
        "    isb                          \n"
        "    ldr r3, =pxCurrentTCB        \n"
        "    ldr r2, [r3]                 \n"
        "    stmdb r0!, {r4-r11}          \n"
        "    str r0, [r2]                 \n"
        "    stmdb sp!, {r3, r14}         \n"
        "    bl vTaskSwitchContext        \n"
        "    ldmia sp!, {r3, r14}         \n"
        "    ldr r1, [r3]                 \n"
        "    ldr r0, [r1]                 \n"
        "    ldmia r0!, {r4-r11}         \n"
        "    msr psp, r0                  \n"
        "    isb                          \n"
        "    bx r14                       \n"
        "    nop                          \n");
}

void prvStartFirstTask()
{
    __asm volatile(
        "    cpsie i \n"
        "    cpsie f \n"
        "    dsb     \n"
        "    isb     \n"
        "    svc 0   \n"
        "    nop     \n");
}

static void prvTaskExitError()
{
    while (1)
        ;
}

static void prvInitNewTask(TaskFunction_t pxTaskCode,
                           const char *const pcName,
                           const uint32_t ulStackDepth,
                           void *const pvParameters,
                           TaskHandle_t *const pxCreatedTask,
                           TCB_t *const pxNewTCB)
{
    portSTACK_TYPE *pxTopOfStack;

    pxTopOfStack = pxNewTCB->pxStack + (ulStackDepth - 1);
    pxTopOfStack = (portSTACK_TYPE *)((uint32_t)pxTopOfStack & (~((uint32_t)7)));
    for (size_t i = 0; i < 16; i++)
    {
        pxNewTCB->pcTaskName[i] = pcName[i];
        if (pcName[i] == '\0')
        {
            break;
        }
    }
    pxNewTCB->pcTaskName[16 - 1] = '\0';

    vListInitListItem(&(pxNewTCB->xStateListItem));
    pxNewTCB->xStateListItem.pvOwner = pxNewTCB;

    pxNewTCB->pxTopOfStack = pxPortInitStack(pxTopOfStack, pxTaskCode, pvParameters);
    if ((void *)pxCreatedTask != NULL)
    {
        *pxCreatedTask = (TaskHandle_t)pxNewTCB;
    }
}

TaskHandle_t xTaskCreateStatic(TaskFunction_t pxTaskCode,
                               const char *const pcName,
                               const uint32_t ulStackDepth,
                               void *const pvParameters,
                               portSTACK_TYPE *const puxStackBuffer,
                               TCB_t *const pxTaskBuffer)
{
    TCB_t *pxNewTCB;
    TaskHandle_t xReturn;

    if ((pxTaskBuffer != NULL) && (puxStackBuffer != NULL))
    {
        pxNewTCB = (TCB_t *)pxTaskBuffer;
        pxNewTCB->pxStack = (portSTACK_TYPE *)puxStackBuffer;

        prvInitNewTask(pxTaskCode, pcName, ulStackDepth, pvParameters, &xReturn, pxNewTCB);
    }
    else
    {
        xReturn = NULL;
    }

    return xReturn;
}

portSTACK_TYPE *pxPortInitStack(portSTACK_TYPE *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters)
{
    pxTopOfStack--;
    *pxTopOfStack = 0x01000000;
    pxTopOfStack--;
    *pxTopOfStack = (portSTACK_TYPE)pxCode & 0xfffffffeUL;
    pxTopOfStack--;
    *pxTopOfStack = (portSTACK_TYPE)prvTaskExitError;
    pxTopOfStack -= 5;
    *pxTopOfStack = (portSTACK_TYPE)pvParameters;

    pxTopOfStack -= 8;
    return pxTopOfStack;
}

void vTaskSwitchContext()
{
    if (pxCurrentTCB == &task1TCB)
    {
        pxCurrentTCB = &task2TCB;
    }
    else
    {
        pxCurrentTCB = &task1TCB;
    }
}

void task(void *param)
{
    // log_info("&task1TCB    %p", &task1TCB);
    // log_info("&task2TCB    %p", &task2TCB);
    // log_info("pxCurrentTCB %p", pxCurrentTCB);
    while (1)
    {
        log_info("heart beat %d ms", HAL_GetTick());
        delay_ms(1000);
        portYIELD();
    }
}
void task2(void *param)
{
    while (1)
    {
        log_info("beat heart %d ms", HAL_GetTick());
        delay_ms(1000);
        portYIELD();
    }
}

void vTaskStartScheduler()
{
    prvInitTaskLists();

    TaskHandle_t handle = xTaskCreateStatic(task, "TASK1", 512, NULL, taskStack, &task1TCB);
    TaskHandle_t handle2 = xTaskCreateStatic(task2, "TASK2", 512, NULL, taskStack2, &task2TCB);
    vListInsert(&pxReadyTaskList[0], &(task1TCB.xStateListItem));
    vListInsert(&pxReadyTaskList[0], &(task2TCB.xStateListItem));

    pxCurrentTCB = &task1TCB;
    log_info("main");
    log_info("&task1TCB    %p", &task1TCB);
    log_info("&task2TCB    %p", &task2TCB);
    log_info("pxCurrentTCB %p", pxCurrentTCB);
    prvStartFirstTask();
}

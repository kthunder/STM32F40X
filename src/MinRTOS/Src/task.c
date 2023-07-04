#include "MinRTOS.h"
#include "cmsis_compiler.h"
#include "log.h"
#define configMAX_PRIORITIES 5

TCB_t *pxCurrentTCB;
List_t pxReadyTaskList[configMAX_PRIORITIES];

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
        "    ldr r0, =pxCurrentTCB        \n"
        "    ldr r0, [r0]                 \n"
        "    ldmia r0!, {r4-r11}          \n"
        "    msr psp, r0                  \n"
        "    isb                          \n"
        "    mov r14, #0xFFFFFFFD         \n"
        "    bx r14                       \n");
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
    uint32_t x;
    log_info("pxTopOfStack0 :%p", pxNewTCB->pxTopOfStack);
    pxTopOfStack = pxNewTCB->pxTopOfStack + (ulStackDepth - 1);
    log_info("pxTopOfStack1 :%p", pxTopOfStack);
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
    log_info("pxTopOfStack2 :%p", pxNewTCB->pxTopOfStack);
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
        log_info("puxStackBuffer :%p", puxStackBuffer);
        log_info("pxTaskBuffer :%p", pxTaskBuffer);
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
    log_info("pxTopOfStack %p", pxTopOfStack);
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

void task(void *param)
{
    while (1)
    {
        log_info("heart beat %d ms", HAL_GetTick());
        delay_ms(1000);
    }
}
portSTACK_TYPE taskStack[128] = {0};
portSTACK_TYPE *pStack = NULL;
void vTaskStartScheduler()
{
    TCB_t task1TCB;
    // TCB_t task2TCB;
    prvInitTaskLists();
    log_info("taskStack :%p", taskStack);
    TaskHandle_t handle = xTaskCreateStatic(task, "TASK1", 128, NULL, taskStack, &task1TCB);
    vListInsert(&pxReadyTaskList[0], &(task1TCB.xStateListItem));

    pxCurrentTCB = &task1TCB;

    prvStartFirstTask();
}

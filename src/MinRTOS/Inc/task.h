#ifndef __TASK_H
#define __TASK_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "MinRTOS.h"

    typedef void *TaskHandle_t;

    typedef struct tskTaskControlBlock
    {
        volatile portSTACK_TYPE *pxTopOfStack;

        ListItem_t xStateListItem;

        portSTACK_TYPE *pxStack;

        char pcTaskName[16];
    } TCB_t;

    portSTACK_TYPE *pxPortInitStack(portSTACK_TYPE *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters);
    TaskHandle_t xTaskCreateStatic(TaskFunction_t pxTaskCode,
                                   const char *const pcName,
                                   const uint32_t ulStackDepth,
                                   void *const pvParameters,
                                   portSTACK_TYPE *const puxStackBuffer,
                                   TCB_t *const pxTaskBuffer);
    void vTaskStartScheduler();

#ifdef __cplusplus
}
#endif

#endif
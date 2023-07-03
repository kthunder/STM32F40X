#ifndef __TASK_H
#define __TASK_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "MinRTOS.h"

    portSTACK_TYPE *pxPortInitStack(portSTACK_TYPE *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters);

    void StartFirstTask();

#ifdef __cplusplus
}
#endif

#endif
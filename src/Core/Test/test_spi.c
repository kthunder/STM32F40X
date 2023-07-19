#include <stdint.h>
#include <stdio.h>
#include "log.h"
#include "tools.h"
static uint8_t ucBuffer[0x1000];
void test_spi()
{
    uint8_t state = 0;
    uint8_t flashId = 0;

    W25Qx_Init();
    W25Qx_GetStatus(1, &state);
    log_info("state1 %02X", state);
    W25Qx_GetStatus(2, &state);
    log_info("state2 %02X", state);
    W25Qx_GetStatus(3, &state);
    log_info("state3 %02X", state);
    W25Qx_Read_ID(&flashId);
    log_info("flashId %02X", flashId);
    W25Qx_Read(ucBuffer,0x00,5);
}
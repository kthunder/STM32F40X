#include <stdint.h>
#include <stdio.h>
#include "log.h"
#include "tools.h"
static uint8_t ucBuffer[0x1000];

void set_buffer()
{
    for (size_t i = 0; i < 0x1000; i++)
    {
        ucBuffer[i] = i;
    }
}

void test_spi()
{
    uint8_t state = 0;
    uint8_t flashId = 0;
    set_buffer();
    W25Qx_Init();

    log_info("state1 %02X", W25Qx_ReadSR(1));
    log_info("flashId %02X", W25Qx_ReadID());

    W25Qx_Read(ucBuffer, 0x00, 5);
    log_hex("ucBuffer", ucBuffer, 5);
}
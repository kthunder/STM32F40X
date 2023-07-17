#include <stdint.h>
#include <stdio.h>

#include "log.h"
#include "tools.h"

#include "stm32f4xx.h"
#include "hal_flash.h"
#include "hal_gpio.h"
#include "hal_spi.h"

#ifndef __W25Q16_H
#define __W25Q16_H

// #include "main.h"

extern uint8_t W25QXX_BUFFER[4096];

// W25X16读写指令表
#define W25X_WriteEnable 0x06
#define W25X_WriteDisable 0x04
#define W25X_ReadStatusReg 0x05
#define W25X_WriteStatusReg 0x01
#define W25X_ReadData 0x03
#define W25X_FastReadData 0x0B
#define W25X_FastReadDual 0x3B
#define W25X_PageProgram 0x02
#define W25X_BlockErase 0xD8
#define W25X_SectorErase 0x20
#define W25X_ChipErase 0xC7
#define W25X_PowerDown 0xB9
#define W25X_ReleasePowerDown 0xAB
#define W25X_DeviceID 0xAB
#define W25X_ManufactDeviceID 0x90
#define W25X_JedecDeviceID 0x9F

// uint16_t  W25QXX_ReadID(void);//读取FLASH ID

// void W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);//读取flash
// void W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);//写入flash

// void W25QXX_Erase_Chip(void);  //整片擦除
// void W25QXX_Erase_Sector(uint32_t Dst_Addr);//扇区擦除

// void W25QXX_PowerDown(void);//进入掉电模式
// void W25QXX_WAKEUP(void);//唤醒

#endif

void test_spi()
{
    uint8_t ucTxBuffer[0x100] = {0};
    uint8_t ucRxBuffer[0x100] = {0};
    GPIO_InitTypeDef spi1_config = {
        .pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7,
        .mode = GPIO_MODE_AF_OUT,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_FREQ_MEDIUM,
        .af = GPIO_AF_5,
        .pupd = GPIO_PUPD_NONE,
    };
    GPIO_InitTypeDef spi1_cs = {
        .pin = GPIO_Pin_4,
        .mode = GPIO_MODE_OUT,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_FREQ_MEDIUM,
        .af = GPIO_AF_0,
        .pupd = GPIO_PUPD_PULL_UP,
    };
    GPIO_Init(GPIOA, &spi1_config);
    GPIO_Init(GPIOA, &spi1_cs);

    SPI_Init(SPI1);

    // for (size_t i = 0; i < 0x100; i++)
    //     ucTxBuffer[i] = i;
    ucTxBuffer[0] = W25X_ReadStatusReg;
    SPI_Transmit(SPI1, ucTxBuffer, ucRxBuffer, 10);
    log_hex("ucTxBuffer", ucTxBuffer, 10);
    log_hex("ucRxBuffer", ucRxBuffer, 10);
}
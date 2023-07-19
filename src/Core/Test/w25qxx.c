#include "W25Qxx.h"
#include "stm32f4xx.h"
#include "hal_flash.h"
#include "hal_gpio.h"
#include "hal_spi.h"
#include "log.h"
#include "tools.h"

static uint8_t ucTxBuffer[0x100] = {0};
static uint8_t ucRxBuffer[0x100] = {0};

// Block 64K
// Sector 4K

// W25X16读写指令表
#define W25X_WriteEnable 0x06
#define W25X_WriteDisable 0x04

#define W25X_ReadStatusReg1 0x05
#define W25X_WriteStatusReg1 0x01
#define W25X_ReadStatusReg2 0x35
#define W25X_WriteStatusReg2 0x31
#define W25X_ReadStatusReg3 0x15
#define W25X_WriteStatusReg3 0x11

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

#define ENABLE_CS() GPIO_ResetBits(GPIOB, GPIO_Pin_0)
#define DISABLE_CS() GPIO_SetBits(GPIOB, GPIO_Pin_0)

uint32_t W25Qx_Init(void)
{
    GPIO_InitTypeDef spi1_config = {
        .pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_3,
        .mode = GPIO_MODE_AF_OUT,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_FREQ_MEDIUM,
        .af = GPIO_AF_5,
        .pupd = GPIO_PUPD_NONE,
    };
    GPIO_InitTypeDef fls_cs = {
        .pin = GPIO_Pin_0,
        .mode = GPIO_MODE_OUT,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_FREQ_MEDIUM,
        .af = GPIO_AF_0,
        .pupd = GPIO_PUPD_PULL_UP,
    };
    GPIO_Init(GPIOB, &spi1_config);
    GPIO_Init(GPIOB, &fls_cs);

    SPI_Init(SPI1);
    return 0;
}

uint32_t W25Qx_GetStatus(uint32_t regNum, uint8_t *regValue)
{
    switch (regNum)
    {
    case 1:
        ucTxBuffer[0] = W25X_ReadStatusReg1;
        break;
    case 2:
        ucTxBuffer[0] = W25X_ReadStatusReg2;
        break;
    case 3:
        ucTxBuffer[0] = W25X_ReadStatusReg3;
        break;
    default:
        return 1;
        break;
    };
    ENABLE_CS();
    SPI_Transmit(SPI1, ucTxBuffer, ucRxBuffer, 10);
    DISABLE_CS();
    // log_hex("ucRxBuffer", ucRxBuffer, 10);
    *regValue = ucRxBuffer[1];
    return 0;
}
uint32_t W25Qx_WriteEnable(void)
{
    ucTxBuffer[0] = W25X_WriteEnable;
    ENABLE_CS();
    SPI_Transmit(SPI1, ucTxBuffer, ucRxBuffer, 10);
    DISABLE_CS();
    // log_hex("ucRxBuffer", ucRxBuffer, 10);
    return 0;
}
uint32_t W25Qx_Read_ID(uint8_t *ID)
{
    ucTxBuffer[0] = W25X_DeviceID;
    ENABLE_CS();
    SPI_Transmit(SPI1, ucTxBuffer, ucRxBuffer, 10);
    DISABLE_CS();
    *ID = ucRxBuffer[4];
    // log_hex("ucRxBuffer", ucRxBuffer, 10);
    return 0;
}

uint32_t W25Qx_Read(uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
    uint8_t cmd[4];

    /* Configure the command */
    ucTxBuffer[0] = W25X_ReadData;
    ucTxBuffer[1] = (uint8_t)(ReadAddr >> 16);
    ucTxBuffer[2] = (uint8_t)(ReadAddr >> 8);
    ucTxBuffer[3] = (uint8_t)(ReadAddr);

    ENABLE_CS();
    SPI_Transmit(SPI1, ucTxBuffer, ucRxBuffer, 10);
    log_hex("ucRxBuffer", ucRxBuffer, 10);
    DISABLE_CS();
    return 0;
}

// uint32_t W25Qx_Write(uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
// {
//     uint8_t cmd[4];
//     uint32_t end_addr, current_size, current_addr;
//     uint32_t tickstart = HAL_GetTick();

//     /* Calculation of the size between the write address and the end of the page */
//     current_addr = 0;

//     while (current_addr <= WriteAddr)
//     {
//         current_addr += W25Q128FV_PAGE_SIZE;
//     }
//     current_size = current_addr - WriteAddr;

//     /* Check if the size of the data is less than the remaining place in the page */
//     if (current_size > Size)
//     {
//         current_size = Size;
//     }

//     /* Initialize the adress variables */
//     current_addr = WriteAddr;
//     end_addr = WriteAddr + Size;

//     /* Perform the write page by page */
//     do
//     {
//         /* Configure the command */
//         cmd[0] = PAGE_PROG_CMD;
//         cmd[1] = (uint8_t)(current_addr >> 16);
//         cmd[2] = (uint8_t)(current_addr >> 8);
//         cmd[3] = (uint8_t)(current_addr);

//         /* Enable write operations */
//         BSP_W25Qx_WriteEnable();

//         W25Qx_Enable();
//         /* Send the command */
//         if (HAL_SPI_Transmit(&hspi2, cmd, 4, W25Qx_TIMEOUT_VALUE) != HAL_OK)
//         {
//             return W25Qx_ERROR;
//         }

//         /* Transmission of the data */
//         if (HAL_SPI_Transmit(&hspi2, pData, current_size, W25Qx_TIMEOUT_VALUE) != HAL_OK)
//         {
//             return W25Qx_ERROR;
//         }
//         W25Qx_Disable();
//         /* Wait the end of Flash writing */
//         while (BSP_W25Qx_GetStatus() == W25Qx_BUSY)
//         {
//             /* Check for the Timeout */
//             if ((HAL_GetTick() - tickstart) > W25Qx_TIMEOUT_VALUE)
//             {
//                 return W25Qx_TIMEOUT;
//             }
//         }

//         /* Update the address and size variables for next page programming */
//         current_addr += current_size;
//         pData += current_size;
//         current_size = ((current_addr + W25Q128FV_PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : W25Q128FV_PAGE_SIZE;
//     } while (current_addr < end_addr);

//     return W25Qx_OK;
// }

// /**
//  * @brief  Erases the specified block of the QSPI memory.
//  * @param  BlockAddress: Block address to erase
//  * @retval QSPI memory status
//  */
// uint8_t BSP_W25Qx_Erase_Block(uint32_t Address)
// {
//     uint8_t cmd[4];
//     uint32_t tickstart = HAL_GetTick();
//     cmd[0] = SECTOR_ERASE_CMD;
//     cmd[1] = (uint8_t)(Address >> 16);
//     cmd[2] = (uint8_t)(Address >> 8);
//     cmd[3] = (uint8_t)(Address);

//     /* Enable write operations */
//     BSP_W25Qx_WriteEnable();

//     /*Select the FLASH: Chip Select low */
//     W25Qx_Enable();
//     /* Send the read ID command */
//     HAL_SPI_Transmit(&hspi2, cmd, 4, W25Qx_TIMEOUT_VALUE);
//     /*Deselect the FLASH: Chip Select high */
//     W25Qx_Disable();

//     /* Wait the end of Flash writing */
//     while (BSP_W25Qx_GetStatus() == W25Qx_BUSY)
//     {
//         /* Check for the Timeout */
//         if ((HAL_GetTick() - tickstart) > W25Q128FV_SECTOR_ERASE_MAX_TIME)
//         {
//             return W25Qx_TIMEOUT;
//         }
//     }
//     return W25Qx_OK;
// }

// /**
//  * @brief  Erases the entire QSPI memory.This function will take a very long time.
//  * @retval QSPI memory status
//  */
// uint8_t BSP_W25Qx_Erase_Chip(void)
// {
//     uint8_t cmd[4];
//     uint32_t tickstart = HAL_GetTick();
//     cmd[0] = SECTOR_ERASE_CMD;

//     /* Enable write operations */
//     BSP_W25Qx_WriteEnable();

//     /*Select the FLASH: Chip Select low */
//     W25Qx_Enable();
//     /* Send the read ID command */
//     HAL_SPI_Transmit(&hspi2, cmd, 1, W25Qx_TIMEOUT_VALUE);
//     /*Deselect the FLASH: Chip Select high */
//     W25Qx_Disable();

//     /* Wait the end of Flash writing */
//     while (BSP_W25Qx_GetStatus() != W25Qx_BUSY)
//     {
//         /* Check for the Timeout */
//         if ((HAL_GetTick() - tickstart) > W25Q128FV_BULK_ERASE_MAX_TIME)
//         {
//             return W25Qx_TIMEOUT;
//         }
//     }
//     return W25Qx_OK;
// }

#ifndef __W25QXX_H
#define __W25QXX_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

  uint32_t W25Qx_Init(void);
  uint32_t W25Qx_GetStatus(uint32_t regNum, uint8_t *regValue);
  uint32_t W25Qx_WriteEnable(void);
  uint32_t W25Qx_Read_ID(uint8_t *ID);
  uint32_t W25Qx_Read(uint8_t *pData, uint32_t ReadAddr, uint32_t Size);
  uint32_t W25Qx_Write(uint8_t *pData, uint32_t WriteAddr, uint32_t Size);
  uint32_t W25Qx_Erase_Block(uint32_t Address);
  uint32_t W25Qx_Erase_Chip(void);

#ifdef __cplusplus
}
#endif

#endif

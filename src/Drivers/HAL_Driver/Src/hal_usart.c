#include "stm32f4xx.h"
#include "hal_usart.h"

void USART_Init(USART_TypeDef *USARTx)
{
    USARTx->BRR = 0x171; // 设置波特率

    SET_BIT(USARTx->CR1, USART_CR1_UE);  // 使能USART
    CLEAR_BIT(USARTx->CR1, USART_CR1_M); // 一个起始位，8个数据位，n个停止位
    SET_BIT(USARTx->CR1, USART_CR1_TE);  // TX使能
    SET_BIT(USARTx->CR1, USART_CR1_RE);  // RX使能
}

uint32_t USART_Transmit(USART_TypeDef *USARTx, uint8_t *pData, uint16_t nSize)
{
    while (nSize > 0)
    {
        while (!READ_BIT(USARTx->SR, USART_SR_TXE))
            ;
        USARTx->DR = *pData++;
        nSize--;
        while (!READ_BIT(USARTx->SR, USART_SR_TC))
            ;
    }
    return nSize;
}
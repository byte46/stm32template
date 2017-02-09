#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_usart.h"
#include <stdio.h>
#include <string.h>

char msg[30] = "USART init\r\n";

void USARTSendDMA(const char *pucBuffer)
{
    GPIOC->BSRR = GPIO_BSRR_BR13; // LED on
    strcpy(msg, pucBuffer);
    DMA1_Channel4->CNDTR = strlen(pucBuffer);
    DMA_Cmd(DMA1_Channel4, ENABLE);
}

void init_DMA_uart(int bufSize)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
 
    /* Configure USART1 Rx (PA10) as input floating */ 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_Cmd(USART1, ENABLE);
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  
    USART_InitStructure.USART_StopBits = USART_StopBits_1;   
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART_InitStructure);
    //USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    //NVIC_EnableIRQ(USART1_IRQn);


	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_InitTypeDef DMA_InitStruct;
	DMA_StructInit(&DMA_InitStruct);
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) &(USART1->DR);
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t) msg;
	DMA_InitStruct.DMA_BufferSize = bufSize;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_Init(DMA1_Channel4, &DMA_InitStruct);

	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	DMA_Cmd(DMA1_Channel4, ENABLE);

	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
	NVIC_EnableIRQ(DMA1_Channel4_IRQn);
}

int main(int argc, char *argv[])
{
    char buf[30] = "";
    GPIO_InitTypeDef GPIO_InitStructure;
    volatile u32 delay;
    volatile u32 counter = 0;

    /* GPIOC Periph clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIOC->BSRR = GPIO_BSRR_BS13;
    init_DMA_uart(12);

    while(1)
    {
        /*
        while(*mp!=0)
        {
        GPIOC->BSRR = GPIO_BSRR_BR13;
            while (!(USART1->SR & USART_SR_TXE)) { } //Waiting for USART
            USART_SendData(USART1, (uint16_t)*mp++);
        }
        GPIOC->BSRR = GPIO_BSRR_BS13;
        mp = msg;
        */
        delay=1000000;
        while(delay)
            delay--;
        snprintf(buf, 30, "Counter is: %ld", counter++);
        USARTSendDMA(buf);
    }
}

void DMA1_Channel4_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA1_IT_TC4);
	DMA_Cmd(DMA1_Channel4, DISABLE);
    GPIOC->BSRR = GPIO_BSRR_BS13; // LED off
}

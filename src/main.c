#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"

int main(int argc, char *argv[])
{
    char msg[] = "Test here!\r\n";
    char *mp = msg;
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    volatile u32 delay;
/* GPIOC Periph clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA |RCC_APB2Periph_USART1, ENABLE);
/* Configure PC12 to mode: slow rise-time, pushpull output */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; // GPIO No. 12
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // slow rise time
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // push-pull output
    GPIO_Init(GPIOC, &GPIO_InitStructure); // GPIOC init

    /* Configure USART1 Tx (PA9) as alternate function push-pull */ 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
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
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    NVIC_EnableIRQ(USART1_IRQn);

    while(1)
    {
        while(*mp!=0)
        {
        GPIOC->BSRR = GPIO_BSRR_BR13;
            while (!(USART1->SR & USART_SR_TXE)) { } //Waiting for USART
            USART_SendData(USART1, (uint16_t)*mp++);
        }
        GPIOC->BSRR = GPIO_BSRR_BS13;
        mp = msg;
        delay=500000;
        while(delay)
            delay--;
    }
}

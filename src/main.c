#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"


int main(void)
{
    GPIO_InitTypeDef gp;
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC->APB2ENR |= ((uint32_t)0x00000010); // GPIOC Enable
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC->APB1ENR |= ((uint32_t)0x00000001); // TIM2 Enable
    gp.GPIO_Pin = GPIO_Pin_13;
    gp.GPIO_Mode = GPIO_Mode_Out_PP;
    gp.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &gp);
    TIM2->PSC = 7200 - 1;
    TIM2->ARR = 1;
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->CR1 |= TIM_CR1_CEN;
    NVIC_EnableIRQ(TIM2_IRQn);

    while(1)
    {
        //GPIOC->BSRR = GPIO_BSRR_BR13; // On
        //GPIOC->BSRR = GPIO_BSRR_BS13; // Off
    }

}

void TIM2_IRQHandler(void)
{
    TIM2->SR &= ~TIM_SR_UIF; 
    //TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    GPIOC->BSRR = GPIO_BSRR_BR13; // On
    GPIOC->BSRR = GPIO_BSRR_BS13; // Off
}

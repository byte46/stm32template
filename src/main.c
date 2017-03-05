#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"


volatile uint16_t previousState;
GPIO_InitTypeDef port;
TIM_TimeBaseInitTypeDef timer;

int main()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);


    GPIO_StructInit(&port);
    port.GPIO_Mode = GPIO_Mode_Out_PP;
    port.GPIO_Pin = GPIO_Pin_13;
    port.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &port);

    TIM_TimeBaseStructInit(&timer);
    timer.TIM_Prescaler = 7200-1;
    timer.TIM_Period = 10000;
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_TimeBaseInit(TIM2, &timer);
    TIM_Cmd(TIM2, ENABLE);
    NVIC_EnableIRQ(TIM2_IRQn);
    while(1)
    {
    }
}

void TIM2_IRQHandler()
{
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // В таком виде — работает…
    if (previousState == 0)
    {
	previousState = 1;
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
	//timer.TIM_Period = 100;
	//TIM_TimeBaseInit(TIM2, &timer);
    //TIM2->SR = ~(TIM_SR_UIF);
    }
    else
    {
	previousState = 0;
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	//timer.TIM_Period = 100;
	//TIM_TimeBaseInit(TIM2, &timer);
    //TIM2->SR = ~(TIM_SR_UIF);
    }
}

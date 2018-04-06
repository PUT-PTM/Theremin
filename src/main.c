#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
//
//
//long EchoTime;  // Czas trwania sygnału ECHO
//int Distance;  // Odległość w centymetrach
//int MaximumRange = 200; // Maksymalna odległość
//int MinimumRange = 2;   // Minimalna odległość
//
//clock_t deltaTime = 0;
int main(void) {
	// Inicjalizacja portu szeregowego

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
//echo
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitTypeDef GPIO_InitStructure1;
//trigger
	GPIO_InitStructure1.GPIO_Pin =
	GPIO_Pin_12 | GPIO_Pin_14;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure1.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure1.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure1);

//	//Timer
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//	TIM_TimeBaseStructure.TIM_Period = 999;
//	TIM_TimeBaseStructure.TIM_Prescaler = 493;
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
//
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure1;
//	TIM_TimeBaseStructure.TIM_Period = 499;
//	TIM_TimeBaseStructure.TIM_Prescaler = 493;
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure1);


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
//10 us
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 9;
	TIM_TimeBaseStructure.TIM_Prescaler = 83;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
//Trigger
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
//Echo	
	GPIO_InitTypeDef GPIO_InitStructure1;
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure1.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure1.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure1);

	TIM_Cmd(TIM4, ENABLE);
//Cykliczne pomiary
	for (;;) {
		if (TIM_GetFlagStatus(TIM4, TIM_FLAG_Update)) {
			GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
			if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_12) == 1) {
				a = 1;
			} else {
				a = 0;
			}
			if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_13) == 1) {
				b = 1;
			} else {
				b = 0;
			}
			TIM_ClearFlag(TIM4, TIM_FLAG_Update);
		}

	}
}

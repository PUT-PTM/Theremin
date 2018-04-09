#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

int a;
int b;

int main(void) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
// Timerek, czestotliwosc 100Hz wydaje sie byc najbardziej przyjazdna dla czujnika. Maksymalna czestotliwosc to 100kHz.
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 999;
	TIM_TimeBaseStructure.TIM_Prescaler = 839;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
//Trigger, podlaczony poki co do zielonej diody, zeby ladnie bylo widac czy idzie sygnal wysoki, czy nie.
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
// Echo, rowniez podlaczone do diody, w tym samym celu.
	GPIO_InitTypeDef GPIO_InitStructure1;
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure1.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure1.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure1);

	TIM_Cmd(TIM4, ENABLE);

	for (;;) {
		if (TIM_GetFlagStatus(TIM4, TIM_FLAG_Update)) {
			GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
//zrobione na potrzeby STM Studio, z racji, ze w jezyku C nie ma typu boolowskiego
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

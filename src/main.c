#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

int a;
int b;
float Distance;
uint16_t TIM2_CNT;

void TIM4_IRQHandler(void) {
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) {
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_12)) {
			TIM_Cmd(TIM2, ENABLE);
		} else if (!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_12)) {
			TIM_Cmd(TIM2, DISABLE);
			TIM2_CNT = TIM2->CNT;
			//Distance = (float) TIM2_CNT / 58;
		}
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
void TIM3_IRQHandler(void) {
	if (TIM_GetITStatus(TIM3, TIM_IT_Update)) {
		GPIO_ToggleBits(GPIOC, GPIO_Pin_8);
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}


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
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15;
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

	/////DRUGI
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
// Timerek, czestotliwosc 100Hz wydaje sie byc najbardziej przyjazdna dla czujnika. Maksymalna czestotliwosc to 100kHz.
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure1;
	TIM_TimeBaseStructure1.TIM_Period = 999;
	TIM_TimeBaseStructure1.TIM_Prescaler = 839;
	TIM_TimeBaseStructure1.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure1.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure1);
//Trigger, podlaczony poki co do zielonej diody, zeby ladnie bylo widac czy idzie sygnal wysoki, czy nie.
	GPIO_InitTypeDef GPIO_InitStructure3;
	GPIO_InitStructure3.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure3.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure3.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure3.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure3.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure3);
// Echo, rowniez podlaczone do diody, w tym samym celu.
	GPIO_InitTypeDef GPIO_InitStructure4;
	GPIO_InitStructure4.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure4.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure4.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure4.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure4.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure4);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure9;
	TIM_TimeBaseStructure9.TIM_Period = 999;
	TIM_TimeBaseStructure9.TIM_Prescaler = 839;
	TIM_TimeBaseStructure9.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure9.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure9);

	NVIC_InitTypeDef NVIC_InitStructure;
	// numer przerwania
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	// priorytet g³ówny
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	// subpriorytet
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	// uruchom dany kana³
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// zapisz wype³nion¹ strukturê do rejestrów
	NVIC_Init(&NVIC_InitStructure);
	// wyczyszczenie przerwania od timera 3 (wyst¹pi³o przy konfiguracji timera)
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	// zezwolenie na przerwania od przepe³nienia dla timera 3
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure1;
	// numer przerwania
	NVIC_InitStructure1.NVIC_IRQChannel = TIM3_IRQn;
	// priorytet g³ówny
	NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 0x00;
	// subpriorytet
	NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 0x00;
	// uruchom dany kana³
	NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;
	// zapisz wype³nion¹ strukturê do rejestrów
	NVIC_Init(&NVIC_InitStructure1);
	// wyczyszczenie przerwania od timera 3 (wyst¹pi³o przy konfiguracji timera)
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	// zezwolenie na przerwania od przepe³nienia dla timera 3
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	TIM_Cmd(TIM4, ENABLE);
	TIM_Cmd(TIM3, ENABLE);

	for (;;)
		;


}

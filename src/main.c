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


	GPIO_SetBits(GPIOD, GPIO_Pin_12);

	while (1) {
		if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_13) == 1) {
			GPIO_SetBits(GPIOD, GPIO_Pin_14);
		} else {
			GPIO_ResetBits(GPIOD, GPIO_Pin_14);
		}
	}
//	// Konfiguracja pinów
//
//	// Ustawiamy TRIG w stan niski na 2us
//	GPIO_SetBits(GPIOD, GPIO_Pin_13);
//
//	// Ustawiamy TRIG w stan wysoki na 10us
//
//
//
//	while (1) {
//
//
////		// Ustawiamy TRIG w stan niski - rozpoczynamy pomiar
////		GPIO_ResetBits(GPIOD, GPIO_Pin_12);
////
////		// Odczytujamy czas trwania stanu wysokiego na pinie ECHO
////		EchoTime = pulseIn(GPIOD, GPIO_Pin_0, 1);
////
////		// Obliczamy odległość
////		Distance = EchoTime / 58;
////
////		// Sprawdzamy zakres pomiarowy
////		if (Distance >= MaximumRange || Distance <= MinimumRange) {
////			printf("Poza zakresem");
////		} else {
////			printf("Odleglosc: ", Distance);
////		}
////
////		// Opóźnienie kolejnego pomiaru
////		for (int i = 0; i = 1000; i++)
////			;
//	}
}

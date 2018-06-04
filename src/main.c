#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "Audio2.h"
double ADC_Result;
double solution;
int iter;
int volume;
unsigned int TIM2_CNT;
unsigned int TIM4_CNT;
float DistanceA;
int DistanceB;
unsigned int a, b, c, d;
unsigned int period;
int i;

void Delay(uint32_t i) 
{
	static uint32_t ij = 0, j = 0;
	for (ij = 0; ij < i; ij++)
		for (j = 0; j < 1; j++)
			;
}
void TIM3_IRQHandler(void) {

	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {

		if (period < 600) {
			for (iter = 0; iter < 2048; iter++)
				DAC_SetChannel1Data(DAC_Align_12b_R, rawAudio[iter] / volume);
		}
	}
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}

void TIM4_IRQHandler(void) {
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) {
		// Sensor 1 to change frequency.
		GPIOD->BSRRH = GPIO_Pin_12;
		Delay(100);
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		GPIOD->BSRRL = GPIO_Pin_12;
		TIM_Cmd(TIM2, ENABLE);
		Delay(500);
		GPIOD->BSRRH = GPIO_Pin_12;

		a = 0;
		GPIOD->BSRRH = GPIO_Pin_14;

		while (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_13) == RESET)
			;
		while (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_13) == SET) {
			a++;
			TIM2_CNT = TIM_GetCounter(TIM2);
			if (a > 100000)
				break;
		}

		b = a;
		DistanceA = b * 0.007;
		GPIOD->BSRRL = GPIO_Pin_14;
		period = ((int) (1 + (DistanceA * 10.0)));
		TIM3->ARR = period;
		Delay(10000);
		//Measurement to change volume.
		GPIOB->BSRRH = GPIO_Pin_0;
		Delay(100);
		GPIOB->BSRRL = GPIO_Pin_0;
		Delay(500);
		GPIOB->BSRRH = GPIO_Pin_0;

		c = 0;

		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == RESET)
			;
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == SET) {
			c++;
			if (c > 100000)
				break;
		}
		d = c;
		DistanceB = d * 0.007;
		if (DistanceB > 50)
			volume = 1000;
		else if (DistanceB <= 50)
			volume = DistanceB * 20;
		Delay(10000);

		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}

int main(void) 
{
	SystemInit();
	period = 10;
	volume = 10.0;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	// Audio pin.
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// Timer 3 to handle DAC and speaker.
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure3;
	TIM_TimeBaseStructure3.TIM_Period = 20;
	TIM_TimeBaseStructure3.TIM_Prescaler = period;
	TIM_TimeBaseStructure3.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure3.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure3);

	NVIC_InitTypeDef NVIC_InitStructure3;
	NVIC_InitStructure3.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure3.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure3.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure3.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure3);
	
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_84Cycles);
	ADC_Cmd(ADC1, ENABLE);

	DAC_InitTypeDef DAC_InitStructure;
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
	DAC_Cmd(DAC_Channel_1, ENABLE);

/////////////////////////////////////////HC-SR04 nr 1////////////////////////////////

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	//Trigger, pinned to green diode, to check signal.
	GPIO_InitTypeDef GPIO_Initq;
	GPIO_Initq.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Initq.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Initq.GPIO_OType = GPIO_OType_PP;
	GPIO_Initq.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Initq.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_Initq);
	// Echo, pinned to orange diode, to check results.
	GPIO_InitTypeDef GPIO_I;
	GPIO_I.GPIO_Pin = GPIO_Pin_13;
	GPIO_I.GPIO_Mode = GPIO_Mode_IN;
	GPIO_I.GPIO_OType = GPIO_OType_PP;
	GPIO_I.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_I.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOD, &GPIO_I);

////Timer 4, frequency 100kHz seems to be the best fitness.
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimerA;
	TIM_TimerA.TIM_Period = 9;
	TIM_TimerA.TIM_Prescaler = 83;
	TIM_TimerA.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimerA.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimerA);
	TIM_Cmd(TIM4, ENABLE);
	NVIC_InitTypeDef NVIC_PrzerwanieA;
	NVIC_PrzerwanieA.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_PrzerwanieA.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_PrzerwanieA.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_PrzerwanieA.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_PrzerwanieA);
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
/////////////////////////////////////////HC-SR04 nr 2////////////////////////////////

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

////    Trigger
	GPIO_InitTypeDef GPIO_TriggerB;
	GPIO_TriggerB.GPIO_Pin = GPIO_Pin_0;
	GPIO_TriggerB.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_TriggerB.GPIO_OType = GPIO_OType_PP;
	GPIO_TriggerB.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_TriggerB.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_TriggerB);

////    Echo
	GPIO_InitTypeDef GPIO_EchoB;
	GPIO_EchoB.GPIO_Pin = GPIO_Pin_1;
	GPIO_EchoB.GPIO_Mode = GPIO_Mode_IN;
	GPIO_EchoB.GPIO_OType = GPIO_OType_PP;
	GPIO_EchoB.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_EchoB.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_EchoB);

	iter = 0;
	TIM_Cmd(TIM3, ENABLE);
	for (;;) 
	{	
		ADC_SoftwareStartConv(ADC1);
		while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
			;
		ADC_Result = ADC_GetConversionValue(ADC1);
	}
}
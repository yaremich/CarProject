#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "Servo.h"
#include "SPI.h"

void ServoInit(void)
{
	GPIO_InitTypeDef port;
	TIM_TimeBaseInitTypeDef timer; 
	TIM_OCInitTypeDef timerPWM; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	port.GPIO_Mode = GPIO_Mode_AF_PP;
	port.GPIO_Pin = GPIO_Pin_0; 
	port.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &port);

	TIM_TimeBaseStructInit(&timer);
	timer.TIM_ClockDivision = TIM_CKD_DIV1;
	timer.TIM_CounterMode = TIM_CounterMode_Up;   
	timer.TIM_Prescaler = 1440-1;
	timer.TIM_Period = 1000;     
	TIM_TimeBaseInit(TIM3, &timer);

	TIM_OCStructInit(&timerPWM);
	timerPWM.TIM_Pulse = 75;   
	timerPWM.TIM_OCMode = TIM_OCMode_PWM1; 
	timerPWM.TIM_OutputState = TIM_OutputState_Enable; 
	timerPWM.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC3Init(TIM3,&timerPWM);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_CtrlPWMOutputs(TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

void Forward(void)
{
	TIM_SetCompare3(TIM3, 75);
}

void TurnRight(void)
{
	TIM_SetCompare3(TIM3, 61);
}

void TurnLeft(void)
{
	TIM_SetCompare3(TIM3, 89);
}




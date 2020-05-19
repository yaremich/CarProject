#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#include "Indicator.h"

void IndicatorInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

void IndicatorBlink(void)
{
  GPIO_SetBits(GPIOC, GPIO_Pin_13);
  for(int i = 0; i < 99999; i++);
  GPIO_ResetBits(GPIOC, GPIO_Pin_13);
  for(int i = 0; i < 99999; i++);
}

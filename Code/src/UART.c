#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "UART.h"

void UartInit(void)
{
   GPIO_InitTypeDef gpio_USART;
   GPIO_StructInit(&gpio_USART);  
 
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
 
   gpio_USART.GPIO_Pin = GPIO_Pin_9;
   gpio_USART.GPIO_Mode = GPIO_Mode_AF_PP;
   gpio_USART.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &gpio_USART);
 
   gpio_USART.GPIO_Pin = GPIO_Pin_10 ;
   gpio_USART.GPIO_Mode = GPIO_Mode_IN_FLOATING;
   GPIO_Init(GPIOA, &gpio_USART);
 
   USART_InitTypeDef husart1;
   USART_StructInit(&husart1);
 
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
 
   husart1.USART_BaudRate = 115200;
   husart1.USART_WordLength = USART_WordLength_8b ; 
   husart1.USART_StopBits = USART_StopBits_1;
   husart1.USART_Parity = USART_Parity_No;
   husart1.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
   husart1.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   
   USART_Init(USART1,&husart1);
   USART_Cmd(USART1,ENABLE);
}

void USARTSend(char *pucBuffer)
{
	while (*pucBuffer)
		{
			USART_SendData(USART1, *pucBuffer++);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){}
		}
}
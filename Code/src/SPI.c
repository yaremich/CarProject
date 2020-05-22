#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_dma.h"
#include "misc.h"
#include "string.h"
#include "SPI.h"
#include "UART.h"
#include "Servo.h"
#include "Engine.h"


#define RX_BUF_SIZE 8

char RXi;
char RXc;
char RX_FLAG_END_LINE = 0;
char RX_BUF[RX_BUF_SIZE] = {'\0'};

void clear_RXBuffer(void)
{
	for(RXi=0; RXi<RX_BUF_SIZE; RXi++)
	{
		RX_BUF[RXi] = '\0';
	}
	RXi = 0;
}

void RPCconnectionInit(){
	GPIO_InitTypeDef port;
	GPIO_InitTypeDef portSS;
	SPI_InitTypeDef spi;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	GPIO_StructInit(&port);
	port.GPIO_Mode = GPIO_Mode_AF_PP;
	port.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	port.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &port);

	GPIO_StructInit(&portSS);
	portSS.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	portSS.GPIO_Pin = GPIO_Pin_4;
	portSS.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &portSS);

	SPI_StructInit(&spi);
	spi.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	spi.SPI_Mode = SPI_Mode_Slave;
	spi.SPI_DataSize = SPI_DataSize_8b;
	spi.SPI_CPOL = SPI_CPOL_Low;
	spi.SPI_CPHA = SPI_CPHA_1Edge;
	spi.SPI_NSS = SPI_NSS_Soft;
	spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	spi.SPI_FirstBit = SPI_FirstBit_MSB;
	spi.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &spi);
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE , ENABLE);

	NVIC_InitTypeDef nvicStructure;
	nvicStructure.NVIC_IRQChannel = SPI1_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure.NVIC_IRQChannelSubPriority = 1;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);
	SPI_Cmd(SPI1, ENABLE);
}
static int counter = 0;
char* right = "right";
char* left = "left";
char* stop = "stop";

int sign;
int temp()
{
	return sign;
}

void SPI1_IRQHandler(void)
{
	if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE))
		{
			uint8_t data;
			data = SPI_I2S_ReceiveData(SPI1);
			RX_BUF[counter++] = data;
			
		if(data == '\0')
			{
				if(strcmp(RX_BUF, right) == 0)
					{
						sign = 1;
						//TurnRight();
						//EngineStart();
						USART_SendData(USART1,(uint16_t)'R');						
					}
					
				if(strcmp(RX_BUF, left) == 0)
					{
						sign = 0;
						//TurnLeft();	
						//EngineStart();
						USART_SendData(USART1,(uint16_t)'L');					
					}
					
				if(strcmp(RX_BUF, stop) == 0)
					{
						sign = 2;
					//Forward();
					//EngineStop();
						USART_SendData(USART1,(uint16_t)'S');				
					}
				counter = 0;
				clear_RXBuffer();
			}
			while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY)!= RESET);
					SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_RXNE);
		}
}
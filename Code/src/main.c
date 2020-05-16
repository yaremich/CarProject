#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"

#include "Servo.c"
#include "Servo.h"

#include "Engine.c"
#include "Engine.h"

#include "SPI.c"
#include "SPI.h"


GPIO_InitTypeDef port;
SPI_InitTypeDef spi;
uint8_t data;
uint8_t sendData;
uint16_t counter;
uint16_t Data;




void SetSysClockTo72(void)
{
    ErrorStatus HSEStartUpStatus;

    RCC_DeInit();
    RCC_HSEConfig( RCC_HSE_ON);
	
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
    if (HSEStartUpStatus == SUCCESS)
    {
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        RCC_PCLK2Config(RCC_HCLK_Div1);
        RCC_PCLK1Config(RCC_HCLK_Div2);
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

        RCC_PLLCmd(ENABLE);
 
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {
        }
				
        RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK);
 
        while (RCC_GetSYSCLKSource() != 0x08)
        {
        }
    }
}
 


int main()
{
		   __enable_irq();	
    initAll();
    SPI_Cmd(SPI1, ENABLE);
	  char sendData = 'H';
    SPI_I2S_SendData(SPI1, (uint8_t) sendData);	
	  /////f 
    initAll();
	  SPI_Cmd(SPI1, ENABLE);
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
    while(1)
		{
       data = SPI_I2S_ReceiveData(SPI1);
			 if (data == 'H')
			 {
				 GPIO_ResetBits(GPIOC, GPIO_Pin_13);
				 for(int i = 0; i < 160000; ++i){}
				 GPIO_SetBits(GPIOC, GPIO_Pin_13);
				 for(int i = 0; i < 160000; ++i){}
				 }
			 //for(int i = 0; i < 160000; --i){}
			 }	
		
	
	
	SetSysClockTo72();
	
	ServoInit();
	
	while(1)
	{
		TIM_SetCompare4(TIM4, 61);
		TIM_SetCompare4(TIM4, 75);
		TIM_SetCompare4(TIM4, 89);
	}
}





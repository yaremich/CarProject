#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_usart.h"
#include "string.h"

#include "Servo.h"

#include "Engine.h"

#include "Indicator.h"

#include "UART.h"

#include "SPI.h"

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
	SetSysClockTo72();
	
	RPCconnectionInit();
	
	ServoInit();
	
	UartInit();
	
	EngineInit();
	
	IndicatorInit();
  
	EngineStart();
	
	char* start = "start";
	USARTSend(start);	
	
	while(1)
	{	
		if(temp() == 1)
			{
				USART_SendData(USART1,(uint16_t)'R');
				TurnRight();			
			}	
		else if(temp() == 2)
			{
				USART_SendData(USART1,(uint16_t)'S');
				EngineStop();
				Forward();
			
			}
		else if(temp() == 0)
			{
				USART_SendData(USART1,(uint16_t)'L');
				TurnLeft();	
			}
			
	for(int i = 0; i < 99999999; i++);
	//EngineStart();
	Forward();
			
	IndicatorBlink();
		
	}
}


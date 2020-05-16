#ifndef USART_DEBUG_H
#define USART_DEBUG_H

void clear_RXBuffer(void);
void usart_init();
void USARTSend(char *pucBuffer);
void USART1_IRQHandler(void);

#endif // USART_DEBUG_H
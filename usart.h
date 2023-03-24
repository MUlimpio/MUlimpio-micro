#ifndef USART_H
#define USART_H

#include <stdint.h>
#include <stm32f407xx.h>

typedef enum e_Com {COM1, COM2} COM;

void USART_init(COM com, uint32_t baudRate);
void USART_Send(COM com, uint8_t a);
void USART_SendString(COM com, uint8_t *pMsg, uint32_t len);
uint8_t USART_Receive(COM com);
uint8_t USART_readString(COM com, uint8_t *pMsg, uint32_t maxLen);
void USART_enableInterrupt(COM com);

#endif

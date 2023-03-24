#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include "ourRccGpio.h"
#include "usart.h"
#include "systick.h"

#define USED_COM_PORT COM1
#define BAUD_RATE 9600

#define T 10
#define PIN_BUZZER 1
#define LED2 8

void initExtiBuzzer(void);
void initLed(void);
void initSensor(void);
void doBuz(void);

uint32_t times = 0;
uint32_t opcion = 0;

int main(){ 
	USART_init(USED_COM_PORT, 9600);
	initExtiBuzzer();
	initLed();
	initSensor();
	//initSystick(1000); //SIMULACION CADA 10S --> MS = ?
	
	while(1){
		if(!(GPIOA->IDR & 1))USART_Send(USED_COM_PORT,'1');
		if(!(GPIOE->IDR & 1))USART_Send(USED_COM_PORT,'2');
		for(int i=0;i<20000;i++);
				
		if(times == 1) doBuz();
		else if(times > 1){
			if(times == 10)times = 0;
		}
	}
}

void initSensor(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, 1);
	GPIOA->MODER &=~ (0x03);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, 1);
	GPIOE->MODER &=~ (0x03);
}

void initLed(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, 1);
	GPIOF->MODER &=~(0x03<<LED2*2);
	GPIOF->MODER |= (0x01<<LED2*2);
}

void initExtiBuzzer(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, 1);
	GPIOB->MODER &=~(0x03<<PIN_BUZZER*2);
	GPIOB->MODER |= (0x01<<PIN_BUZZER*2);
}

void doSum(void){
	times++;
}

void doBuz(void){
	uint32_t tON = 0;
	
	for(int i = 0; i <= 1000; i++) {
			GPIOB->ODR |= (0x01<<PIN_BUZZER);
			wait(tON);
			GPIOB->ODR &=~(0x01<<PIN_BUZZER);
			wait(T - tON);
		
		if(tON == 5) tON = 0;
		else tON++;
		}
}

void doException(void){
	uint8_t temp = USART_Receive(USED_COM_PORT);
	GPIOF->ODR |= (0x01<<LED2);
}

#include "usart.h"
#include "ourRccGpio.h"

void USART_init(COM com, uint32_t baudRate){
	
	if(com == COM1){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, 1);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, 1);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, 1);

		/*USART6_TX --> PC6
		  USART6_RX --> PG9
		*/
		GPIOC->MODER &=~(0x03<<6*2);
		GPIOC->MODER |= (0x02<<6*2); //Bits(7:6) = 10 --> Alternate
		GPIOG->MODER &=~(0x03<<9*2);
		GPIOG->MODER |= (0x02<<9*2); //Bits(10:9) = 10 --> Alternate
		GPIOC->OSPEEDR |= (0x03<<6*2); //HIGH SPEED PC6
	  GPIOG->OSPEEDR |= (0x03<<9*2); //HIGH SPEED PG9
		
		/*PAG 274 en ReferenceManual
			Como debemos configurar los pines 6 y 9, y utilizamos el USART6. Debemos configurar AFRL = AFR[0] y  AFRH = AFR[1], 
			poniendo en los pines AF8 
		*/
		GPIOC->AFR[0] &=~(0xF<<(6*4)); //cada pin 4
		GPIOC->AFR[0] |= (0x08<<(6*4));
		GPIOG->AFR[1] &=~(0x0F<<((9-8)*4));
		GPIOG->AFR[1] |=(0x08<<((9-8)*4));
		
		USART_enableInterrupt(com); //RX enable exception
		
		//CONFIGURACION USART
		USART6->CR1 |=(0x01<<3); //TX en
		USART6->CR1 |= (0x01<<2); //RX en
		USART6->CR1 &=~(0x01<<12); //M --> 0: 1 Start bit, 8 Data bits, n Stop bit
		USART6->CR2 = 0x00000; //STOP 1
		USART6->CR3 = 0x00000; //NO multi
		/*
		El reloj del sistema es 16 MHz. 
		Defina la velocidad en baudios = 9600, fPCLK2 = 16MHz, luego: 
		1) Calcule USARTDIV = 16MHz / 8*2*9600 = 104.16 --} ENTERO = 104 | FRACCION = 0.16
		2) Tome el entero 104 = 0x68. 
		3) La fracción 0,16 * 16 = 2.5
	  Entonces, el valor escrito en el registro USART_BRR es: USART_BRR = (0x68<<4) + 2;
		*/
		USART6->BRR = (0x68<<4) + 2;
		USART6->CR1 |= (0x01<<13); //en USART
	}
	
	if(com == COM2){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, 1);
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, 1); 
	
	/*USART3_TX --> PD8
		USART3_RX --> PD9
	*/
	GPIOD->MODER &=~(0x03<<8*2);
	GPIOD->MODER |= (0x02<<8*2); //Bits(9:8) = 10 --> Alternate
	GPIOD->MODER &=~(0x03<<9*2);
	GPIOD->MODER |= (0x02<<9*2); //Bits(9:10) = 10 --> Alternate
	GPIOD->OSPEEDR |= (0x03<<8*2); //HIGH SPEED PA2
	GPIOD->OSPEEDR |= (0x03<<9*2); //HIGH SPEED PA3
	
	GPIOD->AFR[0] |= (0x07<<(8-8)*4); //PIN2 --> AF7
	GPIOD->AFR[0] |= (0x07<<(9-8)*4); //PIN3*/
		
	USART_enableInterrupt(com); 
	
	USART3->CR1 |= (1<<2); 
	USART3->CR1 |= (1<<3); 
	USART3->CR1 &=~(0x01<<12);
	USART3->CR2 = 0x00000; 
	USART3->CR3 = 0x00000;
	USART3->BRR |= (0x68<<4) + 2;
	USART3->CR1 |= (0x01<<13);
	}
}

void USART_Send(COM com, uint8_t a){
	if(com == COM1){
		while (!(USART6->SR & (1<<7))); //data vacio
		USART6->DR = a; //cargar valor a enviar en DR
		while (!(USART6->SR & (1<<6)));  //esperar hasta transmision completa
	}
	if(com == COM2){
		while (!(USART3->SR & (1<<7))); 
		USART3->DR = a; 
		while (!(USART3->SR & (1<<6))); 
	}
}
void USART_SendString(COM com, uint8_t *string, uint32_t len){
	for(uint32_t i=0;i<len;i++)USART_Send(com, string[i]);
}

uint8_t USART_Receive(COM com){
	uint8_t temp;
	
	if(com == COM1){
		while(!(USART6->SR & (1<<5))); //==1 -> data received
		temp = USART6->DR; //read	
	}
	
	if(com == COM2){
		while(!(USART3->SR & (1<<5))); 
		temp = USART3->DR; 	
	}
	
	return temp;
}
void USART_enableInterrupt(COM com){
	RCC->APB2ENR |= (0x01<<14);
	if(com == COM2)SYSCFG->EXTICR[2] |= (0x03<<4); //GPIOD
	else if(com == COM1) SYSCFG->EXTICR[2] |= (0x06<<4); //SYSCDFG3 --> PIN 9(RX), GPIOG
	EXTI->IMR |= (0x01<<9);
	EXTI->FTSR |= (0x01<<9);
	NVIC->ISER[0] |= (0x01<<(23%32));	
}

#ifndef OUR_RCC_H
#define OUR_RCC_H

#include <stdint.h>
#include <stm32f407xx.h>

#define RCC_AHB1Periph_GPIOA ((uint32_t)0x01)
#define RCC_AHB1Periph_GPIOB ((uint32_t)(0x01<<1))
#define RCC_AHB1Periph_GPIOC ((uint32_t)(0x01<<2))
#define RCC_AHB1Periph_GPIOD ((uint32_t)(0x01<<3))
#define RCC_AHB1Periph_GPIOE ((uint32_t)(0x01<<4))
#define RCC_AHB1Periph_GPIOF ((uint32_t)(0x01<<5))
#define RCC_AHB1Periph_GPIOG ((uint32_t)(0x01<<6))
#define RCC_APB2Periph_SYSCFG ((uint32_t)(0x01<<14))
#define RCC_APB1Periph_USART3 ((uint32_t)(0x01<<18))
#define RCC_APB2Periph_USART6 ((uint32_t)(0x01<<5))

void RCC_AHB1PeriphClockCmd(uint32_t nPeriph, uint32_t on);
void RCC_APB1PeriphClockCmd(uint32_t nPeriph, uint32_t on);
void RCC_APB2PeriphClockCmd(uint32_t nPeriph, uint32_t on);

typedef enum
{ 
  GPIO_Mode_IN   = 0x00, /*!< GPIO Input Mode */
  GPIO_Mode_OUT  = 0x01, /*!< GPIO Output Mode */
  GPIO_Mode_AF   = 0x02, /*!< GPIO Alternate function Mode */
  GPIO_Mode_AN   = 0x03  /*!< GPIO Analog Mode */
}GPIOMode_Type;


void initGpioPinMode(GPIO_TypeDef *, uint32_t pin, GPIOMode_Type mode);
void togleGpioPinValue(GPIO_TypeDef *, uint32_t pin);
void setGpioPinValue(GPIO_TypeDef *, uint32_t pin, uint32_t value);
void setGpioPinAF(GPIO_TypeDef *, uint32_t pin, uint32_t AF);
uint32_t getGpioPinValue(GPIO_TypeDef *, uint32_t pin);

#endif

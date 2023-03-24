#include "systick.h"
#include "ourRccGpio.h"

void initSystick(uint32_t ms){
	if(ms){
		SysTick->LOAD = 16000000U/1000*ms - 1U;
		SysTick->VAL = 0U;
		SysTick->CTRL &=~(0x07);
		SysTick->CTRL |= (0x07);
	}	
}

void wait(int ms) {
    for(int i = 0; i < (100 * ms); i++);
}

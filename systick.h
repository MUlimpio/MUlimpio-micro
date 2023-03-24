#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

void initSystick(uint32_t ms);
void wait(int ms);

#endif

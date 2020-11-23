#ifndef UTIL_H
#define UTIL_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/systick.h>


static volatile uint64_t _millis = 0;
void delay(uint64_t duration);
void systick_setup();
uint64_t millis();
void sys_tick_handler(void);
void usart_printf();



#endif // UTIL_H

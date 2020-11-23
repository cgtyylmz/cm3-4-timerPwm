#include "util.h"
#include <libopencm3/stm32/usart.h>

void delay(uint64_t duration)
{
    const uint64_t until = millis() + 5*duration;
    while (millis() < until);
}

void systick_setup()
{
    // Set the systick clock source to our main clock
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    // Clear the Current Value Register so that we start at 0
    STK_CVR = 0;
    // In order to trigger an interrupt every millisecond, we can set the reload
    // value to be the speed of the processor / 1000 -1
    systick_set_reload(rcc_ahb_frequency / 1000 - 1);
    // Enable interrupts from the system tick clock
    systick_interrupt_enable();
    // Enable the system tick counter
    systick_counter_enable();
}

uint64_t millis()
{
    return _millis;
}

void sys_tick_handler(void)
{
    // Increment our monotonic clock
    _millis++;
}

void usart_printf(uint8_t *s)
{
	while(*s)
	{
		usart_send_blocking(USART2, *s);
		s++;
	}
}

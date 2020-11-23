#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/timer.h>

#include "util.h"

#define LED_PORT GPIOB
#define BLUE_LED GPIO3
#define ORAGE_LED GPIO4

#define PWM_FREQ 500


void init_usart()
{
	usart_set_baudrate(USART2, 9600);
	usart_set_databits(USART2, 8);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_stopbits(USART2, USART_CR2_STOPBITS_1);
	usart_set_mode(USART2, USART_MODE_TX_RX);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
	usart_enable(USART2);
}

void init_clk(void)
{
	// Set HSE
	rcc_clock_setup_in_hse_8mhz_out_48mhz();

	// GPIO Clock Enable
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOA);

	// USART1 Clock Enable
	rcc_periph_clock_enable(RCC_USART2);

	//TIMERs Clok Enable
	rcc_periph_clock_enable(RCC_TIM2);
	rcc_periph_clock_enable(RCC_TIM3);

}

void init_gpio(void)
{
	// LED Pins
	gpio_mode_setup(LED_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, ORAGE_LED);
	gpio_mode_setup(LED_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, BLUE_LED);

	// TIMER PWM CHANNEL

	//	- PB3 Timer2 Channel 2 (AF2)
	//	- PB4 Timer3 Channel 1 (AF1)
	//	*ref:DS9826 Rev 6 page:45
	gpio_set_af(GPIOB, GPIO_AF2, BLUE_LED);
	gpio_set_af(GPIOB, GPIO_AF1, ORAGE_LED);



	// USART2 PINS
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO3);
	gpio_set_af(GPIOA, GPIO_AF1, GPIO2);
	gpio_set_af(GPIOA, GPIO_AF1, GPIO3);

}

void init_timer(void)
{
	// Timer2 Settings
	timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_CENTER_1, TIM_CR1_DIR_UP);
	timer_set_prescaler(TIM2, 48);
	timer_set_period(TIM2, 1000000/PWM_FREQ);
	timer_enable_break_main_output(TIM2);

	// Timer3 Settings
	timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_CENTER_1, TIM_CR1_DIR_UP);
	timer_set_prescaler(TIM3, 48);
	timer_set_period(TIM3, 1000000/PWM_FREQ);
	timer_enable_break_main_output(TIM3);

	// PB3 Timer2 PWM Output Channel 2
	timer_enable_oc_output(TIM2, TIM_OC2);
	timer_set_oc_mode(TIM2, TIM_OC2, TIM_OCM_PWM2);

	// PB4 Timer3 PWM output Channel 1
	timer_enable_oc_output(TIM3, TIM_OC1);
	timer_set_oc_mode(TIM3, TIM_OC1, TIM_OCM_PWM2);

	// Enable TIMERs
	timer_enable_counter(TIM2);
	timer_enable_counter(TIM3);

	// Set PWM Duty
	timer_set_oc_value(TIM2, TIM_OC2, 0);
	timer_set_oc_value(TIM3, TIM_OC1, 0);
}

int main()
{
	systick_setup();
	init_clk();
	init_gpio();
	init_usart();
	init_timer();

	int pwm = 0;


	while(1)
	{
		while(pwm < 2000)
		{
			timer_set_oc_value(TIM2, TIM_OC2, pwm);
			timer_set_oc_value(TIM3, TIM_OC1, 2000-pwm);

			pwm += 10;
			delay(10);
		}

		while(pwm > 0)
		{
			timer_set_oc_value(TIM2, TIM_OC2, pwm);
			timer_set_oc_value(TIM3, TIM_OC1, 2000-pwm);

			pwm -= 10;

			delay(10);
		}

	}
	return 0;
}

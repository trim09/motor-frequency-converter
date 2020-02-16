#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "config.h"
#include "led.h"
#include "break.h"
#include "adc.h"
#include "dac.h"
#include "uart.h"
#include "time.h"
#include "switch.h"
#include "control/timer0.h"
#include "space_vector/psc.h"


#ifdef DEBUG
extern volatile uint16_t test_overcurrent;
extern volatile int16_t  test_command;
extern volatile int16_t  test_amplitude;

int16_t toFreq(int16_t command) {
	return ((int32_t)command * F_MAX) / COMMAND_MAX;
}

void print_info(void) {
	static uint16_t counter;

	static const char TIME[] PROGMEM = "Time: %lu [ticks]\n";
	static const char COUNTER[] PROGMEM = "info number: %u\n";
	static const char OVERCURRENT[] PROGMEM = "overcurrent loops: %u\n";
	static const char CURRENT[] PROGMEM = "current: %u\n";
	static const char VOLTAGE[] PROGMEM = "voltage: %u\n";
	static const char FREQ_POT[] PROGMEM = "frequency (speed potentiometer): %i [Hz]\n";
	static const char FREQ_REGUL[] PROGMEM = "frequency (speed regulation): %i [Hz]\n";
	static const char AMPL[] PROGMEM = "amplitude: %u%% (%u [V])\n\n";

	printf_P(TIME, time_get());
	printf_P(COUNTER, ++counter);
	printf_P(OVERCURRENT, test_overcurrent);
	printf_P(CURRENT, adc_get_current_value());
	printf_P(VOLTAGE, adc_get_voltage_value());
	printf_P(FREQ_POT, toFreq(adc_get_speed_pot_value()));
	printf_P(FREQ_REGUL, toFreq(test_command));
	printf_P(AMPL, (uint16_t)(((uint32_t)test_amplitude * 100) / MAX_PWM),
			(uint16_t)(((uint32_t)test_amplitude * 325) / MAX_PWM));
}
#else
void print_info(void) {
	return;
}
#endif


int main(void) {
	PORTB = 0xFF; /* enable pull up on PORTB */
	PORTC = 0xFF; /* enable pull up on PORTC */
	PORTD = 0xFF; /* enable pull up on PORTD */
	PORTE = 0xFF; /* enable pull up on PORTE */

	time_init();
	switch_init();
	break_init();
	dac_init();
	uart_init();
	led_init();
	adc_init();
	psc_init(MAX_PWM, DEADTIME);
	timer0_init();

	sei(); /* allow interruptions */

	while (1) {
        led1_toggle();
		led2_set_state(switch_get_state()); /* TODO - can be removed */
		print_info();
		_delay_ms(250);
	}
}




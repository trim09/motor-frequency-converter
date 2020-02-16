#include "assert.h"
#include "uart.h"
#include "space_vector/psc.h"
#include "led.h"
#include "break.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

void print_info(void);

void assert_handle(int line) {
	cli();
	psc_halt();
	break_off();
	led_init();
	print_info();

#ifdef DEBUG
	static const char ASSERTION_FAILED[] PROGMEM = "Assertion failed at line: %d\n";
	printf_P(ASSERTION_FAILED, line);
#else
	static const char ASSERTION_FAILED[] PROGMEM = "Assertion failed at line: ";
	uart_puts_P(ASSERTION_FAILED);
	uart_putx(line);
#endif

	led1_set_state(OFF);
	led2_set_state(ON);
	while(1) {
		led1_toggle();
		led2_toggle();
		_delay_ms(200);
	}
}


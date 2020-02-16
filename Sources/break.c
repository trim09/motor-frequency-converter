#include <avr/io.h>
#include "break.h"

#define BREAK_PORT PORTD
#define BREAK_DDR  DDRD
#define BREAK      PD1

void break_init(void) {
    break_off();
    BREAK_DDR |= 1 << BREAK;
}

void break_on(void) {
	BREAK_PORT |=  1 << BREAK;
}

void break_off(void) {
	BREAK_PORT &=  ~(1 << BREAK);
}

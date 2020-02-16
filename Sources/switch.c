#include <avr/io.h>
#include "switch.h"

#define SWITCH_IN   PINC
#define SWITCH_DDR  DDRC
#define SWITCH_PIN  PC1


void switch_init(void){
	SWITCH_DDR &= ~(1 << SWITCH_PIN);
}

state_t switch_get_state(void){
	return (SWITCH_IN & (1 << SWITCH_PIN)) ? ON : OFF;
}

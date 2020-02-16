#include "time.h"
#include <avr/interrupt.h>
#include <util/atomic.h>

static volatile uint16_t time;

void time_init(void) {
	TCCR1A = 0;
	TCCR1B = 1 << CS10;
	TIMSK1 = 1 << TOIE1;
}

time_t time_get(void) {
	typeof(time) high;
	uint16_t low;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		high = time;
		low = TCNT1;
	}

	return ((time_t)high << 16) | low;
}

time_t time_get_diff(time_t old) {
	return time_get() - old;
}


ISR(TIMER1_OVF_vect) {
	time++;
}

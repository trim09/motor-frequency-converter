#include <avr/io.h>
#include <stdint.h>
#include "common.h"
#include "assert.h"
#include "multiplication/mult16x16.h"

#define DAC_MAX_VALUE ((1 << 10) - 1)

void dac_init(void) {
	/* DAC enable, right adjust (10 bit), output enable */
	DACON = (1 << DAEN) | (1 << DAOE);
}

void dac_set_value(uint16_t value) {
	DACL = LOW(value);
	DACH = HIGH(value);
}

void dac_set_value_scaled(uint16_t value, uint16_t max_value) {
	assert(value <= max_value);
	if (max_value < DAC_MAX_VALUE) {
		while (max_value * 2 <= DAC_MAX_VALUE) {
			assert(max_value <= UINT16_MAX / 2);
			max_value *= 2;
			assert(value <= UINT16_MAX / 2);
			value *= 2;
		}
	} else {
		while (max_value > DAC_MAX_VALUE) {
			max_value /= 2;
			value /= 2;
		}
	}

	dac_set_value(value);
}

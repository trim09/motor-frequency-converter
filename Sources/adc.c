
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "adc.h"
#include "assert.h"
#include "multiplication/mult16x8.h"
#include "divide/div16.h"

/* Max filter length is 6bits (64 values long)
 * 10bit ADC value + max 6bit for filter -> 16b variale */
#define ADC_SPEED_FILTER_LEN   64
#define ADC_VOLTAGE_FILTER_LEN 4

#define MUX_CONST_PART  (1<<REFS0)
#define MUX_SPEED_POT   ((1<<MUX3) | (0<<MUX2) | (1<<MUX1) | (0<<MUX0))
#define MUX_VOLTAGE     ((0<<MUX3) | (0<<MUX2) | (1<<MUX1) | (0<<MUX0))
#define MUX_CURENT      ((0<<MUX3) | (1<<MUX2) | (1<<MUX1) | (0<<MUX0))

#define INDEX_SPEED_POT 0
#define INDEX_VOLTAGE   1
#define INDEX_CURRENT   2

static int16_t  adc_speed_pot_value;
static uint16_t adc_voltage_value;
static uint8_t adc_current_value;

static uint8_t adc_mux[3] =
		{[INDEX_SPEED_POT] = MUX_CONST_PART | MUX_VOLTAGE,
		 [INDEX_VOLTAGE]   = MUX_CONST_PART | MUX_CURENT,
		 [INDEX_CURRENT]   = MUX_CONST_PART | MUX_SPEED_POT
		};

void adc_init(void) {
	/* enable VCC ref, right adjust the ADC result, cannel ADC010 */
	ADMUX = adc_mux[0];

	/* enable ADC, enable interrupts,  ADC Prescaler = 128, Auto trigger mode */
    ADCSRA = (1 << ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0) | (1<<ADATE);
    /* Free Running Mode */
    ADCSRB = 0;

    /* start conversion */
    ADCSRA |= 1<<ADSC;
}

int16_t adc_get_speed_pot_value(void) {
	int16_t ret;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		ret = adc_speed_pot_value;
	}

	/* slow */
	ret /= ADC_SPEED_FILTER_LEN;
	/* fast */
	//	cassert(ADC_FILTER_LEN == 64);
	//	DivS16_64(retval, retval);

	return ret;
}

///* TODO FIXME !!!! */
//int16_t adc_get_speed_pot_value(void) {
//	static int32_t counter;
//	static const int32_t tmp = 80000;
//	counter++;
//
//	if (counter < tmp) {
//		return COMMAND_MAX / 4;
//	} else if (counter < 2 * tmp) {
//		return -(COMMAND_MAX / 4);
//	} else {
//		counter = 0;
//		return COMMAND_MAX / 4;
//	}
//}

uint16_t adc_get_voltage_value(void) {
	uint16_t ret;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		ret = adc_voltage_value;
	}
	ret /= ADC_VOLTAGE_FILTER_LEN;
	return ret;
}

uint8_t adc_get_current_value(void) {
	return adc_current_value;
}

/* convert from shifted code to 2's complement code
 *	when the ADC output is higher than 512, the measure is positive
 *	when the ADC output is lower  then 512, the measure is negative */
static inline int16_t adc_get_complement(void) {
	int16_t val = ADC;
	asm ("bst	%B[new_val], 1;	\n\t"
		 "cbr	%B[new_val], 0x02; \n\t"
		 "brts	end; \n\t"
		 "ori	%B[new_val], 0xFE; \n\t"
		 "end:"
		 :[new_val] "+a" (val));

	return val;
}

/* adc_value = (adc_value * (ADC_FILTER_LEN - 1) / ADC_FILTER_LEN) + ADC; */
ISR(ADC_vect) {
	static uint8_t last_index = INDEX_SPEED_POT;

	switch (last_index) {
	case INDEX_SPEED_POT:
		/* adc_value * (ADC_FILTER_LEN - 1) / ADC_FILTER_LEN =>
		 * adc_value - adc_value / ADC_FILTER_LEN */
		/* slow (use DivS16_64() to speed it up) */
		adc_speed_pot_value -= adc_speed_pot_value / ADC_SPEED_FILTER_LEN;
		adc_speed_pot_value += adc_get_complement();
		break;
	case INDEX_VOLTAGE:
		adc_voltage_value -= adc_voltage_value / ADC_VOLTAGE_FILTER_LEN;
		adc_voltage_value += ADC;
		break;
	case INDEX_CURRENT:
		adc_current_value = (uint8_t)(ADC >> 2);
		break;
	default:
		assert_soft(0);
	}

	last_index++;
	if (last_index == countof(adc_mux)) {
		last_index = 0;
	}

	ADMUX = adc_mux[last_index];
}



#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#include "../break.h"
#include "timer0.h"
#include "speed.h"
#include "controlVF.h"
#include "speed_control.h"
#include "current_limitation.h"
#include "../adc.h"
#include "../assert.h"
#include "../config.h"
#include "../common.h"
#include "../space_vector/vector_recalculation.h"
#include "../multiplication/mult16x8.h"

#ifdef DEBUG
volatile uint16_t test_overcurrent;
volatile int16_t  test_command;
volatile int16_t  test_amplitude;
#endif

/* Timer 0 Configuration */
void timer0_init (void) {
	cassert(TIMER0_DIVIDER == 64);
	TCCR0B = (1 << CS01) | (1 << CS00);  // f_quartz = 16 MHz / 64 = 250 kHz
	TCCR0A = (1 << WGM01); // mode CTC : Clear Timer on Compare
	OCR0A  = OCR0A_VAL;
	TIMSK0 = (1 << OCIE0A); // allow interruption when timer=compare
}

static int16_t regulate(int16_t real_speed, int16_t requested_speed) {
	uint8_t current = adc_get_current_value();
	int16_t command;

	/* ------------------- regulate speed  --------------------------------- */
	if (current > CURRENT_MAX) {
		command_reset();
		command = 0;
		assert(0); /* TODO FIXME  - break needs to be implemented, than remove this line */
	} else if ((current > CURRENT_TRESHOLD)
			&& (abs(command_get_old_value()) <= abs(requested_speed)))
	{
#	ifdef DEBUG
		test_overcurrent++;
#	endif
		command = command_get_old_value();
	} else {
		command = command_get(real_speed, requested_speed);
		assert_soft((command >= -COMMAND_MAX) && (command <= COMMAND_MAX));
	}

	return command;
}

static uint16_t command_to_angle_increment_scaled(int16_t command) {
	uint16_t angle_increment_scaled;
	int16_t  command_abs = abs(command);

	/* angle_increment_scaled = ANGLE_SCALE * ANGLE_INCREMENT_MAX * (command_abs / COMMAND_MAX); */
	cassert(ANGLE_SCALE * ANGLE_INCREMENT_MAX <= MAX_VALUE(angle_increment_scaled)); /* prevent overflow */
	cassert((ANGLE_SCALE * ANGLE_INCREMENT_MAX / COMMAND_MAX) > 0); /* shouldn't be zero */
	cassert(((ANGLE_SCALE * ANGLE_INCREMENT_MAX) % COMMAND_MAX) == 0); /* prevent rounding errors */
	/* FIXME */
	//cassert(ANGLE_INCREMENT_MAX_NUMERATOR % ANGLE_INCREMENT_MAX_DENOMINATOR == 0); /* prevent rounding errors */
	/* angle_increment = command_abs * (ANGLE_INTEGRATOR_SCALE * ANGLE_INCREMENT_MAX / COMMAND_MAX); */
	cassert((ANGLE_SCALE * ANGLE_INCREMENT_MAX / COMMAND_MAX) <= UINT8_MAX);

	MultiSU16XConst8toL16(angle_increment_scaled, command_abs,
			ANGLE_SCALE * ANGLE_INCREMENT_MAX / COMMAND_MAX);

	return angle_increment_scaled;
}

/* This is called every OCR0A_VAL * TIMER0_DIVIDER / F_CPU second => 0.256ms.
 * One angle unit = 360 / 6 / TAB_SIN_SIZE = 0.48 degree
 *
 * stator speed 100 Hz:
 *    degrees per call = 360 * 100hz * 256us = 9.216
 *    angle units per call = degrees per call / one angle unit = 9.216 / 0.48 = 19.2
 *
 *
 * stator speed 1 Hz:
 *    degrees per call = 360 * 1hz * 256us = 0.09216
 *    angle units per call = degrees per call / one angle unit = 0.09216 / 0.48 = 0.192
 *
 *
 *  19.200 * ANGLE_INTEGRATOR_SCALE = 4915,200
 *   0.192 * ANGLE_INTEGRATOR_SCALE =   49,152
 *
 * Space vector is recalculated every: 2 * (MAX_PWM + DEADTIME) / 64Mhz =>
 *  2 * MAX_PWM / 64Mhz +  2 * DEADTIME / 64Mhz => 66us (15.15kHz)
 *
 */
ISR(TIMER0_COMPA_vect) {
	static volatile bool timer0_int_inprocess;
	int16_t     command, real_speed, requested_speed;
	uint16_t    angle_increment_scaled, amplitude;
	direction_t direction;

	assert(false == timer0_int_inprocess);
	timer0_int_inprocess = true;
	sei(); /* enable interrupts! */

	if (adc_get_voltage_value() > VOLTAGE_MAX) {
		assert(0);
	} else if (adc_get_voltage_value() < VOLTAGE_TRESHOLD_LO) {
		break_off();
	} else if (adc_get_voltage_value() > VOLTAGE_TRESHOLD_HI) {
		break_on();
	}

	/* get real speed of rotor */
	real_speed = dummy_get_real_speed();
	assert_soft((real_speed >= -SPEED_MAX) && (real_speed <= SPEED_MAX));

	/* get requested speed from potentiometer  */
	requested_speed = dummy_get_requested_speed();
	assert_soft((requested_speed >= -SPEED_MAX) && (requested_speed <= SPEED_MAX));

	/* regulate the speed and current */
	command = regulate(real_speed, requested_speed);
	assert_soft((command >= -COMMAND_MAX) && (command <= COMMAND_MAX));

	/* convert command to angle speed */
	angle_increment_scaled = command_to_angle_increment_scaled(command);
	assert_soft(angle_increment_scaled <= ANGLE_INCREMENT_SCALED_MAX);

	/* V/f law */
	amplitude = controlVF(angle_increment_scaled);
	assert_soft(amplitude <= AMPLITUDE_MAX);

	/* set new amplitude and angle speed to SVPWM */
	direction = (command >= 0) ? FORWARD : BACKWARD;
	vector_set_new_speed(amplitude, angle_increment_scaled, direction);

#	ifdef DEBUG
	test_command = command;
	test_amplitude = amplitude;
#	endif

	cli();
	timer0_int_inprocess = false;

	/* Match Flag must be still cleared otherwise we are too slow */
	//assert((TIFR0 & (1 << OCF0A)) == 0);
}

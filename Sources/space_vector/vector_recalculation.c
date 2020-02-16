#include <util/atomic.h>
#include <stdbool.h>

#include "SVPWM.h"

#include "../assert.h"
#include "../config.h"
#include "../common.h"
#include "psc.h"

static uint16_t    angle_integrator_scaled;
static uint16_t    angle_increment_scaled;
static uint16_t    amplitude;
static direction_t direction;


void vector_set_new_speed(
		uint16_t    new_amplitude,
		uint16_t    new_angle_increment_scaled,
		direction_t new_direction)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		amplitude              = new_amplitude;
		angle_increment_scaled = new_angle_increment_scaled;
		direction              = new_direction;
	}
}

/* PSC2 end cycle interrupt handle
 * New vector of magnetization will be calculated and set to PSC in this routine. */
ISR(PSC2_EC_vect) {
	uint8_t  angle_units; /* in sin's table "degrees" (units)*/
	pwm_t    pwm;

	/* Prevent overflow */
	cassert(MAX_VALUE(angle_integrator_scaled) >= (ANGLE_INCREMENT_MAX + 1) * ANGLE_SCALE);
	angle_integrator_scaled += angle_increment_scaled;

	/* angle_units type is big enough to contain the result
	 * MAX_VALUE(angle_units) >= ((ANGLE_INCREMENT_MAX + 1) * ANGLE_SCALE / ANGLE_SCALE) */
	cassert(MAX_VALUE(angle_units) >= ANGLE_INCREMENT_MAX + 1);
	angle_units = angle_integrator_scaled / ANGLE_SCALE;

	if (angle_units) {
		// ------------- space vector PWN algorithm -----------------
		SVPWM(amplitude, angle_units, &pwm);

#		include "../dac.h" /* TODO */
		dac_set_value_scaled(pwm.pwm1, MAX_PWM); /* TODO */

		// -------- load the PSCs with the new duty cycles -----------
		if (direction == FORWARD) {
			psc_load(pwm.pwm0, pwm.pwm1, pwm.pwm2);
		} else {
			psc_load(pwm.pwm0, pwm.pwm2, pwm.pwm1);
		}

		angle_integrator_scaled -= (uint16_t)angle_units * ANGLE_SCALE;
	}

	/* test that we are fast enough */
	assert((PIFR2 & (1 << PEOP2)) == 0);
}

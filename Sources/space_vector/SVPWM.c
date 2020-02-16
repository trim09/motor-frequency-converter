#include "../common.h"
#include "../config.h"
#include "../assert.h"
#include "../multiplication/mult16x8.h"
#include "table_sin.h"
#include "SVPWM.h"

#define MAX_ANGLE      (TAB_SIN_SIZE - 1)

/* From input parameters "amp" and "angle_increment" calculates an output "pwm".
 * "pwm" will contains values which can be directly copied into PSC module.
 *
 * The values are calculated using the "space vector" algorithm.
 *
 * angle_step must be in range 0 to MAX_ANGLE and is in units of sin table.
 * amp must be in range 0 to MAX_PWM
 *
 * angle_step is a angle increment from last call of this function.
 *
 * amp = Ts * 2/sqrt(3) * Vx/Vmax
 * Ts = MAX_PWM
 * Vx is at most Vmax * sqrt(3)/2
 * Vx is a requested size of voltage (a size of requested vector)
 * so the "amp" can be in range 0...MAX_PWM
 */
void SVPWM(uint16_t amp, uint8_t angle_increment, pwm_t *pwm) {
	static uint8_t  sector_number = 1;
	static uint8_t  angle = 0; /* angle in "tab_sin" units */
	uint16_t        tau1, tau2;
	uint16_t        val1, val2_1, val3, val2_2;

	/* Angle increment is too big */
	assert(angle_increment <= ANGLE_INCREMENT_MAX);
	/* Amplitude is too big */
	assert(amp <= MAX_PWM);
	/* Data type of "angle" is too small for angle computation */
	cassert(MAX_VALUE(angle) >= MAX_ANGLE + ANGLE_INCREMENT_MAX);
	/* If not fulfilled, 1 sector can be skipped. Current algorithm can't handle it. */
	cassert(MAX_ANGLE >= ANGLE_INCREMENT_MAX);

	angle += angle_increment;

	if (angle > MAX_ANGLE) {
		angle -= MAX_ANGLE;
		sector_number++;

		if (sector_number > 6) {
			sector_number = 1;
		}
	}

	/* slow */
	//tau1 = ((uint32_t)amp * tab_sin[MAX_ANGLE - angle]) / TAB_SIN_SCALE;
	//tau2 = ((uint32_t)amp * tab_sin[angle]) / TAB_SIN_SCALE;

	/* faster */
	/* call MultiUU16X8toH16 to multiply amp and tab_sin and take high 16bits to divide it by 256 */
	cassert(TAB_SIN_SCALE == 256);
	MultiUU16X8toH16(tau1, amp, tab_sin[MAX_ANGLE - angle]);
	MultiUU16X8toH16(tau2, amp, tab_sin[angle]);
	/*
	 *  |    t0/2    |  t1    |            t2         |    t0/2   |
	 *  -----------------------------------------------------------
	 *         val0 -^        ^- val1            ^    ^- val2
	 *     for example MAX_PWM/2 somewhere here -+      MAX_PWM -^
	 *
	 * build 3 equations:
	 *  1. - val0 + val1        = t1
	 *  2.        - val1 + val2 = t2
	 *  3. + val0        + val2 = MAX_PWM
	 *
	 * The third equation presents that val0 and val2 has MAX_PWM/2 as a center.
	 *
	 * Lets solve:
	 *  step 1
	 *  1. + 3.   + val1 + val2 = t1 + MAX_PWM
	 *
	 *  step 2
	 *  1. + 3. + 2.   2 * val2 = MAX_PWM + t1 + t2
	 *  simplify to:
	 *                     val2 = MAX_PWM / 2 + t1 / 2 + t2 / 2
	 *
	 *  step 3 (val2 to equation 1. + 3.)
	 *  1. + 3.  val1 + MAX_PWM / 2 + (t1 + t2) / 2 = t1 + MAX_PWM
	 *  simplify to:
	 *      val1 =      MAX_PWM / 2 + t1 / 2 - t2 / 2
	 *
	 *  step 4 (val2 to equation 3.)
	 *  3.  val0 + MAX_PWM / 2 + (t1 + t2) / 2 = MAX_PWM
	 *  simplify to:
	 *      val0 = MAX_PWM / 2 - t1 / 2 - t2 / 2
	 *
	 *
	 *  the solution is:
	 *   val0 = MAX_PWM / 2 - t1 / 2 - t2 / 2
	 *   val1 = MAX_PWM / 2 + t1 / 2 - t2 / 2
	 *   val2 = MAX_PWM / 2 + t1 / 2 + t2 / 2
	 *
	 *	or
	 *	 val0 = (MAX_PWM - t1 - t2) / 2
	 *   val1 = (MAX_PWM + t1 - t2) / 2
	 *   val2 = (MAX_PWM + t1 + t2) / 2
	 */

	assert(tau1 + tau2 <= MAX_PWM);
	val1   = (MAX_PWM - tau1 - tau2) / 2;
	val2_1 = (MAX_PWM + tau1 - tau2) / 2;
	val2_2 = (MAX_PWM - tau1 + tau2) / 2;
	val3   = (MAX_PWM + tau1 + tau2) / 2;

	switch (sector_number) {
	case 1:
		/*
		 * pwm0 _____+-------------------+____
		 * pwm1 _________+-----------+________
		 * pwm2 ____________+-----+___________
		 *
		 *           ^   ^  ^  ^
		 *     val0 -+   |  |  |
		 *     val1 -----+  |  |
		 *     val2 --------+  |
		 * MAX_PWM ------------+
		 *
		 * t1 = tau1
		 * t2 = tau2
		 */
		pwm->pwm0 = val1;
		pwm->pwm1 = val2_1;
		pwm->pwm2 = val3;
		break;
	case 2:
		/*
		 * pwm0 _________+-----------+________
		 * pwm1 _____+-------------------+____
		 * pwm2 ____________+-----+___________
		 *
		 *           ^   ^  ^  ^
		 *     val0 -+   |  |  |
		 *     val1 -----+  |  |
		 *     val2 --------+  |
		 * MAX_PWM ------------+
		 *
		 * but t1 is now t2 and t2 is t1
		 */
		pwm->pwm0 = val2_2;
		pwm->pwm1 = val1;
		pwm->pwm2 = val3;
		break;
	case 3:
		/*
		 * pwm0 ____________+-----+___________
		 * pwm1 _____+-------------------+____
		 * pwm2 _________+-----------+________
		 *
		 *           ^   ^  ^  ^
		 *     val0 -+   |  |  |
		 *     val1 -----+  |  |
		 *     val2 --------+  |
		 * MAX_PWM ------------+
		 *
		 * t1 = tau1
		 * t2 = tau2
		 */
		pwm->pwm0 = val3;
		pwm->pwm1 = val1;
		pwm->pwm2 = val2_1;
		break;
	case 4:
		/*
		 * pwm0 ____________+-----+___________
		 * pwm1 _________+-----------+________
		 * pwm2 _____+-------------------+____
		 *
		 *           ^   ^  ^  ^
		 *     val0 -+   |  |  |
		 *     val1 -----+  |  |
		 *     val2 --------+  |
		 * MAX_PWM ------------+
		 *
		 * but t1 is now t2 and t2 is t1
		 *
		 */
		pwm->pwm0 = val3;
		pwm->pwm1 = val2_2;
		pwm->pwm2 = val1;
		break;
	case 5:
		/*
		 * pwm0 _________+-----------+________
		 * pwm1 ____________+-----+___________
		 * pwm2 _____+-------------------+____
		 *
		 *           ^   ^  ^  ^
		 *     val0 -+   |  |  |
		 *     val1 -----+  |  |
		 *     val2 --------+  |
		 * MAX_PWM ------------+
		 *
		 * t1 = tau1
		 * t2 = tau2
		 */
		pwm->pwm0 = val2_1;
		pwm->pwm1 = val3;
		pwm->pwm2 = val1;
		break;
	case 6:
		/*
		 * pwm0 _____+-------------------+____
		 * pwm1 ____________+-----+___________
		 * pwm2 _________+-----------+________
		 *
		 *           ^   ^  ^  ^
		 *     val0 -+   |  |  |
		 *     val1 -----+  |  |
		 *     val2 --------+  |
		 * MAX_PWM ------------+
		 *
		 * but t1 is now t2 and t2 is t1
		 *
		 */
		pwm->pwm0 = val1;
		pwm->pwm1 = val3;
		pwm->pwm2 = val2_2;
		break;
	default:
		assert(0);
		break;
	}
}

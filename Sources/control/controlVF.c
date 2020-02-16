#include "controlVF.h"
#include "../config.h"
#include "../assert.h"
#include "../multiplication/mult16x8.h"

/* This module requires/depends on:
 * ANGLE_INCREMENT_MAX, F_MAX, MAX_PWM
 * */

/* internal "Vf_SLOPE" scale */
#define CONTROL_VF_SLOPE_SCALE       32U
//#define CONTROL_VF_SLOPE_SCALE  (1ull+MAX_VALUE(angle_increment_scaled) / (AMPLITUDE_MAX))

/* OANGLE_INCREMENT_50hz_SCALED = ANGLE_INCREMENT_MAX * (50hz / F_MAX) */
#define ANGLE_INCREMENT_50hz_SCALED (((1ULL * ANGLE_INCREMENT_SCALED_MAX * 50) + F_MAX / 2) / F_MAX)
//#define ANGLE_INCREMENT_MIN_SCALED  (ANGLE_INCREMENT_50hz_SCALED / 10) /* 10% of ANGLE_INCREMENT_MAX */
#warning TODO FIXME
#define ANGLE_INCREMENT_MIN_SCALED  1 /* FIXME TODO */

#define Vf_SLOPE_NUMERATOR   (1ULL * AMPLITUDE_MAX * CONTROL_VF_SLOPE_SCALE)
#define Vf_SLOPE_DENOMINATOR ANGLE_INCREMENT_50hz_SCALED
#define Vf_SLOPE             (Vf_SLOPE_NUMERATOR / Vf_SLOPE_DENOMINATOR)

#define AMPLITUDE_MIN        ((Vf_SLOPE * ANGLE_INCREMENT_MIN_SCALED) / CONTROL_VF_SLOPE_SCALE)

/* V/f law
 * IN:     "angle_increment_scaled" is scaled
 * RETURN: PWM max value (will be used in PSC and waveform generation)
 *
 * The return value never exceeds VOLTAGE_MAX, if "angle_increment_scaled"
 * doesn't exceed ANGLE_INCREMENT_MAX.
 */
uint16_t controlVF(uint16_t angle_increment_scaled) {
	uint16_t amplitude;

	/* shouldn't be zero, otherwise the following algorithm doesn't work */
	cassert(Vf_SLOPE >= 0x01);
	/* boost voltage greater than zero */
	cassert(ANGLE_INCREMENT_MIN_SCALED >= 0x01);
	/* just for sure */
	cassert(ANGLE_INCREMENT_50hz_SCALED <= ANGLE_INCREMENT_SCALED_MAX);
	/* shouldn't be bigger than AMPLITUDE_MAX */
	cassert(ANGLE_INCREMENT_50hz_SCALED * Vf_SLOPE / CONTROL_VF_SLOPE_SCALE
			<= AMPLITUDE_MAX);
	/* max error is smaller than Vf_SLOPE */
	cassert(AMPLITUDE_MAX - ANGLE_INCREMENT_50hz_SCALED * Vf_SLOPE /
			CONTROL_VF_SLOPE_SCALE < 2 * Vf_SLOPE);
	/* prevent overflow */
	cassert((uint32_t)ANGLE_INCREMENT_50hz_SCALED * Vf_SLOPE
			<= (uint32_t)MAX_VALUE(angle_increment_scaled));
	/* we will cast it to 8bit */
	cassert(Vf_SLOPE <= UINT8_MAX);

	if (angle_increment_scaled <= ANGLE_INCREMENT_MIN_SCALED) {
		/* boost frequency */
		amplitude = AMPLITUDE_MIN; /* boost voltage */
	} else if (angle_increment_scaled >= ANGLE_INCREMENT_50hz_SCALED) {
		/* more than rated frequency */
		amplitude = AMPLITUDE_MAX; /* rated value */
	} else { /* V/f law */
		/* slow */
		/* amplitude = (angle_increment_scaled * (uint8_t)Vf_SLOPE) / CONTROL_VF_SLOPE_SCALE; */
		/* faster */
		MultiUU16XConst8toL16(amplitude, angle_increment_scaled, Vf_SLOPE);
		amplitude /= CONTROL_VF_SLOPE_SCALE;
	}

	return amplitude;
}

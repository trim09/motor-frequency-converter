#ifndef SPACE_VECTOR_PWM_H_
#define SPACE_VECTOR_PWM_H_

#import <stdint.h>

typedef struct {
	uint16_t pwm0;
	uint16_t pwm1;
	uint16_t pwm2;
} pwm_t;

/* Space Vector algorithm used to recalculate new vector of modulation.
 * IN: amp, angle_step
 * OUT: pwm (pwm contains PSCn's values) */
void SVPWM(uint16_t amp, uint8_t angle_step, pwm_t *pwm);

#endif /* SPACE_VECTOR_PWM_H_ */

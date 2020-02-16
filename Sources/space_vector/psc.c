#include <avr/io.h>
#include <stdint.h>

#include "../assert.h"
#include "../common.h"
#include "../config.h"

static uint8_t deadtime;

/*
 * Center Aligned Mode
 *
 * PLL should be already started (we drive MCU clock from PLL output)
 */
void psc_init(uint16_t pwm_max_value, uint8_t dead_time) {
	deadtime = dead_time;

	/* set PLL output to 64Mhz */
	PLLCSR |= 1 << PLLF;
	assert_soft((PLLCSR & 0x07) == 0x07); /* locked, started and 64Mhz */

	assert_soft((1 << 12) > pwm_max_value + deadtime);
	OCR0RB = pwm_max_value + deadtime;
	OCR1RB = pwm_max_value + deadtime;
	OCR2RB = pwm_max_value + deadtime;

	OCR0SA = pwm_max_value;
	OCR1SA = pwm_max_value;
	OCR2SA = pwm_max_value;
	OCR0SB = pwm_max_value / 2 + deadtime;
	OCR1SB = pwm_max_value / 2 + deadtime;
	OCR2SB = pwm_max_value / 2 + deadtime;

	/* Center Aligned Mode, fast clock input, Output Polarity: active LOW. */
	PCNF0 = (1 << PMODE01) | (1 << PMODE00) | (1 << PCLKSEL0);
	PCNF1 = (1 << PMODE11) | (1 << PMODE10) | (1 << PCLKSEL1);
	PCNF2 = (1 << PMODE21) | (1 << PMODE20) | (1 << PCLKSEL2);

	/* PSC0 and PSC1 as slave, PSC2 as master */
	PCTL0 = 1 << PARUN0;
	PCTL1 = 1 << PARUN1;
	PCTL2 = 1 << PRUN2;
//#	warning TODO FIXME
//	PCTL0 = (1 << PARUN0) | (1 << PPRE00);
//	PCTL1 = (1 << PARUN1) | (1 << PPRE10);
//	PCTL2 = (1 << PRUN2) | (1 << PPRE20);

	/* PSC2 interrupt at the end of cycle enable */
	PIM2 = 1 << PEOPE2;

	// connect the PSC waveform generator outputs to the port outputs
	PSOC0 = (1 << POEN0B) | (1 << POEN0A);
	PSOC1 = (1 << POEN1B) | (1 << POEN1A);
	PSOC2 = (1 << POEN2B) | (1 << POEN2A);
}

void psc_halt(void) {
	PSOC0 &= ~((1 << POEN0B) | (1 << POEN0A));
	PSOC1 &= ~((1 << POEN1B) | (1 << POEN1A));
	PSOC2 &= ~((1 << POEN2B) | (1 << POEN2A));
}

/* Load the PSC 0, 1 and 2 with new values */
void psc_load(uint16_t time0, uint16_t time1, uint16_t time2) {
	/* slow */
	//	PCNF0 |= 1 << PLOCK0;
	//	PCNF1 |= 1 << PLOCK1;
	//	PCNF2 |= 1 << PLOCK2;
	/* faster */
	set_bit(PCNF0, PLOCK0);
	set_bit(PCNF1, PLOCK1);
	set_bit(PCNF2, PLOCK2);

	assert(OCR0RB >= time0 + deadtime);
	OCR0SA = time0;
	OCR0SB = time0 + deadtime;

	assert(OCR1RB >= time1 + deadtime);
	OCR1SA = time1;
	OCR1SB = time1 + deadtime;

	assert(OCR2RB >= time2 + deadtime);
	OCR2SA = time2;
	OCR2SB = time2 + deadtime;

	/* slow */
	//	PCNF0 &= ~(1 << PLOCK0);
	//	PCNF1 &= ~(1 << PLOCK1);
	//	PCNF2 &= ~(1 << PLOCK2);
	/* faster */
	clr_bit(PCNF0, PLOCK0);
	clr_bit(PCNF1, PLOCK1);
	clr_bit(PCNF2, PLOCK2);
}


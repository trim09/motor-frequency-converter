#ifndef PSC_H_
#define PSC_H_

#include <stdint.h>

void psc_init(uint16_t pwm_max_value, uint8_t dead_time);
void psc_halt(void);

/* Load the PSC 0, 1 and 2 with new values */
void psc_load(uint16_t time0, uint16_t time1, uint16_t time2);

#endif /* PSC_H_ */

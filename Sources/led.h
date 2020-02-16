#ifndef LED_H_
#define LED_H_

#include "common.h"

void led_init(void);
void led1_toggle(void);
void led2_toggle(void);
void led1_set_state(state_t state);
void led2_set_state(state_t state);

#endif /* LED_H_ */

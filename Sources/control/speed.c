#include "speed.h"
#include "../adc.h"
#include <stdint.h>

int16_t dummy_get_real_speed(void) {
	return 0;
}

/* returns value from -512 to 511
 *
 *    0 = stop
 *    1 = forward  F_MAX/COMMAND_MAX hz
 *   -1 = backward F_MAX/COMMAND_MAX hz
 *  511 = forward  F_MAX hz
 * -512 = backward F_MAX + F_MAX/COMMAND_MAX hz
 * */
int16_t dummy_get_requested_speed(void) {
	return adc_get_speed_pot_value();
}

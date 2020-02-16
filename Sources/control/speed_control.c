#include "../config.h"
#include "speed_control.h"

#define COMMAND_FILTER_LEN 8

static int32_t command;

void command_reset (void) {
	command = 0;
}


int16_t command_get_old_value (void) {
	return command >> COMMAND_FILTER_LEN;
}

int16_t command_get(int16_t real_speed, int16_t requested_speed) {
	//command = mc_control_speed_16b(Omega_ref,Omega_meas); // for use in closed loop
	//return requested_speed;

	int16_t tmp = command >> COMMAND_FILTER_LEN;

	if (tmp < requested_speed) {
		command++;
	} else if (tmp > requested_speed) {
		command--;
	}

	//return tmp; /* it doesn't matter whether we return old or new value */
	return command >> COMMAND_FILTER_LEN;
}

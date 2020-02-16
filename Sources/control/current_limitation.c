#include "current_limitation.h"
#include "../adc.h"
#include "../config.h"

int16_t current_limitation(int16_t real_speed, int16_t command) {
//	uint16_t current = adc_get_current_value();
//	if (current <= CURRENT_TRESHOLD) {
//		return command;
//	} else if (current <= CURRENT_MAX) {
//		return command - ((command - real_speed) / (CURRENT_MAX - CURRENT_TRESHOLD));
//	} else {
//		return real_speed;
//	}
	return command;
}

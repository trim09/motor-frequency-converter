#ifndef SPEED_CONTROL_H_
#define SPEED_CONTROL_H_

#include <stdint.h>

void command_reset (void);
int16_t command_get_old_value (void);
int16_t command_get (int16_t real_speed, int16_t requested_speed);

#endif /* SPEED_CONTROL_H_ */

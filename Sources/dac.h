#ifndef DAC_H_
#define DAC_H_

#include <stdint.h>

void dac_init(void);
void dac_set_value(uint16_t value);
void dac_set_value_scaled(uint16_t value, uint16_t max_value);

#endif /* DAC_H_ */

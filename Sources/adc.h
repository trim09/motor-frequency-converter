#ifndef ADC_H_
#define ADC_H_

void adc_init(void);

int16_t adc_get_speed_pot_value(void);
uint16_t adc_get_voltage_value(void);
uint8_t adc_get_current_value(void);

#endif /* ADC_H_ */

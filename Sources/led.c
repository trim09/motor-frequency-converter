#include <avr/io.h>
#include "led.h"

/* Port, na ktery je pripojena led */
#define LED1_PORT PORTC
#define LED2_PORT PORTD
/* DDR registr, na ktery je pripojena led */
#define LED1_DDR  DDRC
#define LED2_DDR  DDRD
/* Pin, na ktery je pripojena led */
#define LED1      PC2
#define LED2      PD2


/* Nastavi DDR registr, aby bylo mozne ovladat ledku - pin ledky jako vystupni */
void led_init(void) {
    LED1_DDR |= 1 << LED1;
    LED2_DDR |= 1 << LED2;
}

/* Zmeni stav ledky */
void led1_toggle(void){
    LED1_PORT ^=  1 << LED1; // XOR
}

void led1_set_state(state_t state) {
	if (state == ON) {
		LED1_PORT &=  ~(1 << LED1);
	} else {
		LED1_PORT |=  1 << LED1;
	}
}

void led2_toggle(void){
    LED2_PORT ^=  1 << LED2; // XOR
}

void led2_set_state(state_t state) {
	if (state == ON) {
		LED2_PORT &=  ~(1 << LED2);
	} else {
		LED2_PORT |=  1 << LED2;
	}
}

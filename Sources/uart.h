#ifndef __UART_H
#define __UART_H

#include "config.h"

#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/setbaud.h>

inline bool uart_isTXempty(void) {
    return (UCSRA & (1<<UDRE)) ? true : false;
}

inline bool uart_isRXempty(void) {
    return (UCSRA & (1<<RXC)) ? false : true;
}

void uart_init(void);

#ifndef DEBUG
void uart_puts_P(const prog_char * string);
void uart_puthex(char c);
void uart_putx(uint16_t d);
#endif

//inline char uart_receive_nowait(void) {
//    /* Get and return received data from buffer */
//    return UDR;
//}
//char uart_getchar(void);
//void uart_putchar(char c);
//void uart_puts(const char * string);
///* put string stored in PROGMEM and place '\n' at the end */
//void uart_putsn_P(const prog_char * string);
//void uart_putnewline(void);
//void uart_vt100_clear_screen(void);
//
//void uart_putlu(uint32_t d);
//void uart_putllu(uint64_t d);

#endif

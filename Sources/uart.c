#include "uart.h"
#include <stdio.h>

static int uart_stdio_putchar(char c, FILE *stream);
static int uart_stdio_getchar(FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(&uart_stdio_putchar, NULL, _FDEV_SETUP_WRITE);
static FILE mystdin  = FDEV_SETUP_STREAM(NULL, &uart_stdio_getchar, _FDEV_SETUP_READ);

void uart_init(void) {
    /* Set baud rate */
    UBRRH = UBRRH_VALUE;
    UBRRL = UBRRL_VALUE;
    /* Clear TXC and set/clear double speed mode */
    UCSRA = (1<<TXC) | (USE_2X<<U2X);
    /* Set frame format: 8data, no parity & 1 stop bit */
    UCSRC = (1<<UCSZ1) | (1<<UCSZ0);
    /* Enable receiver and transmitter */
    UCSRB = (1<<RXEN) | (1<<TXEN);

    stdout = &mystdout;
    stdin  = &mystdin;
}

static int uart_stdio_putchar(char c, FILE *stream) {
	if (c == '\n')
		uart_stdio_putchar('\r', stream);
    /* Wait for empty transmit buffer */
    while (false == uart_isTXempty())
        ;
	UDR = c;
	return 0;
}

static int uart_stdio_getchar(FILE *stream) {
    /* Wait for data to be received */
    while (true == uart_isRXempty())
        ;
    /* Get and return received data from buffer */
    return UDR;
}

#ifndef DEBUG
void uart_puts_P(const prog_char * string) {
    uint8_t tmp;
    tmp = pgm_read_byte(string++);
    while (tmp != '\0') {
    	uart_stdio_putchar(tmp, NULL);
        tmp = pgm_read_byte(string++);
    }
    uart_stdio_putchar('\n', NULL);
}

void uart_puthex(char d) {
	static const char dtox_lookup[16] = "0123456789ABCDEF";
	uart_stdio_putchar(dtox_lookup[d >> 4], NULL);
	uart_stdio_putchar(dtox_lookup[d & 0x0f], NULL);
}

void uart_putx(uint16_t d) {
	uart_puthex((char)(d >> 8));
	uart_puthex(((char)d & 0x00FF));
}
#endif

//static char uart_getchar(void) {
//    /* Wait for data to be received */
//    while (true == uart_isRXempty())
//        ;
//    /* Get and return received data from buffer */
//    return UDR;
//}
//
//char uart_getchar(void) {
//    /* Wait for data to be received */
//    while (true == uart_isRXempty())
//        ;
//    /* Get and return received data from buffer */
//    return UDR;
//}
//
//
//void uart_puthex(char d) {
//	static const char dtox_lookup[16] = {
//			'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
//			'A', 'B', 'C', 'D', 'E', 'F'};
//	uart_putchar(dtox_lookup[d >> 4]);
//	uart_putchar(dtox_lookup[d & 0x0f]);
//}
//
//void uart_puts(const char * string) {
//    while (*string != '\0') {
//        uart_putchar(*string++);
//    }
//}
//
//void uart_putlu(uint32_t d) {
//	uart_putx((uint16_t)(d >> 16));
//	uart_putx((uint16_t)d);
//}
//
//void uart_putllu(uint64_t d) {
//	uart_putx((uint32_t)(d >> 32));
//	uart_putx((uint32_t)d);
//}
//
//void uart_putnewline(void) {
//	uart_putchar('\r');
//	uart_putchar('\n');
//}
//
//
//void uart_putsn_P(const prog_char * string) {
//	uart_puts_P(string);
//	uart_putnewline();
//}
//
//void uart_vt100_clear_screen(void) {
//	uart_putchar(27);
//	uart_putchar('[');
//	uart_putchar('2');
//	uart_putchar('J');
//}

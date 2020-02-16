#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>

typedef enum {
	FORWARD,
	BACKWARD
} direction_t;

typedef enum {
	OFF,
	ON
} state_t;


#define LOW(a)  ((uint8_t)(a))
#define HIGH(a) ((uint8_t)((a) >> 8))

typedef union {
  uint16_t w;
  uint8_t  b[2];
} cast16_u;

#define set_bit(sfr, bit) \
	{ char __tmp; \
		asm volatile ( \
			"lds %0, %1; \n\t" \
			"sbr %0, %2; \n\t" \
			"sts %1, %0; \n\t" \
			: "=d" (__tmp) \
			: "n" (_SFR_MEM_ADDR(sfr)), "M" (1 << (bit)) \
			); \
	}

#define clr_bit(sfr, bit) \
	{ char __tmp; \
		asm volatile ( \
			"lds %0, %1; \n\t" \
			"cbr %0, %2; \n\t" \
			"sts %1, %0; \n\t" \
			: "=d" (__tmp) \
			: "n" (_SFR_MEM_ADDR(sfr)), "M" (1 << (bit)) \
			); \
	}

#define countof(arr) (sizeof(arr) / sizeof(arr[0]))

#endif /* COMMON_H_ */

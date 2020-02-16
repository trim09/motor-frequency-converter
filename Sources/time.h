#ifndef TIME_H_
#define TIME_H_

#include <stdint.h>

typedef uint32_t time_t;

void time_init(void);
time_t time_get(void);
time_t time_get_diff(time_t old);

#endif /* TIME_H_ */

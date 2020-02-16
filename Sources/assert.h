#ifndef ASSERT_H_
#define ASSERT_H_

#include <stdlib.h>
#include "config.h"

#define MAX_VALUE(var) ((1ULL << (8 * sizeof(var))) - 1ULL)
#define assert(e) ((e) ? ((void)0) : assert_handle(__LINE__))
/* compile time assert */
#define cassert(pred) switch(0) {case 0: case (pred): ;}

void assert_handle(int line) __ATTR_NORETURN__;

#ifdef DEBUG
#define assert_soft(e) ((e) ? ((void)0) : assert_handle(__LINE__))
#else
#define assert_soft(e) ((void)0)
#endif

#endif /* ASSERT_H_ */

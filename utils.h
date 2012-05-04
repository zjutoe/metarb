#ifndef _UTILS_
#define _UTILS_

#include "inst.h"

#ifdef __GNUC__
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#else
#define likely(x)       (x)
#define unlikely(x)     (x)
#endif


extern inline void dummp_inst(inst_t i);

#endif	/* _UTILS_ */

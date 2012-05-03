#ifndef _UTILS_
#define _UTILS_

#include <stdio.h>		/* fro the printf */
#include "core.h"

inline void dummp_inst(inst_t i);

#define dieif(cond, msg)				\
	do{						\
		if((cond)){				\
			printf("%s\n", (msg));		\
			exit(1);			\
		}					\
	}while(0)

#endif

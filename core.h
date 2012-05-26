#ifndef _CORE_H
#define _CORE_H

#include <stdint.h>

#define MAX_CORE_NUM 256

#define REG_ZERO     0
#define REG_PC       32
#define REG_OVERFLOW 33
#define REG_LO       34
#define REG_HI       35
#define MAX_REG_NUM  36


/* A MIPS core */
typedef struct _core {

	/* process ID, used in multi-core /multi-CPU system */
	int id;

	/* the 32 architecture registers */
	int r[MAX_REG_NUM];

} core_t, *core_p;


extern int init_cores(int num);
extern core_p get_core(int id);

extern inline uint32_t get_reg(core_p core, int rid);
extern inline     void set_reg(core_p core, int rid, uint32_t v);

#endif	/* _CORE_H */

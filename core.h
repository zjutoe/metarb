#ifndef _CORE_H
#define _CORE_H

#include <stdint.h>

#define MAX_CORE_NUM 256

#define REG_ZERO     0
#define REG_PC       32
#define REG_OVERFLOW 33
#define MAX_REG_NUM  34


/* A MIPS core */
typedef struct _core {

	/* process ID, used in multi-core /multi-CPU system */
	int id;

	/* program counter */
	unsigned int pc;

	/* the 32 architecture registers */
	int r[32];

} core_t, *core_p;

//typedef core_t* core_p;

extern void core_init(core_p core, int id);
extern core_p get_core(int id);

extern inline uint32_t get_reg(core_p core, int rid);
extern inline     void set_reg(core_p core, int rid, uint32_t v);

#endif	/* _CORE_H */

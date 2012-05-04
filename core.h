#ifndef _CORE_H
#define _CORE_H

#define MAX_CORE_NUM 256

#define r0    0


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


#endif	/* _CORE_H */

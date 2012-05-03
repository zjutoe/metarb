#ifndef _CORE_H
#define _CORE_H

#define MAX_CORE_NUM 256

#define r0    0


/* A MIPS core */
typedef struct _core {
	int id;			/* process ID, used in multi-core /multi-CPU system */

	unsigned int pc;	/* program counter */
	int r[32];		/* the 32 architecture registers */
} core_t, *core_p;

//typedef core_t* core_p;

void core_init(core_p core, int id);
core_p get_core(int id);

/* FIXME will this structure affect the performance? maybe we should
   use a 32-bit word, which is naturally supported by the native
   CPU? */
typedef struct _inst {
	short op    :    6;
	short rs    :    5;
	short rt    :    5;
	union {
		short rd    :    5;
		short imm   :    11;
	};
} inst_t;

#endif	/* _CORE_H */

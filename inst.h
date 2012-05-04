#ifndef _INST_H_
#define _INST_H_

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


#endif	/* _INST_H_ */

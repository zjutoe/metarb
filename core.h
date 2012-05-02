#ifndef _CORE_H
#define _CORE_H

typedef struct _core {
	int id;
} core_t;

typedef struct _inst {
	short op    :    6;
	short rs    :    5;
	short rt    :    5;
	union {
		short rd    :    5;
		short cnst  :    11;
	};
} inst_t;

#endif	/* _CORE_H */

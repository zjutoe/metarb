#include <stdio.h>

#include "utils.h"
#include "inst.h"
#include "core.h"

inline void dump_word_bin(unsigned long * p) 
{
	char tmp[33];
	int i;
	for (i=0; i<8; i++) {
		tmp[i<<2]   = *p & 0x8000 ? '1' : '0';
		tmp[i<<2+1] = *p & 0x4000 ? '1' : '0';
		tmp[i<<2+2] = *p & 0x2000 ? '1' : '0';
		tmp[i<<2+3] = *p & 0x1000 ? '1' : '0';
	}
	
	tmp[32] = 0;
	printf("%s\n", tmp);
}

inline void dump_inst(inst_t i)
{
	printf("op:%d, rs:%d, rt:%d, rd:%d, imm:%d\n", 
	       i.op,   i.rs,  i.rt,  i.rd,  i.imm);
}


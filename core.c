#include <stdio.h>

#include "core.h"
#include "utils.h"


int main(int argc, char* argv[])
{
	inst_t i0, i1;
	i0.op = 0;
	i0.rs = 1;
	i0.rt = 2;
	i0.rd = 3;

	i1.op = 8;
	i1.rs = 1;
	i1.rt = 3;
	i1.cnst = 33;

	void *p;
	p = &i0;
	printf ("i0 = 0x%x\n", *(unsigned int*)p);
	dump_inst(i0);

	p = &i1;
	printf ("i1 = 0x%x\n", *(unsigned int*)p);
	dump_inst(i1);
	
	return 0;
}

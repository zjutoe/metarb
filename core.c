#include <assert.h>
#include <stdlib.h>

#include "utils.h"
#include "core.h"

 
static core_p cores[MAX_CORE_NUM];
static int core_num;

void core_init(core_p core, int id)
{
	assert(id>=0 && id<MAX_CORE_NUM);

	core->id = id;
	core->r[0] = 0;
} 

int init_cores(int num)
{
	assert(num <= MAX_CORE_NUM && num > 0);

	core_p p = NULL;
	p = malloc(sizeof(core_t) * num);
	dieif(p == NULL, "Out Of Memory, failed to allocate cores...");
	
	int i;
	for (i=0; i<num; i++) {
		core_init(p, i);
		cores[i] = p;
		p += 1;		
	}
	core_num = num;
	
	return 0;
}

core_p get_core(int id)
{
	assert(id>=0 && id<=core_num);

	return cores[id];
}


#define RtypeRegs(inst)				\
	int s = inst.rs;			\
	int t = inst.rt;			\
	int d = inst.rd;

#define ItypeRegs(inst)				\
	int imm = inst.imm;			\
	int s = inst.rs;			\
	int t = inst.rt;

/*
 * add $d, $s, $t  --  $d = $s + $t
 */
inline int exec_ADD(core_t core, inst_t inst)
{
	RtypeRegs(inst);
	core.r[d] = core.r[s] + core.r[t];
}

inline int exec_ADDI(core_t core, inst_t inst)
{
	ItypeRegs(inst);
	core.r[t] = core.r[s] + imm;
}

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "logging.h"
#include "utils.h"
#include "core.h"

 
static core_p cores[MAX_CORE_NUM];
static int core_num;

static void core_init(core_p core, int id)
{
	LOG_T;

	assert(id>=0 && id<MAX_CORE_NUM);

	core->id = id;
	core->r[0] = 0;
} 

int init_cores(int num)
{
	LOG_T;

	if (num > MAX_CORE_NUM || num <= 0) {
		LOG_E("Trying to initialzie invalid number %d of cores", num);
		return -1;
	}

	core_p p = NULL;
	p = malloc(sizeof(core_t) * num);
	if (unlikely(p == NULL)) {
		LOG_C("Out Of Memory, failed to allocate %d cores...", num);
		exit(1);
	}
	
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
	LOG_T;

	assert(id>=0 && id<=core_num);

	return cores[id];
}

inline uint32_t get_reg(core_p core, int rid)
{
	assert(rid >= 0 && rid < MAX_REG_NUM);
	if (rid == 0)		// $0 is always 0
		return 0;
	return core->r[rid];
}

inline void set_reg(core_p core, int rid, uint32_t v)
{
	assert(rid >= 0 && rid < MAX_REG_NUM);
	if (rid == 0)		// $0 is always 0, never written
		return;
	core->r[rid] = v;
}

/*
 * add $d, $s, $t  --  $d = $s + $t FIXME handle overflow
 */
inline int exec_ADD(core_p core, inst_t inst)
{
	LOG_T;

	// TODO create a macro as a "R-type exec template" to avoid
	// the duplication
	int op   = OP(inst);
	int rs   = RS(inst);
	int rt   = RT(inst);
	int rd   = RD(inst);
	int sa   = SA(inst);
	int func = FUNC(inst);

	assert (op == 0);
	assert (core != NULL);

	// TODO need to validate all the fields of the inst

	uint64_t s = get_reg(core, rs);
	uint64_t t = get_reg(core, rt);
	uint64_t d = s + t;
	if (d >= 0x100000000)
		set_reg(core, REG_OVERFLOW, 1);
	set_reg(core, rd, d);
}

/*
 * addi $t, $s, imm -- $t = $s + imm FIXME handle overflow
 */
inline int exec_ADDI(core_p core, inst_t inst)
{
	LOG_T;

		//core.r[t] = core.r[s] + imm;
}

/*
 * addu $d, $s, $t  --  $d = $s + $t FIXME handle overflow
 */
inline int exec_ADDU(core_p core, inst_t inst)
{
	LOG_T;

	int op   = OP(inst);
	int rs   = RS(inst);
	int rt   = RT(inst);
	int rd   = RD(inst);
	int sa   = SA(inst);
	int func = FUNC(inst);

	// TODO validate the instruction

	assert (op == 0);
	assert (core != NULL);

	uint32_t s = get_reg(core, rs);
	uint32_t t = get_reg(core, rt);
	uint32_t d = s + t;

	set_reg(core, rd, d);
}

/*
 * addiu $t, $s, imm -- $t = $s + imm FIXME handle overflow
 */
inline int exec_ADDIU(core_p core, inst_t inst)
{
	LOG_T;

		//core.r[t] = core.r[s] + imm;
}

/*
 * and $d, $s, $t  --  $d = $s & $t
 */
inline int exec_AND(core_p core, inst_t inst)
{
	LOG_T;

	int op   = OP(inst);
	int rs   = RS(inst);
	int rt   = RT(inst);
	int rd   = RD(inst);
	int sa   = SA(inst);
	int func = FUNC(inst);

	// TODO validate the instruction

	assert (op == 0);
	assert (core != NULL);

	uint32_t s = get_reg(core, rs);
	uint32_t t = get_reg(core, rt);
	uint32_t d = s & t;

	set_reg(core, rd, d);

}

inline int exec_ANDI(core_p core, inst_t inst)
{
	LOG_T;

		//core.r[t] = core.r[s] & imm;
}


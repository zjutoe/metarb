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

// the operations below are routine for all R-type instructions
#define R_TYPE_EXEC_TEMPLATE(core, inst)	\
	int op, rs, rt, rd, sa, func;		\
	uint32_t s, t;				\
	do {					\
		op   = OP(inst);		\
		rs   = RS(inst);		\
		rt   = RT(inst);		\
		rd   = RD(inst);		\
		sa   = SA(inst);		\
		func = FUNC(inst);		\
						\
		assert (op == 0);		\
		assert (core != NULL);		\
						\
		s = get_reg(core, rs);		\
		t = get_reg(core, rt);		\
	} while(0)
	


// add $d, $s, $t  --  $d = $s + $t FIXME handle overflow
inline int exec_ADD(core_p core, inst_t inst)
{
	LOG_T;

	R_TYPE_EXEC_TEMPLATE(core, inst);
	// TODO need to validate all the fields of the inst comply
	// with the ADD instruction. I.e. the unspecified fields be
	// all zero's.

	// ADD treats the operands as signed values
	int32_t ss = s;
	int32_t st = t;

	uint64_t d = ss + st;

	// a trick to test overflow. TODO Well, I learn this trick
	// from the SPEC, but don't understand it very well.
	uint32_t test_bits = (d >> 31) & 0x03;
	if (test_bits!=0 && test_bits!=0x03)
		set_reg(core, REG_OVERFLOW, 1); //FIXME spec says need
						//to raise an
						//exception
	
	set_reg(core, rd, d);
}



// addu $d, $s, $t  --  $d = $s + $t FIXME handle overflow
inline int exec_ADDU(core_p core, inst_t inst)
{
	LOG_T;

	R_TYPE_EXEC_TEMPLATE(core, inst);
	// TODO validate the instruction

	uint32_t d = s + t;
	set_reg(core, rd, d);
}



// and $d, $s, $t  --  $d = $s & $t
inline int exec_AND(core_p core, inst_t inst)
{
	LOG_T;

	R_TYPE_EXEC_TEMPLATE(core, inst);

	uint32_t d = s & t;
	set_reg(core, rd, d);
}

// div $d, $s, $t  --  $d = $s / $t
inline int exec_DIV(core_p core, inst_t inst)
{
	LOG_T;

	R_TYPE_EXEC_TEMPLATE(core, inst);

	// DIV treats the operands as signed values
	int32_t ss = s;
	int32_t st = t;

	int32_t lo = ss / st;
	int32_t hi = ss % st;
	set_reg(core, REG_LO, lo);
	set_reg(core, REG_HI, hi);
}

// divu $d, $s, $t  --  $d = $s / $t
inline int exec_DIVU(core_p core, inst_t inst)
{
	LOG_T;

	R_TYPE_EXEC_TEMPLATE(core, inst);

	uint32_t lo = s / t;
	uint32_t hi = s % t;
	set_reg(core, REG_LO, lo);
	set_reg(core, REG_HI, hi);
}


inline int exec_MFHI(core_p core, inst_t inst)
{
	LOG_T;

	uint32_t rd = get_reg(core, REG_HI);
	set_reg(core, REG_HI, rd);
}


inline int exec_MFLO(core_p core, inst_t inst)
{
	LOG_T;

	uint32_t rd = get_reg(core, REG_LO);
	set_reg(core, REG_HI, rd);
}


// addi $t, $s, imm -- $t = $s + imm FIXME handle overflow
inline int exec_ADDI(core_p core, inst_t inst)
{
	LOG_T;

	//core.r[t] = core.r[s] + imm;
}


// addiu $t, $s, imm -- $t = $s + imm FIXME handle overflow
inline int exec_ADDIU(core_p core, inst_t inst)
{
	LOG_T;

		//core.r[t] = core.r[s] + imm;
}

inline int exec_ANDI(core_p core, inst_t inst)
{
	LOG_T;

		//core.r[t] = core.r[s] & imm;
}


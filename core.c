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
	int32_t s, t;				\
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
//    temp <- (GPR[rs]31||GPR[rs]31..0) + (GPR[rt]31||GPR[rt]31..0) 
//    if temp32 != temp31 then
//        SignalException(IntegerOverflow)
//    else
//        GPR[rd] <- temp
//    endif
inline int exec_ADD(core_p core, inst_t inst)
{
	LOG_T;

	R_TYPE_EXEC_TEMPLATE(core, inst);
	// TODO need to validate all the fields of the inst comply
	// with the ADD instruction. I.e. the unspecified fields be
	// all zero's.

	int64_t ls = s;
	uint64_t lus = ls;
	lus &= 0x00000001FFFFFFFF;

	int64_t lt = t;
	uint64_t lut = lt;
	lut &= 0x00000001FFFFFFFF;

	uint64_t lud = lus + lut;

	int test32 = (lud<<31)>>63;
	int test31 = (lud<<32)>>63;
	if (test32 != test31)
		//FIXME spec says need to raise an exception
		set_reg(core, REG_OVERFLOW, 1);

	int32_t d = lud;
	set_reg(core, rd, d);

	return 0;
}


// addu $d, $s, $t  --  $d = $s + $t
inline int exec_ADDU(core_p core, inst_t inst)
{
	LOG_T;

	R_TYPE_EXEC_TEMPLATE(core, inst);
	// TODO validate the instruction

	uint32_t d = s + t;
	set_reg(core, rd, d);

	return 0;
}



// and $d, $s, $t  --  $d = $s & $t
inline int exec_AND(core_p core, inst_t inst)
{
	LOG_T;

	R_TYPE_EXEC_TEMPLATE(core, inst);

	uint32_t d = s & t;
	set_reg(core, rd, d);

	return 0;
}

// div $d, $s, $t  --  ($hi,$lo) = $s / $t
inline int exec_DIV(core_p core, inst_t inst)
{
	LOG_T;

	R_TYPE_EXEC_TEMPLATE(core, inst);

	int32_t lo = s / t;
	int32_t hi = s % t;
	set_reg(core, REG_LO, lo);
	set_reg(core, REG_HI, hi);

	return 0;
}

// divu $d, $s, $t  --  ($hi,$lo) = $s / $t
inline int exec_DIVU(core_p core, inst_t inst)
{
	LOG_T;

	R_TYPE_EXEC_TEMPLATE(core, inst);

	uint32_t us = s;
	uint32_t ut = t;

	uint32_t lo = us / ut;
	uint32_t hi = us % ut;
	set_reg(core, REG_LO, lo);
	set_reg(core, REG_HI, hi);

	return 0;
}


inline int exec_MFHI(core_p core, inst_t inst)
{
	LOG_T;

	uint32_t rd = get_reg(core, REG_HI);
	set_reg(core, REG_HI, rd);

	return 0;
}


inline int exec_MFLO(core_p core, inst_t inst)
{
	LOG_T;

	uint32_t rd = get_reg(core, REG_LO);
	set_reg(core, REG_HI, rd);

	return 0;
}


// mult $d, $s, $t  --  ($hi,$lo) = $s * $t
inline int exec_MULT(core_p core, inst_t inst)
{
	LOG_T;

	R_TYPE_EXEC_TEMPLATE(core, inst);

	uint64_t res = s * t;
	uint32_t lo = res;
	uint32_t hi = res >> 32;
	set_reg(core, REG_LO, lo);
	set_reg(core, REG_HI, hi);

	return 0;
}


// multu $d, $s, $t  --  ($hi,$lo) = $s * $t
inline int exec_MULTU(core_p core, inst_t inst)
{
	LOG_T;

	R_TYPE_EXEC_TEMPLATE(core, inst);

	uint32_t us = s;
	uint32_t ut = t;

	uint64_t res = us * ut;
	uint32_t lo = res;
	uint32_t hi = res >> 32;
	set_reg(core, REG_LO, lo);
	set_reg(core, REG_HI, hi);
}



// Noop
inline int exec_NOOP(core_p core, inst_t inst)
{
	LOG_T;
	// TODO validate the inst
	return 0;
}


inline int exec_OR(core_p core, inst_t inst)
{
	LOG_T;

	R_TYPE_EXEC_TEMPLATE(core, inst);
	// TODO validate the instruction

	uint32_t d = s | t;
	set_reg(core, rd, d);

	return 0;
}


// Set on Less Than
inline int exec_SLT(core_p core, inst_t inst)
{
	LOG_T;

	R_TYPE_EXEC_TEMPLATE(core, inst);
	// TODO validate the instruction

	uint32_t d = s < t;
	set_reg(core, rd, d);

	return 0;
}


// Set on Less Than Unsigned
inline int exec_SLTU(core_p core, inst_t inst)
{
	LOG_T;

	R_TYPE_EXEC_TEMPLATE(core, inst);
	// TODO validate the instruction

	uint32_t us = s;
	uint32_t ut = t;

	uint32_t d = us < ut;
	set_reg(core, rd, d);

	return 0;
}


// Shift Left Logical
inline int exec_SLL(core_p core, inst_t inst)
{
	LOG_T;

	R_TYPE_EXEC_TEMPLATE(core, inst);
	// TODO validate the instruction

	uint32_t d = s << sa;
	set_reg(core, rd, d);

	return 0;
}


// Shift Left Logical Value
inline int exec_SLLV(core_p core, inst_t inst)
{
	LOG_T;

	R_TYPE_EXEC_TEMPLATE(core, inst);
	// TODO validate the instruction

	uint32_t d = t << (s & 0x1F);
	set_reg(core, rd, d);

	return 0;
}


// Shift Right Arithmetic
inline int exec_SRA(core_p core, inst_t inst)
{
	LOG_T;

	R_TYPE_EXEC_TEMPLATE(core, inst);
	// TODO validate the instruction

	int32_t d = t >> sa;
	set_reg(core, rd, d);

	return 0;
}


// Shift Right Logical
inline int exec_SRL(core_p core, inst_t inst)
{
	LOG_T;

	R_TYPE_EXEC_TEMPLATE(core, inst);
	// TODO validate the instruction

	uint32_t ut = t;
	uint32_t d = ut >> sa;
	set_reg(core, rd, d);

	return 0;
}


// Shift Right Logical Value
inline int exec_SRLV(core_p core, inst_t inst)
{
	LOG_T;

	R_TYPE_EXEC_TEMPLATE(core, inst);
	// TODO validate the instruction

	uint32_t ut = t;
	uint32_t d = ut >> (s & 0x1F);
	set_reg(core, rd, d);

	return 0;
}


// sub $d, $s, $t  --  $d = $s - $t FIXME handle overflow
// temp <- (GPR[rs]31||GPR[rs]31..0) - (GPR[rt]31||GPR[rt]31..0)
// if temp32 != temp31 then
//       SignalException(IntegerOverflow)
// else
//       GPR[rd] <-temp31..0 
// endif
inline int exec_SUB(core_p core, inst_t inst)
{
	LOG_T;

	R_TYPE_EXEC_TEMPLATE(core, inst);
	// TODO need to validate all the fields of the inst comply
	// with the ADD instruction. I.e. the unspecified fields be
	// all zero's.

	int64_t ls = s;
	uint64_t lus = ls;
	lus &= 0x00000001FFFFFFFF;

	int64_t lt = t;
	uint64_t lut = lt;
	lut &= 0x00000001FFFFFFFF;

	uint64_t lud = lus - lut;

	int test32 = (lud<<31)>>63;
	int test31 = (lud<<32)>>63;
	if (test32 != test31)
		//FIXME spec says need to raise an exception
		set_reg(core, REG_OVERFLOW, 1);

	int32_t d = lud;
	set_reg(core, rd, d);

	return 0;
}



// subu $d, $s, $t  --  $d = $s - $t
inline int exec_SUBU(core_p core, inst_t inst)
{
	LOG_T;

	R_TYPE_EXEC_TEMPLATE(core, inst);
	// TODO validate the instruction

	uint32_t d = s - t;
	set_reg(core, rd, d);

	return 0;
}


// TODO R_SYSCALL system call


// xor $d, $s, $t  --  $d = $s ^ $t
inline int exec_XOR(core_p core, inst_t inst)
{
	LOG_T;

	R_TYPE_EXEC_TEMPLATE(core, inst);

	uint32_t d = s ^ t;
	set_reg(core, rd, d);

	return 0;
}

// restrict this macro in this file
#undef R_TYPE_EXEC_TEMPLATE


#define I_TYPE_EXEC_TEMPLATE(core, inst)	\
	int op, rs, rt;				\
	int16_t imm;				\
	int32_t s;				\
	do {					\
		op   = OP(inst);		\
		rs   = RS(inst);		\
		rt   = RT(inst);		\
		imm  = IMM(inst);		\
		assert (core != NULL);		\
						\
		s = get_reg(core, rs);		\
	} while(0)


// addi $t, $s, imm -- $t = $s + imm FIXME handle overflow
inline int exec_ADDI(core_p core, inst_t inst)
{
	LOG_T;

	I_TYPE_EXEC_TEMPLATE(core, inst);

	int64_t ls = s;
	uint64_t lus = ls;
	lus &= 0x00000001FFFFFFFF;

	int64_t limm = imm;
	uint64_t luimm = limm;
	luimm &= 0x00000001FFFFFFFF;

	uint64_t lut = lus + luimm;

	int test32 = (lut<<31)>>63;
	int test31 = (lut<<32)>>63;
	if (test32 != test31)
		//FIXME spec says need to raise an exception
		set_reg(core, REG_OVERFLOW, 1);

	int32_t t = lut;

	set_reg(core, rt, t);

	return 0;
}


// addiu $t, $s, imm -- $t = $s + imm FIXME handle overflow
inline int exec_ADDIU(core_p core, inst_t inst)
{
	LOG_T;

	I_TYPE_EXEC_TEMPLATE(core, inst);

	uint32_t t = s + imm;

	// DON'T check overflow in ADDIU

	set_reg(core, rt, t);

	return 0;
}

inline int exec_ANDI(core_p core, inst_t inst)
{
	LOG_T;

	I_TYPE_EXEC_TEMPLATE(core, inst);

	uint32_t t = s & imm;
	set_reg(core, rt, t);

	return 0;
}


inline int exec_ORI(core_p core, inst_t inst)
{
	LOG_T;

	I_TYPE_EXEC_TEMPLATE(core, inst);

	uint32_t t = s | imm;
	set_reg(core, rt, t);

	return 0;
}


inline int exec_SLTI(core_p core, inst_t inst)
{
	LOG_T;

	I_TYPE_EXEC_TEMPLATE(core, inst);

	uint32_t t = s < imm;
	set_reg(core, rt, t);

	return 0;
}


inline int exec_SLTIU(core_p core, inst_t inst)
{
	LOG_T;

	I_TYPE_EXEC_TEMPLATE(core, inst);

	int32_t simm = imm;
	uint32_t uimm = simm;
	uint32_t us = s;
	
	uint32_t t = us < uimm;
	set_reg(core, rt, t);

	return 0;
}


inline int exec_XORI(core_p core, inst_t inst)
{
	LOG_T;

	I_TYPE_EXEC_TEMPLATE(core, inst);

	uint32_t t = s ^ imm;
	set_reg(core, rt, t);

	return 0;
}


inline int exec_BEQ(core_p core, inst_t inst)
{
	LOG_T;

	I_TYPE_EXEC_TEMPLATE(core, inst);

	int32_t t = get_reg(core, rt);
	uint32_t current_pc = get_reg(core, REG_PC);
	int32_t offset = imm;	//to sign extend imm
	if (s == t) {
		set_reg(core, REG_PC, current_pc + (offset << 2));
	}

	return 0;
}

inline int exec_BNE(core_p core, inst_t inst)
{
	LOG_T;

	I_TYPE_EXEC_TEMPLATE(core, inst);

	int32_t t = get_reg(core, rt);
	uint32_t current_pc = get_reg(core, REG_PC);
	int32_t offset = imm;	//to sign extend imm
	if (s != t) {
		set_reg(core, REG_PC, current_pc + (offset << 2));
	}

	return 0;
}


inline int exec_BGEZ(core_p core, inst_t inst)
{
	LOG_T;

	I_TYPE_EXEC_TEMPLATE(core, inst);

	uint32_t current_pc = get_reg(core, REG_PC);
	int32_t offset = imm;	//to sign extend imm
	if (s >= 0) {
		set_reg(core, REG_PC, current_pc + (offset << 2));
	}

	return 0;
}


inline int exec_BGTZ(core_p core, inst_t inst)
{
	LOG_T;

	I_TYPE_EXEC_TEMPLATE(core, inst);

	uint32_t current_pc = get_reg(core, REG_PC);
	int32_t offset = imm;	//to sign extend imm
	if (s > 0) {
		set_reg(core, REG_PC, current_pc + (offset << 2));
	}

	return 0;
}

inline int exec_BLEZ(core_p core, inst_t inst)
{
	LOG_T;

	I_TYPE_EXEC_TEMPLATE(core, inst);

	uint32_t current_pc = get_reg(core, REG_PC);
	int32_t offset = imm;	//to sign extend imm
	if (s <= 0) {
		set_reg(core, REG_PC, current_pc + (offset << 2));
	}

	return 0;
}

inline int exec_BLTZ(core_p core, inst_t inst)
{
	LOG_T;

	I_TYPE_EXEC_TEMPLATE(core, inst);

	uint32_t current_pc = get_reg(core, REG_PC);
	int32_t offset = imm;	//to sign extend imm
	if (s < 0) {
		set_reg(core, REG_PC, current_pc + (offset << 2));
	}

	return 0;
}

int32_t byte_of_word(uint32_t data, int i)
{
	int sl = (3 - i) << 3;
	int8_t d = (data << sl) >> 24;
	return d;
}

// TODO support both little endian and big endian
inline int exec_LB(core_p core, inst_t inst)
{
	LOG_T;

	I_TYPE_EXEC_TEMPLATE(core, inst);

	uint32_t current_pc = get_reg(core, REG_PC);
	int32_t offset = imm;	//to sign extend imm
	uint32_t addr = offset + s;

	uint32_t data = mem_read(addr & 0xFFFFFFFC);
	LOG_D("loaded word=0x%x", data);
	int32_t datab = byte_of_word(data, addr % 4); // assume little endian now
	LOG_D("loaded byte=0x%x", datab);
	set_reg(core, rt, datab);

	return 0;
}

// TODO support both little endian and big endian
inline int exec_LBU(core_p core, inst_t inst)
{
	LOG_T;

	I_TYPE_EXEC_TEMPLATE(core, inst);

	uint32_t current_pc = get_reg(core, REG_PC);
	int32_t offset = imm;	//to sign extend imm
	uint32_t addr = offset + s;

	uint32_t data = mem_read(addr & 0xFFFFFFFC);
	LOG_D("loaded word=0x%x", data);
	uint32_t datab = byte_of_word(data, addr % 4); // assume little endian now
	LOG_D("loaded byte=0x%x", datab);
	set_reg(core, rt, datab);

	return 0;
}

// TODO support both little endian and big endian
inline int exec_LH(core_p core, inst_t inst)
{
	LOG_T;

	I_TYPE_EXEC_TEMPLATE(core, inst);

	uint32_t current_pc = get_reg(core, REG_PC);
	int32_t offset = imm;	//to sign extend imm
	uint32_t addr = offset + s;

	uint32_t data = mem_read(addr & 0xFFFFFFFC);
	LOG_D("loaded word=0x%x", data);
	uint32_t datab = byte_of_word(data, addr % 4); // assume little endian now
	LOG_D("loaded byte=0x%x", datab);
	set_reg(core, rt, datab);

	return 0;
}


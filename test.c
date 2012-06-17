#include <assert.h>
#include <stdint.h>

#include "core.h"
#include "inst.h"
#include "mem.h"
#include "logging.h"
#include "test.h"


#define TEST_PROGRAM_LEN 128
static uint32_t test_program_bin[TEST_PROGRAM_LEN] = {
	
	0x014b4820,	// add $t1, $t2, $t3
	0x014b4820,	// add $t1, $t2, $t3
	0x016c5020,	// add $t2, $t3, $t4
	0x0		// Noop FIXME should use "halt" to terminate,
			// but don't find the inst in the spec yet.
};

int base_test()
{
	LOG_T;

	int ret;
	int i;

	mem_init();
	for (i=0; i<10; i++)
		mem_write(i, i*2);
	for (i=0; i<10; i++) 
		printf("0x%x ", mem_read(i));
	printf("\n");
	mem_fini();
	return 0;
	

	init_cores(1);
	core_p core = get_core(0);

	randomize_core(core);

	uint32_t inst;
	i=0;
	do {
		inst = test_program_bin[i];
		ret = test_r_type(core, inst);
		if (ret)
			goto exit;

		i++;
	} while (inst != 0x0 && i<TEST_PROGRAM_LEN);

 exit:
	return ret;
}


// to randomly set the initial values of a core 
// TODO by now we just manually set some values. should use random
// functions in the future
void randomize_core(core_p core)
{
	int i;
	for (i=1; i<32; i++)
		set_reg(core, i, i);
	set_reg(core, REG_PC, 0);
	set_reg(core, REG_OVERFLOW, 0);
}

void dump_r_type_inst(inst_t inst)
{
	LOG_T;

	LOG_D("inst=%lx : op=%d rs=%d rt=%d rd=%d sa=%d func=%d",
	       inst,
	       OP(inst),
	       RS(inst),
	       RT(inst),
	       RD(inst),
	       SA(inst),
	       FUNC(inst)
	       );

}

// TODO to generate randomized inputs to drive the test cases, like
// what they do in OVM testbench for hardware design
int test_r_type(core_p core, inst_t inst)
{
	LOG_T;

	int op   = OP(inst);
	int rs   = RS(inst);
	int rt   = RT(inst);
	int rd   = RD(inst);
	int sa   = SA(inst);
	int func = FUNC(inst);

	dump_r_type_inst(inst);

	// Registers are unsigned. When reading them to signed
	// variables, then they are treated (converted) to signed. So
	// we use signed and unsigned s, t and d for signed and
	// unsigned instructions.
	uint32_t us, ut, ud, ulo, uhi;
	int32_t  s, t, d, lo, hi;
	uint64_t ud64;

	assert (op == 0);
	assert (core);

	switch (func) {
	case    R_ADD:		// add signed (with overflow) 
		LOG_T;

		s = get_reg(core, rs);
		t = get_reg(core, rt);
		ud64 = s + t;
		set_reg(core, REG_OVERFLOW, 0);
		exec_ADD(core, inst);
		int is_overflow = get_reg(core, REG_OVERFLOW);

		// if the 2 bits ud64[32]!=ud64[31], then overflow occurs
		int bit32 = ud64 & 0x100000000;
		int bit31 = ud64 &  0x80000000;
		assert(bit32 != bit31 ? is_overflow == 1 : is_overflow == 0);
		assert(get_reg(core, rd) == ud64);

		break;
		
	case    R_ADDU:    	// add unsigned (no overflow)
		LOG_T;
		us = get_reg(core, rs);
		ut = get_reg(core, rt);
		ud = s + t;
		exec_ADDU(core, inst);

		assert(get_reg(core, rd) == ud);

		break;
		
	case    R_AND:     	// bitwise and
		LOG_T;
		us = get_reg(core, rs);
		ut = get_reg(core, rt);
		ud = us & ut;
		exec_AND(core, inst);

		assert(get_reg(core, rd) == ud);

		break;

	case    R_DIV:     	// divide signed 
		LOG_T;
		s = get_reg(core, rs);
		t = get_reg(core, rt);
		lo = s / t;
		hi = s % t;
		exec_DIV(core, inst);
		
		assert(get_reg(core, REG_LO) == lo);
		assert(get_reg(core, REG_HI) == hi);

		break;

	case    R_DIVU:    	// divide unsigned 
		LOG_T;
		us = get_reg(core, rs);
		ut = get_reg(core, rt);
		ulo = us / ut;
		uhi = us % ut;
		exec_DIV(core, inst);
		
		assert(get_reg(core, REG_LO) == ulo);
		assert(get_reg(core, REG_HI) == uhi);

		break;

	case    R_MFHI:    	// move from HI 
		LOG_T;
		break;
	case    R_MFLO:    	// move from LO 
		LOG_T;
		break;
	case    R_MULT:    	// multiply signed 
		LOG_T;
		break;
	case    R_MULTU:   	// multiply unsigned 
		LOG_T;
		break;
	// case    R_NOOP:    	// no-op FIXME shall we say noop is R-style? 
	// 	break;
	case    R_OR:      	// bitwise or 
		LOG_T;
		break;
	case    R_SLT:     	// set on less than (signed) 
		LOG_T;
		break;
	case    R_SLTU:    	// set on less than immediate (signed) 
		LOG_T;
		break;
	case    R_SLL:     	// shift left logical 
		LOG_T;
		break;
	case    R_SLLV:    	// shift left logical variable 
		LOG_T;
		break;
	case    R_SRA:     	// shift right arithmetic 
		LOG_T;
		break;
	case    R_SRL:     	// shift right logic  
		LOG_T;
		break;
	case    R_SRLV:    	// shift right logical variable 
		LOG_T;
		break;
	case    R_SUB:     	// sub signed 
		LOG_T;
		break;
	case    R_SUBU:    	// sub unsigned 
		LOG_T;
		break;
	case    R_SYSCALL: 	// system call FIXME system call is not R-type in theory? 
		LOG_T;
		break;
	case    R_XOR:     	// bitwise exclusive or 
		LOG_T;
		break;
	default:
		LOG_T;
		break;
	}

	return 0;
}

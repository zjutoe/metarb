#include <assert.h>
#include <stdint.h>

#include "core.h"
#include "inst.h"

#include "test.h"

// to build a R-type instruction. opcode is not listed as parameter
// because it's always 0
uint32_t build_r_type_inst(int rs, int rt, int rd, int sa, int func)
{
	uint32_t inst;

	// We do not sanitize the built instruction here, because we
	// may try to build invalid instructions and feed to the core
	// on purpose
	inst = (rs<<21) | (rt<<16) | (rd<<11) | (sa<<6) | func;

	return inst;
}

// TODO to generate randomized inputs to drive the test cases, like
// what they do in OVM testbench for hardware design
int test_r_type(core_p core, inst_t inst)
{
	int op   = OP(inst);
	int rs   = RS(inst);
	int rt   = RT(inst);
	int rd   = RD(inst);
	int sa   = SA(inst);
	int func = FUNC(inst);

	assert (op == 0);
	assert (core);

	switch (func) {
	case    R_ADD:		// add signed (with overflow) 
		uint64_t s = get_reg(core, rs);
		uint64_t t = get_reg(core, rt);
		uint64_t d = s + t;
		exec_ADD(core, inst);
		assert(get_reg(core, rd) == d);
		if (d >= 0x100000000)
			assert(get_reg(core, REG_OVERFLOW) == 1);
		break;
		
	case    R_ADDU:    	// add unsigned (no overflow)
		uint32_t s = get_reg(core, rs);
		uint32_t t = get_reg(core, rt);
		uint32_t d = s + t;
		exec_ADDU(core, inst);
		assert(get_reg(core, rd) == d);
		break;
		
	case    R_AND:     	// bitwise and
		uint32_t s = get_reg(core, rs);
		uint32_t t = get_reg(core, rt);
		uint32_t d = s & t;
		set_reg(core, rd, d);
		break;

	case    R_DIV:     	// divide signed 
		break;
	case    R_DIVU:    	// divide unsigned 
		break;
	case    R_MFHI:    	// move from HI 
		break;
	case    R_MFLO:    	// move from LO 
		break;
	case    R_MULT:    	// multiply signed 
		break;
	case    R_MULTU:   	// multiply unsigned 
		break;
	case    R_NOOP:    	// no-op FIXME shall we say noop is R-style? 
		break;
	case    R_OR:      	// bitwise or 
		break;
	case    R_SLT:     	// set on less than (signed) 
		break;
	case    R_SLTU:    	// set on less than immediate (signed) 
		break;
	case    R_SLL:     	// shift left logical 
		break;
	case    R_SLLV:    	// shift left logical variable 
		break;
	case    R_SRA:     	// shift right arithmetic 
		break;
	case    R_SRL:     	// shift right logic  
		break;
	case    R_SRLV:    	// shift right logical variable 
		break;
	case    R_SUB:     	// sub signed 
		break;
	case    R_SUBU:    	// sub unsigned 
		break;
	case    R_SYSCALL: 	// system call FIXME system call is not R-type in theory? 
		break;
	case    R_XOR:     	// bitwise exclusive or 
		break;
	default:
		break;
	}
	
}

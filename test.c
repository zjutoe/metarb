#include <assert.h>
#include <stdint.h>

#include "core.h"
#include "inst.h"
#include "logging.h"
#include "test.h"


int base_test()
{
	LOG_T;

	int ret;

	init_cores(1);
	core_p core = get_core(0);

	randomize_core(core);

	inst_t i0 = build_r_type_inst(1, 2, 3, 0, R_ADD);
	inst_t i1 = build_r_type_inst(1, 2, 3, 0, R_ADDU);

	
	ret = test_r_type(core, i0);
	if (ret)
		goto exit;

	ret = test_r_type(core, i1);

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


// to build a R-type instruction. opcode is not listed as parameter
// because it's always 0
uint32_t build_r_type_inst(int rs, int rt, int rd, int sa, int func)
{
	LOG_T;
	uint32_t inst;
	syslog(LOG_DEBUG, "[D]rs=%d rt=%d rd=%d sa=%d func=%d\n",
	       rs,
	       rt,
	       rd,
	       sa,
	       func
	       );

	// We do not sanitize the built instruction here, because we
	// may try to build invalid instructions and feed to the core
	// on purpose
	inst = (rs<<21) | (rt<<16) | (rd<<11) | (sa<<6) | func;

	dump_r_type_inst(inst);

	return inst;
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

	uint64_t s, t, d;

	assert (op == 0);
	assert (core);

	switch (func) {
	case    R_ADD:		// add signed (with overflow) 
		LOG_T;
		s = get_reg(core, rs);
		t = get_reg(core, rt);
		d = s + t;
		exec_ADD(core, inst);

		
		test_bits = (d >> 31) & 0x03;
		// if the 2 bits d[32]!=d[31], then overflow occurs
		assert((test_bits!=0 && test_bits!=0x03) ? 
		       get_reg(core, REG_OVERFLOW) == 0 : (get_reg(core, REG_OVERFLOW) == 1));
		assert(get_reg(core, rd) == d & 0xFFFFFFFF);
		break;
		
	case    R_ADDU:    	// add unsigned (no overflow)
		LOG_T;
		s = get_reg(core, rs);
		t = get_reg(core, rt);
		d = s + t;
		exec_ADDU(core, inst);

		assert(get_reg(core, rd) == d & 0xFFFFFFFF);
		break;
		
	case    R_AND:     	// bitwise and
		LOG_T;
		s = get_reg(core, rs);
		t = get_reg(core, rt);
		d = s & t;
		set_reg(core, rd, d);
		break;

	case    R_DIV:     	// divide signed 
		LOG_T;
		break;
	case    R_DIVU:    	// divide unsigned 
		LOG_T;
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

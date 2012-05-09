#ifndef _TEST_H_
#define _TEST_H_

extern int base_test();
extern int test_r_type(core_p core, inst_t inst);
extern void randomize_core(core_p core);
extern uint32_t build_r_type_inst(int rs, int rt, int rd, int sa, int func);

#endif	//_TEST_H_

#ifndef _TEST_H_
#define _TEST_H_

extern uint32_t build_r_type_inst(int rs, int rt, int rd, int sa, int func);
extern int test_r_type(core_p core, inst_t inst);

#endif	//_TEST_H_

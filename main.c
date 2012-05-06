#include <stdio.h>

#include "core.h"
#include "inst.h"
#include "utils.h"

int main(int argc, char* argv[])
{
	init_cores(1);
	core_p core = get_core(0);

	inst_t i0 = build_r_type_inst(1, 2, 3, 0, R_ADD);
	inst_t i1 = build_r_type_inst(1, 2, 3, 0, R_ADDU);

	test_r_type(core, i0);
	test_r_type(core, i1);

	return 0;
}

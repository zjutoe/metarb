#include <stdio.h>
#include <assert.h>

#include "core.h"
#include "inst.h"
#include "utils.h"
#include "test.h"
#include "logging.h"

int main(int argc, char* argv[])
{
	INIT_LOG;
	LOG_T;

	// init_cores(1);
	// core_p core = get_core(0);

	// inst_t i0 = build_r_type_inst(1, 2, 3, 0, R_ADD);
	// inst_t i1 = build_r_type_inst(1, 2, 3, 0, R_ADDU);

	// test_r_type(core, i0);
	// LOG_D("hah!");
	// test_r_type(core, i1);
		
	base_test();
	LOG_I("test here %d", 42);
	FINI_LOG;
		
	return 0;
}

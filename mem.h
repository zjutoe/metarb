#ifndef _MEM_H_
#define	_MEM_H_

#include <stdint.h>

extern mem_init();
extern mem_fini();
extern uint32_t mem_read(uint32_t addr);
extern void mem_write(uint32_t addr, uint32_t val);

#endif				//_MEM_H_

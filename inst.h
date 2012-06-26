#ifndef _INST_H_
#define _INST_H_

#include <stdint.h>		// for uint32_t

 // FIXME will this structure affect the performance? maybe we should
 //   use a 32-bit word, which is naturally supported by the native
 //   CPU?
// typedef struct _inst {
// 	short op    :    6;
// 	short rs    :    5;
// 	short rt    :    5;
// 	union {
// 		short rd    :    5;
// 		short imm   :    11;
// 	};
// } inst_t;

typedef uint32_t inst_t;

#define OP(i)    ((i)  >> 26)	         // 31:26
#define RS(i)    (((i) >> 21) & 0x1F)    // 25:21
#define RT(i)    (((i) >> 16) & 0x1F)    // 20:16
#define RD(i)    (((i) >> 11) & 0x1F)    // 15:11
#define SA(i)    (((i) >>  6) & 0x1F)    // 10:6
#define FUNC(i)  ((i)  & 0x3F)		 // 5:0

#define IMM(i)   ((i) & 0xFFFF)	         // 15:0



#define OP_R         0x00	/* R-Type opcode, need to check the funt code further */
#define    R_ADD     0x20	/* add signed (with overflow) */
#define    R_ADDU    0x21	/* add unsigned */
#define    R_AND     0x24	/* bitwise and */
#define    R_DIV     0x1A	/* divide signed */
#define    R_DIVU    0x1B	/* divide unsigned */
#define    R_MFHI    0x10	/* move from HI */
#define    R_MFLO    0x12	/* move from LO */
#define    R_MULT    0x18	/* multiply signed */
#define    R_MULTU   0x19	/* multiply unsigned */
#define    R_NOOP    0x00	/* no-op FIXME shall we say noop is R-style? */
#define    R_OR      0x25	/* bitwise or */
#define    R_SLT     0x2A	/* set on less than (signed) */
#define    R_SLTU    0x2B	/* set on less than immediate (signed) */
#define    R_SLL     0x00	/* shift left logical */
#define    R_SLLV    0x04	/* shift left logical variable */
#define    R_SRA     0x03	/* shift right arithmetic */
#define    R_SRL     0x02	/* shift right logic  */
#define    R_SRLV    0x06	/* shift right logical variable */
#define    R_SUB     0x22	/* sub signed */
#define    R_SUBU    0x23	/* sub unsigned */
#define    R_SYSCALL 0x0C	/* system call FIXME system call is not R-type in theory? */
#define    R_XOR     0x26	/* bitwise exclusive or */
 
#define OP_ADDI      0x08	/* add immediate with overflow */
#define OP_ADDIU     0x09	/* add immediate no overflow */
#define OP_ANDI      0x0C	/* bitwise and immediate */
#define OP_ORI       0x0D	/* bitwise or immediate */
#define OP_XORI      0x0E	/* bitwise exclusive or immediate */

#define OP_SLTI      0x0A	/* set on less than immediate */
#define OP_SLTIU     0x0B    	/* set on less than immediate unsigned */

#define OP_BEQ       0x04	/* branch on equal */
#define OP_BZ        0x01	/* the following 4 branches share this OP code */
#define    BZ_BGEZ   0x01	/* fmt=0x01, BGEZ, branch on >= 0*/
#define    BZ_BGEZAL 0x11	/* fmt=0x11, BGEZAL, BGEZ and link */
#define    BZ_BLTZ   0x00	/* fmt=0x00, BLTZ, branch on < 0 */
#define    BZ_BLTZAL 0x10	/* fmt=0x10, BLTZAL, BLTZ and link */

#define OP_BGTZ      0x07	/* branch if $s > 0 */
#define OP_BLEZ      0x06	/* branch if $s <= 0 */
#define OP_BNE       0x05	/* branch if $s != $t */
#define OP_J         0x02	/* jump */
#define OP_JAL       0x03	/* jump and link */

#define OP_LB        0x20	/* load byte */
#define OP_LBU       0x24	/* load byte unsigned */
#define OP_LH        0x21	/*  */
#define OP_LHU       0x25	/*  */
#define OP_LUI       0x0F	/* load upper immediate */
#define OP_LW        0x23	/* load word */
#define OP_LWCL      0x31	/* load word */
#define OP_SB        0x28	/* store byte */
#define OP_SH        0x29	/*  */
#define OP_SW        0x2B	/* store word */
#define OP_SWCL      0x39	/*  */


#endif	/* _INST_H_ */

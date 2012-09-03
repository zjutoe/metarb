case R_ADD:
    exec_ADD(core, inst);
    break;
case R_ADDU:
    exec_ADDU(core, inst);
    break;
case R_AND:
    exec_AND(core, inst);
    break;
case R_DIV:
    exec_DIV(core, inst);
    break;
case R_DIVU:
    exec_DIVU(core, inst);
    break;
case R_MFHI:
    exec_MFHI(core, inst);
    break;
case R_MFLO:
    exec_MFLO(core, inst);
    break;
case R_MULT:
    exec_MULT(core, inst);
    break;
case R_MULTU:
    exec_MULTU(core, inst);
    break;
case R_OR:
    exec_OR(core, inst);
    break;
case R_SLT:
    exec_SLT(core, inst);
    break;
case R_SLTU:
    exec_SLTU(core, inst);
    break;
case R_SLL:
    exec_SLL(core, inst);
    break;
case R_SLLV:
    exec_SLLV(core, inst);
    break;
case R_SRA:
    exec_SRA(core, inst);
    break;
case R_SRL:
    exec_SRL(core, inst);
    break;
case R_SRLV:
    exec_SRLV(core, inst);
    break;
case R_SUB:
    exec_SUB(core, inst);
    break;
case R_SUBU:
    exec_SUBU(core, inst);
    break;
case R_SYSCALL:
    exec_SYSCALL(core, inst);
    break;
case R_XOR:
    exec_XOR(core, inst);
    break;

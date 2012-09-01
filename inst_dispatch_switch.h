case OP_R:
    exec_R(core, inst);
    break;
case OP_ADDI:
    exec_ADDI(core, inst);
    break;
case OP_ADDIU:
    exec_ADDIU(core, inst);
    break;
case OP_ANDI:
    exec_ANDI(core, inst);
    break;
case OP_ORI:
    exec_ORI(core, inst);
    break;
case OP_XORI:
    exec_XORI(core, inst);
    break;
case OP_SLTI:
    exec_SLTI(core, inst);
    break;
case OP_SLTIU:
    exec_SLTIU(core, inst);
    break;
case OP_BEQ:
    exec_BEQ(core, inst);
    break;
case OP_BZ:
    exec_BZ(core, inst);
    break;
case OP_BGTZ:
    exec_BGTZ(core, inst);
    break;
case OP_BLEZ:
    exec_BLEZ(core, inst);
    break;
case OP_BNE:
    exec_BNE(core, inst);
    break;
case OP_J:
    exec_J(core, inst);
    break;
case OP_JAL:
    exec_JAL(core, inst);
    break;
case OP_LB:
    exec_LB(core, inst);
    break;
case OP_LBU:
    exec_LBU(core, inst);
    break;
case OP_LH:
    exec_LH(core, inst);
    break;
case OP_LHU:
    exec_LHU(core, inst);
    break;
case OP_LUI:
    exec_LUI(core, inst);
    break;
case OP_LW:
    exec_LW(core, inst);
    break;
case OP_LWCL:
    exec_LWCL(core, inst);
    break;
case OP_SB:
    exec_SB(core, inst);
    break;
case OP_SH:
    exec_SH(core, inst);
    break;
case OP_SW:
    exec_SW(core, inst);
    break;
case OP_SWCL:
    exec_SWCL(core, inst);
    break;

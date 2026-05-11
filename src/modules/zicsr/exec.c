
#include "exec.h"

void exec_csrrw(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t csr = (uint32_t)inst_imm_i(inst);
}



#include <stdint.h>

#include "exec.h"
#include "instruction.h"
#include "vm.h"

void exec_addi(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        vm_reg_write(vm, rd, vm_reg_read(vm, rs1) + (uint32_t)imm_i);
}

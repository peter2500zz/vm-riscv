#include <stdint.h>

#include "exec.h"
#include "instruction.h"
#include "vm.h"

void exec_lui(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        int32_t imm_u = inst_imm_u(inst);

        vm_reg_write(vm, rd, (uint32_t)imm_u);
}

void exec_auipc(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        int32_t imm_u = inst_imm_u(inst);

        vm_reg_write(vm, rd, vm->pc + (uint32_t)imm_u);
}

void exec_jal(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        int32_t imm_j = inst_imm_j(inst);

        vm_reg_write(vm, rd, vm->pc_next);
        vm->pc_next = vm->pc + (uint32_t)imm_j;
}

void exec_addi(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        vm_reg_write(vm, rd, vm_reg_read(vm, rs1) + (uint32_t)imm_i);
}

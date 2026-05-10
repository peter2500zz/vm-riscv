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

        vm_reg_write(vm, rd, vm_pc_read(vm) + (uint32_t)imm_u);
}

void exec_jal(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        int32_t imm_j = inst_imm_j(inst);

        vm_reg_write(vm, rd, vm->pc_next);
        vm->pc_next = vm_pc_read(vm) + (uint32_t)imm_j;
}

void exec_jalr(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        uint32_t target = (vm_reg_read(vm, rs1) + (uint32_t)imm_i) & ~1u;
        vm_reg_write(vm, rd, vm->pc_next);
        vm->pc_next = target;
}

void exec_beq(VM *vm, Instruction inst) {
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);
        int32_t imm_b = inst_imm_b(inst);

        if (vm_reg_read(vm, rs1) == vm_reg_read(vm, rs2)) {
                vm->pc_next = vm_pc_read(vm) + (uint32_t)imm_b;
        }
}

void exec_bne(VM *vm, Instruction inst) {
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);
        int32_t imm_b = inst_imm_b(inst);

        if (vm_reg_read(vm, rs1) != vm_reg_read(vm, rs2)) {
                vm->pc_next = vm_pc_read(vm) + (uint32_t)imm_b;
        }
}

void exec_blt(VM *vm, Instruction inst) {
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);
        int32_t imm_b = inst_imm_b(inst);

        if ((int32_t)vm_reg_read(vm, rs1) < (int32_t)vm_reg_read(vm, rs2)) {
                vm->pc_next = vm_pc_read(vm) + (uint32_t)imm_b;
        }
}

void exec_bltu(VM *vm, Instruction inst) {
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);
        int32_t imm_b = inst_imm_b(inst);

        if (vm_reg_read(vm, rs1) < vm_reg_read(vm, rs2)) {
                vm->pc_next = vm_pc_read(vm) + (uint32_t)imm_b;
        }
}

void exec_bge(VM *vm, Instruction inst) {
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);
        int32_t imm_b = inst_imm_b(inst);

        if ((int32_t)vm_reg_read(vm, rs1) >= (int32_t)vm_reg_read(vm, rs2)) {
                vm->pc_next = vm_pc_read(vm) + (uint32_t)imm_b;
        }
}

void exec_bgeu(VM *vm, Instruction inst) {
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);
        int32_t imm_b = inst_imm_b(inst);

        if (vm_reg_read(vm, rs1) >= vm_reg_read(vm, rs2)) {
                vm->pc_next = vm_pc_read(vm) + (uint32_t)imm_b;
        }
}

void exec_lb(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        uint32_t addr = (uint32_t)((int32_t)vm_reg_read(vm, rs1) + imm_i);

        vm_reg_write(vm, rd, (uint32_t)(int8_t)*vm_mem_ptr_byte(vm, addr));
}

void exec_lh(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        uint32_t addr = (uint32_t)((int32_t)vm_reg_read(vm, rs1) + imm_i);

        vm_reg_write(vm, rd, (uint32_t)(int16_t)*vm_mem_ptr_half(vm, addr));
}

void exec_lw(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        uint32_t addr = (uint32_t)((int32_t)vm_reg_read(vm, rs1) + imm_i);

        vm_reg_write(vm, rd, *vm_mem_ptr_word(vm, addr));
}

void exec_lbu(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        uint32_t addr = (uint32_t)((int32_t)vm_reg_read(vm, rs1) + imm_i);

        vm_reg_write(vm, rd, (uint32_t)*vm_mem_ptr_byte(vm, addr));
}

void exec_lhu(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        uint32_t addr = (uint32_t)((int32_t)vm_reg_read(vm, rs1) + imm_i);

        vm_reg_write(vm, rd, (uint32_t)*vm_mem_ptr_half(vm, addr));
}

void exec_addi(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        vm_reg_write(vm, rd, vm_reg_read(vm, rs1) + (uint32_t)imm_i);
}

#include "exec.h"

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

void exec_sb(VM *vm, Instruction inst) {
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);
        int32_t imm_s = inst_imm_s(inst);

        uint32_t addr = (uint32_t)((int32_t)vm_reg_read(vm, rs1) + imm_s);

        *vm_mem_ptr_byte(vm, addr) = (uint8_t)vm_reg_read(vm, rs2);
}

void exec_sh(VM *vm, Instruction inst) {
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);
        int32_t imm_s = inst_imm_s(inst);

        uint32_t addr = (uint32_t)((int32_t)vm_reg_read(vm, rs1) + imm_s);

        *vm_mem_ptr_half(vm, addr) = (uint16_t)vm_reg_read(vm, rs2);
}

void exec_sw(VM *vm, Instruction inst) {
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);
        int32_t imm_s = inst_imm_s(inst);

        uint32_t addr = (uint32_t)((int32_t)vm_reg_read(vm, rs1) + imm_s);

        *vm_mem_ptr_word(vm, addr) = vm_reg_read(vm, rs2);
}

void exec_addi(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        vm_reg_write(vm, rd, vm_reg_read(vm, rs1) + (uint32_t)imm_i);
}

void exec_slti(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        uint32_t value = 0;

        if ((int32_t)vm_reg_read(vm, rs1) < imm_i) {
                value = 1;
        }

        vm_reg_write(vm, rd, value);
}

void exec_sltiu(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        uint32_t value = 0;

        if (vm_reg_read(vm, rs1) < (uint32_t)imm_i) {
                value = 1;
        }

        vm_reg_write(vm, rd, value);
}

void exec_andi(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        vm_reg_write(vm, rd, vm_reg_read(vm, rs1) & (uint32_t)imm_i);
}

void exec_ori(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        vm_reg_write(vm, rd, vm_reg_read(vm, rs1) | (uint32_t)imm_i);
}

void exec_xori(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        vm_reg_write(vm, rd, vm_reg_read(vm, rs1) ^ (uint32_t)imm_i);
}

void exec_slli(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        uint32_t offset = imm_i & 0x1F; // 0b11111

        vm_reg_write(vm, rd, vm_reg_read(vm, rs1) << offset);
}

void exec_srli(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        uint32_t offset = imm_i & 0x1F; // 0b11111

        vm_reg_write(vm, rd, vm_reg_read(vm, rs1) >> offset);
}

void exec_srai(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        int32_t offset = imm_i & 0x1F; // 0b11111

        vm_reg_write(vm, rd,
                     (uint32_t)((int32_t)vm_reg_read(vm, rs1) >> offset));
}

void exec_add(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        vm_reg_write(vm, rd, vm_reg_read(vm, rs1) + vm_reg_read(vm, rs2));
}

void exec_sub(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        vm_reg_write(vm, rd, vm_reg_read(vm, rs1) - vm_reg_read(vm, rs2));
}

void exec_sll(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        uint32_t offset = vm_reg_read(vm, rs2) & 0x1F; // 0b11111

        vm_reg_write(vm, rd, vm_reg_read(vm, rs1) << offset);
}

void exec_slt(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        uint32_t value = 0;

        if ((int32_t)vm_reg_read(vm, rs1) < (int32_t)vm_reg_read(vm, rs2)) {
                value = 1;
        }

        vm_reg_write(vm, rd, value);
}

void exec_sltu(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        uint32_t value = 0;

        if (vm_reg_read(vm, rs1) < vm_reg_read(vm, rs2)) {
                value = 1;
        }

        vm_reg_write(vm, rd, value);
}

void exec_xor(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        vm_reg_write(vm, rd, vm_reg_read(vm, rs1) ^ vm_reg_read(vm, rs2));
}

void exec_srl(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        uint32_t offset = vm_reg_read(vm, rs2) & 0x1F; // 0b11111

        vm_reg_write(vm, rd, vm_reg_read(vm, rs1) >> offset);
}

void exec_sra(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        uint32_t offset = vm_reg_read(vm, rs2) & 0x1F; // 0b11111

        vm_reg_write(vm, rd,
                     (uint32_t)((int32_t)vm_reg_read(vm, rs1) >> offset));
}

void exec_or(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        vm_reg_write(vm, rd, vm_reg_read(vm, rs1) | vm_reg_read(vm, rs2));
}

void exec_and(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        vm_reg_write(vm, rd, vm_reg_read(vm, rs1) & vm_reg_read(vm, rs2));
}

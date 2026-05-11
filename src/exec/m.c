
#include "../vm.h"
#include <stdint.h>

void exec_mul(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        vm_reg_write(vm, rd, vm_reg_read(vm, rs1) * vm_reg_read(vm, rs2));
}

void exec_mulh(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        vm_reg_write(vm, rd,
                     (uint32_t)(((int64_t)(int32_t)vm_reg_read(vm, rs1) *
                                 (int64_t)(int32_t)vm_reg_read(vm, rs2)) >>
                                32));
}

void exec_mulhsu(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        vm_reg_write(vm, rd,
                     (uint32_t)(((int64_t)(int32_t)vm_reg_read(vm, rs1) *
                                 (int64_t)vm_reg_read(vm, rs2)) >>
                                32));
}

void exec_mulhu(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        vm_reg_write(vm, rd,
                     (uint32_t)(((uint64_t)vm_reg_read(vm, rs1) *
                                 (uint64_t)vm_reg_read(vm, rs2)) >>
                                32));
}

void exec_div(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        int32_t a = (int32_t)vm_reg_read(vm, rs1);
        int32_t b = (int32_t)vm_reg_read(vm, rs2);
        if (b == 0) {
                vm_reg_write(vm, rd, (uint32_t)-1);
                return;
        }
        if (a == INT32_MIN && b == -1) {
                vm_reg_write(vm, rd, (uint32_t)INT32_MIN);
                return;
        }
        vm_reg_write(vm, rd, (uint32_t)(a / b));
}

void exec_divu(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        uint32_t left = vm_reg_read(vm, rs1);
        uint32_t right = vm_reg_read(vm, rs2);
        if (right == 0) {
                vm_reg_write(vm, rd, 0xFFFFFFFF);
                return;
        }
        vm_reg_write(vm, rd, left / right);
}

void exec_rem(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        int32_t left = (int32_t)vm_reg_read(vm, rs1);
        int32_t right = (int32_t)vm_reg_read(vm, rs2);
        if (right == 0) {
                vm_reg_write(vm, rd, (uint32_t)left);
                return;
        }
        if (left == INT32_MIN && right == -1) {
                vm_reg_write(vm, rd, 0);
                return;
        }
        vm_reg_write(vm, rd, (uint32_t)(left % right));
}

void exec_remu(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        uint32_t left = vm_reg_read(vm, rs1);
        uint32_t right = vm_reg_read(vm, rs2);
        if (right == 0) {
                vm_reg_write(vm, rd, left);
                return;
        }
        vm_reg_write(vm, rd, left % right);
}

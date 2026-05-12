#include "exec.h"

void exec_mul(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        hart_reg_write(hart, rd, hart_reg_read(hart, rs1) * hart_reg_read(hart, rs2));
}

void exec_mulh(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        hart_reg_write(hart, rd,
                     (uint32_t)(((int64_t)(int32_t)hart_reg_read(hart, rs1) *
                                 (int64_t)(int32_t)hart_reg_read(hart, rs2)) >>
                                32));
}

void exec_mulhsu(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        hart_reg_write(hart, rd,
                     (uint32_t)(((int64_t)(int32_t)hart_reg_read(hart, rs1) *
                                 (int64_t)hart_reg_read(hart, rs2)) >>
                                32));
}

void exec_mulhu(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        hart_reg_write(hart, rd,
                     (uint32_t)(((uint64_t)hart_reg_read(hart, rs1) *
                                 (uint64_t)hart_reg_read(hart, rs2)) >>
                                32));
}

void exec_div(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        int32_t a = (int32_t)hart_reg_read(hart, rs1);
        int32_t b = (int32_t)hart_reg_read(hart, rs2);
        if (b == 0) {
                hart_reg_write(hart, rd, (uint32_t)-1);
                return;
        }
        if (a == INT32_MIN && b == -1) {
                hart_reg_write(hart, rd, (uint32_t)INT32_MIN);
                return;
        }
        hart_reg_write(hart, rd, (uint32_t)(a / b));
}

void exec_divu(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        uint32_t left = hart_reg_read(hart, rs1);
        uint32_t right = hart_reg_read(hart, rs2);
        if (right == 0) {
                hart_reg_write(hart, rd, 0xFFFFFFFF);
                return;
        }
        hart_reg_write(hart, rd, left / right);
}

void exec_rem(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        int32_t left = (int32_t)hart_reg_read(hart, rs1);
        int32_t right = (int32_t)hart_reg_read(hart, rs2);
        if (right == 0) {
                hart_reg_write(hart, rd, (uint32_t)left);
                return;
        }
        if (left == INT32_MIN && right == -1) {
                hart_reg_write(hart, rd, 0);
                return;
        }
        hart_reg_write(hart, rd, (uint32_t)(left % right));
}

void exec_remu(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        uint32_t left = hart_reg_read(hart, rs1);
        uint32_t right = hart_reg_read(hart, rs2);
        if (right == 0) {
                hart_reg_write(hart, rd, left);
                return;
        }
        hart_reg_write(hart, rd, left % right);
}

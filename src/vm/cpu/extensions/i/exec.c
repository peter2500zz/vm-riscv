#include "exec.h"
#include "../../../memory/access.h"

void exec_lui(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        int32_t imm_u = inst_imm_u(inst);

        hart_reg_write(hart, rd, (uint32_t)imm_u);
}

void exec_auipc(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        int32_t imm_u = inst_imm_u(inst);

        hart_reg_write(hart, rd, hart_pc_read(hart) + (uint32_t)imm_u);
}

void exec_jal(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        int32_t imm_j = inst_imm_j(inst);

        hart_reg_write(hart, rd, hart->pc_next);
        hart->pc_next = hart_pc_read(hart) + (uint32_t)imm_j;
}

void exec_jalr(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        uint32_t target = (hart_reg_read(hart, rs1) + (uint32_t)imm_i) & ~1u;
        hart_reg_write(hart, rd, hart->pc_next);
        hart->pc_next = target;
}

void exec_beq(Hart *hart, Instruction inst) {
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);
        int32_t imm_b = inst_imm_b(inst);

        if (hart_reg_read(hart, rs1) == hart_reg_read(hart, rs2)) {
                hart->pc_next = hart_pc_read(hart) + (uint32_t)imm_b;
        }
}

void exec_bne(Hart *hart, Instruction inst) {
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);
        int32_t imm_b = inst_imm_b(inst);

        if (hart_reg_read(hart, rs1) != hart_reg_read(hart, rs2)) {
                hart->pc_next = hart_pc_read(hart) + (uint32_t)imm_b;
        }
}

void exec_blt(Hart *hart, Instruction inst) {
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);
        int32_t imm_b = inst_imm_b(inst);

        if ((int32_t)hart_reg_read(hart, rs1) <
            (int32_t)hart_reg_read(hart, rs2)) {
                hart->pc_next = hart_pc_read(hart) + (uint32_t)imm_b;
        }
}

void exec_bltu(Hart *hart, Instruction inst) {
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);
        int32_t imm_b = inst_imm_b(inst);

        if (hart_reg_read(hart, rs1) < hart_reg_read(hart, rs2)) {
                hart->pc_next = hart_pc_read(hart) + (uint32_t)imm_b;
        }
}

void exec_bge(Hart *hart, Instruction inst) {
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);
        int32_t imm_b = inst_imm_b(inst);

        if ((int32_t)hart_reg_read(hart, rs1) >=
            (int32_t)hart_reg_read(hart, rs2)) {
                hart->pc_next = hart_pc_read(hart) + (uint32_t)imm_b;
        }
}

void exec_bgeu(Hart *hart, Instruction inst) {
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);
        int32_t imm_b = inst_imm_b(inst);

        if (hart_reg_read(hart, rs1) >= hart_reg_read(hart, rs2)) {
                hart->pc_next = hart_pc_read(hart) + (uint32_t)imm_b;
        }
}

void exec_lb(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        uint32_t addr = (uint32_t)((int32_t)hart_reg_read(hart, rs1) + imm_i);

        int8_t value;
        MEM_ACCESS(hart, addr, &value, sizeof(int8_t), MEM_READ);

        hart_reg_write(hart, rd, (uint32_t)value);
}

void exec_lh(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        uint32_t addr = (uint32_t)((int32_t)hart_reg_read(hart, rs1) + imm_i);

        int16_t value;
        MEM_ACCESS(hart, addr, &value, sizeof(int16_t), MEM_READ);

        hart_reg_write(hart, rd, (uint32_t)value);
}

void exec_lw(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        uint32_t addr = (uint32_t)((int32_t)hart_reg_read(hart, rs1) + imm_i);

        int32_t value;
        MEM_ACCESS(hart, addr, &value, sizeof(int32_t), MEM_READ);

        hart_reg_write(hart, rd, (uint32_t)value);
}

void exec_lbu(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        uint32_t addr = (uint32_t)((int32_t)hart_reg_read(hart, rs1) + imm_i);

        uint8_t value;
        MEM_ACCESS(hart, addr, &value, sizeof(uint8_t), MEM_READ);

        hart_reg_write(hart, rd, (uint32_t)value);
}

void exec_lhu(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        uint32_t addr = (uint32_t)((int32_t)hart_reg_read(hart, rs1) + imm_i);

        uint16_t value;
        MEM_ACCESS(hart, addr, &value, sizeof(uint16_t), MEM_READ);

        hart_reg_write(hart, rd, (uint32_t)value);
}

void exec_sb(Hart *hart, Instruction inst) {
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);
        int32_t imm_s = inst_imm_s(inst);

        uint32_t addr = (uint32_t)((int32_t)hart_reg_read(hart, rs1) + imm_s);

        uint8_t value = (uint8_t)hart_reg_read(hart, rs2);
        MEM_ACCESS(hart, addr, &value, sizeof(uint8_t), MEM_WRITE);
}

void exec_sh(Hart *hart, Instruction inst) {
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);
        int32_t imm_s = inst_imm_s(inst);

        uint32_t addr = (uint32_t)((int32_t)hart_reg_read(hart, rs1) + imm_s);

        uint16_t value = (uint16_t)hart_reg_read(hart, rs2);
        MEM_ACCESS(hart, addr, &value, sizeof(uint16_t), MEM_WRITE);
}

void exec_sw(Hart *hart, Instruction inst) {
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);
        int32_t imm_s = inst_imm_s(inst);

        uint32_t addr = (uint32_t)((int32_t)hart_reg_read(hart, rs1) + imm_s);

        uint32_t value = hart_reg_read(hart, rs2);
        MEM_ACCESS(hart, addr, &value, sizeof(uint32_t), MEM_WRITE);
}

void exec_addi(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        hart_reg_write(hart, rd, hart_reg_read(hart, rs1) + (uint32_t)imm_i);
}

void exec_slti(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        uint32_t value = 0;

        if ((int32_t)hart_reg_read(hart, rs1) < imm_i) {
                value = 1;
        }

        hart_reg_write(hart, rd, value);
}

void exec_sltiu(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        uint32_t value = 0;

        if (hart_reg_read(hart, rs1) < (uint32_t)imm_i) {
                value = 1;
        }

        hart_reg_write(hart, rd, value);
}

void exec_andi(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        hart_reg_write(hart, rd, hart_reg_read(hart, rs1) & (uint32_t)imm_i);
}

void exec_ori(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        hart_reg_write(hart, rd, hart_reg_read(hart, rs1) | (uint32_t)imm_i);
}

void exec_xori(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        hart_reg_write(hart, rd, hart_reg_read(hart, rs1) ^ (uint32_t)imm_i);
}

void exec_slli(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        uint32_t offset = imm_i & 0x1F; // 0b11111

        hart_reg_write(hart, rd, hart_reg_read(hart, rs1) << offset);
}

void exec_srli(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        uint32_t offset = imm_i & 0x1F; // 0b11111

        hart_reg_write(hart, rd, hart_reg_read(hart, rs1) >> offset);
}

void exec_srai(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        int32_t offset = imm_i & 0x1F; // 0b11111

        hart_reg_write(hart, rd,
                       (uint32_t)((int32_t)hart_reg_read(hart, rs1) >> offset));
}

void exec_add(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        hart_reg_write(hart, rd,
                       hart_reg_read(hart, rs1) + hart_reg_read(hart, rs2));
}

void exec_sub(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        hart_reg_write(hart, rd,
                       hart_reg_read(hart, rs1) - hart_reg_read(hart, rs2));
}

void exec_sll(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        uint32_t offset = hart_reg_read(hart, rs2) & 0x1F; // 0b11111

        hart_reg_write(hart, rd, hart_reg_read(hart, rs1) << offset);
}

void exec_slt(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        uint32_t value = 0;

        if ((int32_t)hart_reg_read(hart, rs1) <
            (int32_t)hart_reg_read(hart, rs2)) {
                value = 1;
        }

        hart_reg_write(hart, rd, value);
}

void exec_sltu(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        uint32_t value = 0;

        if (hart_reg_read(hart, rs1) < hart_reg_read(hart, rs2)) {
                value = 1;
        }

        hart_reg_write(hart, rd, value);
}

void exec_xor(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        hart_reg_write(hart, rd,
                       hart_reg_read(hart, rs1) ^ hart_reg_read(hart, rs2));
}

void exec_srl(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        uint32_t offset = hart_reg_read(hart, rs2) & 0x1F; // 0b11111

        hart_reg_write(hart, rd, hart_reg_read(hart, rs1) >> offset);
}

void exec_sra(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        uint32_t offset = hart_reg_read(hart, rs2) & 0x1F; // 0b11111

        hart_reg_write(hart, rd,
                       (uint32_t)((int32_t)hart_reg_read(hart, rs1) >> offset));
}

void exec_or(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        hart_reg_write(hart, rd,
                       hart_reg_read(hart, rs1) | hart_reg_read(hart, rs2));
}

void exec_and(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t rs2 = inst_rs2(inst);

        hart_reg_write(hart, rd,
                       hart_reg_read(hart, rs1) & hart_reg_read(hart, rs2));
}

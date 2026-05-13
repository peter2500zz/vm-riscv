#include "exec.h"
#include "../../../cpu/hart/unprivileged/types.h"
#include "../../../memory/access.h"
#include <stdint.h>
#include <stdio.h>

void exec_clwsp(Hart *hart, CInstruction inst) {
        uint32_t rd = cinst_r_11_7(inst);
        uint32_t imm =
            BIT(inst, 12) << 5 | BITS(inst, 6, 4) << 2 | BITS(inst, 3, 2) << 6;

        uint32_t value;
        MEM_ACCESS(hart, hart_reg_read(hart, 2) + imm, &value, sizeof(uint32_t),
                   MEM_READ);
        hart_reg_write(hart, rd, value);
}

void exec_cswsp(Hart *hart, CInstruction inst) {
        uint32_t rs2 = cinst_r_6_2(inst);
        uint32_t imm = BITS(inst, 12, 9) << 2 | BITS(inst, 8, 7) << 6;

        uint32_t value = hart_reg_read(hart, rs2);
        MEM_ACCESS(hart, hart_reg_read(hart, 2) + imm, &value, sizeof(uint32_t),
                   MEM_WRITE);
}

void exec_clw(Hart *hart, CInstruction inst) {
        uint32_t rd = cinst_r_4_2(inst);
        uint32_t rs1 = cinst_r_9_7(inst);
        uint32_t imm =
            BITS(inst, 12, 10) << 3 | BIT(inst, 6) << 2 | BIT(inst, 5) << 6;

        uint32_t value;
        MEM_ACCESS(hart, hart_reg_read(hart, rs1) + imm, &value,
                   sizeof(uint32_t), MEM_READ);
        hart_reg_write(hart, rd, value);
}

void exec_csw(Hart *hart, CInstruction inst) {
        uint32_t rs2 = cinst_r_4_2(inst);
        uint32_t rs1 = cinst_r_9_7(inst);
        uint32_t imm =
            BITS(inst, 12, 10) << 3 | BIT(inst, 6) << 2 | BIT(inst, 5) << 6;

        uint32_t value = hart_reg_read(hart, rs2);
        MEM_ACCESS(hart, hart_reg_read(hart, rs1) + imm, &value,
                   sizeof(uint32_t), MEM_WRITE);
}

void exec_cj(Hart *hart, CInstruction inst) {
        int32_t imm = sign_extend(
            BIT(inst, 12) << 11 | BIT(inst, 11) << 4 | BITS(inst, 10, 9) << 8 |
                BIT(inst, 8) << 10 | BIT(inst, 7) << 6 | BIT(inst, 6) << 7 |
                BITS(inst, 5, 3) << 1 | BIT(inst, 2) << 5,
            12);

        hart->pc_next = hart_pc_read(hart) + (uint32_t)imm;
}

void exec_cjal(Hart *hart, CInstruction inst) {
        int32_t imm = sign_extend(
            BIT(inst, 12) << 11 | BIT(inst, 11) << 4 | BITS(inst, 10, 9) << 8 |
                BIT(inst, 8) << 10 | BIT(inst, 7) << 6 | BIT(inst, 6) << 7 |
                BITS(inst, 5, 3) << 1 | BIT(inst, 2) << 5,
            12);

        uint32_t return_address = hart_pc_read(hart) + 2;
        hart_reg_write(hart, 1, return_address);
        hart->pc_next = hart_pc_read(hart) + (uint32_t)imm;
}

void exec_cjr(Hart *hart, CInstruction inst) {
        uint32_t rs1 = cinst_r_11_7(inst);

        hart->pc_next = hart_reg_read(hart, rs1) & ~1u;
}

void exec_cjalr(Hart *hart, CInstruction inst) {
        uint32_t rs1 = cinst_r_11_7(inst);

        if (rs1 == 0) {
                // C.JALR with rs1 = x0 is C.EBREAK
                printf("EBREAK instruction\n");
                return;
        }

        uint32_t return_address = hart_pc_read(hart) + 2;
        hart_reg_write(hart, 1, return_address);
        hart->pc_next = hart_reg_read(hart, rs1) & ~1u;
}

void exec_cbeqz(Hart *hart, CInstruction inst) {
        uint32_t rs1 = cinst_r_9_7(inst);
        int32_t imm = sign_extend(BIT(inst, 12) << 8 | BITS(inst, 11, 10) << 3 |
                                      BITS(inst, 6, 5) << 6 |
                                      BITS(inst, 4, 3) << 1 | BIT(inst, 2) << 5,
                                  9);

        if (hart_reg_read(hart, rs1) == 0) {
                hart->pc_next = hart_pc_read(hart) + (uint32_t)imm;
        }
}

void exec_cbnez(Hart *hart, CInstruction inst) {
        uint32_t rs1 = cinst_r_9_7(inst);
        int32_t imm = sign_extend(BIT(inst, 12) << 8 | BITS(inst, 11, 10) << 3 |
                                      BITS(inst, 6, 5) << 6 |
                                      BITS(inst, 4, 3) << 1 | BIT(inst, 2) << 5,
                                  9);

        if (hart_reg_read(hart, rs1) != 0) {
                hart->pc_next = hart_pc_read(hart) + (uint32_t)imm;
        }
}

void exec_cli(Hart *hart, CInstruction inst) {
        uint32_t rd = cinst_r_11_7(inst);
        int32_t imm = sign_extend(BIT(inst, 12) << 5 | BITS(inst, 6, 2), 6);

        if (rd != 0) {
                hart_reg_write(hart, rd, (uint32_t)imm);
        }
}

void exec_clui(Hart *hart, CInstruction inst) {
        uint32_t rd = cinst_r_11_7(inst);
        int32_t imm =
            sign_extend(BIT(inst, 12) << 17 | BITS(inst, 6, 2) << 12, 18);

        if (rd == 0) {
                // C.LUI with rd=0 is HINT
                return;
        } else if (rd == 2) {
                // C.LUI with rd=x2 is C.ADDI16SP
                // exec_caddi16sp(hart, inst);
                return;
        } else if (imm == 0) {
                // C.LUI with imm=0 is reserved, and must not write to rd
                return;
        }
        hart_reg_write(hart, rd, (uint32_t)imm);
}

void exec_caddi(Hart *hart, CInstruction inst) {
        uint32_t rd = cinst_r_11_7(inst);
        int32_t imm = sign_extend(BIT(inst, 12) << 5 | BITS(inst, 6, 2), 6);

        hart_reg_write(hart, rd, hart_reg_read(hart, rd) + (uint32_t)imm);
}

void exec_caddi16sp(Hart *hart, CInstruction inst) {
        int32_t imm = sign_extend(BIT(inst, 12) << 9 | BIT(inst, 6) << 4 |
                                      BIT(inst, 5) << 6 |
                                      BITS(inst, 4, 3) << 7 | BIT(inst, 2) << 5,
                                  10);

        if (imm == 0) {
                // C.ADDI4SPN with imm=0 is reserved, and must not write to rd
                hart->trap_pending = 1;
                return;
        }

        hart_reg_write(hart, 2, hart_reg_read(hart, 2) + (uint32_t)imm);
}

void exec_caddi4spn(Hart *hart, CInstruction inst) {
        uint32_t rd = cinst_r_4_2(inst);
        uint32_t imm = (BITS(inst, 12, 11) << 4 | BITS(inst, 10, 7) << 6 |
                        BIT(inst, 6) << 2 | BIT(inst, 5) << 3);

        if (imm == 0) {
                // C.ADDI4SPN with imm=0 is reserved, and must not write to rd
                hart->trap_pending = 1;
                return;
        }

        hart_reg_write(hart, rd, hart_reg_read(hart, 2) + imm);
}

void exec_cslli(Hart *hart, CInstruction inst) {
        uint32_t rd = cinst_r_11_7(inst);
        uint32_t shamt = BIT(inst, 12) << 5 | BITS(inst, 6, 2);

        if (shamt & 0x20) {
                // For RV32, shamt[5] must be zero
                return;
        }

        hart_reg_write(hart, rd, hart_reg_read(hart, rd) << shamt);
}

void exec_csrli(Hart *hart, CInstruction inst) {
        uint32_t rd = cinst_r_9_7(inst);
        uint32_t shamt = BIT(inst, 12) << 5 | BITS(inst, 6, 2);

        if (shamt & 0x20) {
                // For RV32, shamt[5] must be zero
                return;
        }

        hart_reg_write(hart, rd, hart_reg_read(hart, rd) >> shamt);
}

void exec_csrai(Hart *hart, CInstruction inst) {
        uint32_t rd = cinst_r_9_7(inst);
        uint32_t shamt = BIT(inst, 12) << 5 | BITS(inst, 6, 2);

        if (shamt & 0x20) {
                // For RV32, shamt[5] must be zero
                return;
        }

        hart_reg_write(
            hart, rd,
            (uint32_t)((int32_t)hart_reg_read(hart, rd) >> (int32_t)shamt));
}

void exec_candi(Hart *hart, CInstruction inst) {
        uint32_t rd = cinst_r_9_7(inst);
        int32_t imm = sign_extend(BIT(inst, 12) << 5 | BITS(inst, 6, 2), 6);

        hart_reg_write(hart, rd, hart_reg_read(hart, rd) & (uint32_t)imm);
}

void exec_cmv(Hart *hart, CInstruction inst) {
        uint32_t rs2 = cinst_r_6_2(inst);
        uint32_t rd = cinst_r_11_7(inst);

        // if (hart_reg_read(hart, rs2) != 0) {
        hart_reg_write(hart, rd, hart_reg_read(hart, rs2));
        // } else {
        //         exec_cjr(hart, inst);
        // }
}

void exec_cadd(Hart *hart, CInstruction inst) {
        uint32_t rs2 = cinst_r_6_2(inst);
        uint32_t rd = cinst_r_11_7(inst);

        // if (rs2 == 0) {
        //         exec_cjalr(hart, inst);
        // } else {
        hart_reg_write(hart, rd,
                       hart_reg_read(hart, rd) + hart_reg_read(hart, rs2));
        // }
}

void exec_cand(Hart *hart, CInstruction inst) {
        uint32_t rs2 = cinst_r_4_2(inst);
        uint32_t rd = cinst_r_9_7(inst);

        hart_reg_write(hart, rd,
                       hart_reg_read(hart, rd) & hart_reg_read(hart, rs2));
}

void exec_cor(Hart *hart, CInstruction inst) {
        uint32_t rs2 = cinst_r_4_2(inst);
        uint32_t rd = cinst_r_9_7(inst);

        hart_reg_write(hart, rd,
                       hart_reg_read(hart, rd) | hart_reg_read(hart, rs2));
}

void exec_cxor(Hart *hart, CInstruction inst) {
        uint32_t rs2 = cinst_r_4_2(inst);
        uint32_t rd = cinst_r_9_7(inst);

        hart_reg_write(hart, rd,
                       hart_reg_read(hart, rd) ^ hart_reg_read(hart, rs2));
}

void exec_csub(Hart *hart, CInstruction inst) {
        uint32_t rs2 = cinst_r_4_2(inst);
        uint32_t rd = cinst_r_9_7(inst);

        hart_reg_write(hart, rd,
                       hart_reg_read(hart, rd) - hart_reg_read(hart, rs2));
}

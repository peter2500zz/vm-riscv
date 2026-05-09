#include <stdint.h>
#include <stdlib.h>

#include "instruction.h"

int32_t sign_extend(uint32_t value, int bits) {
        int shift = 32 - bits;
        return (int32_t)(value << shift) >> shift;
}

uint32_t inst_opcode(Instruction inst) { return BITS(inst, 6, 0); }

uint32_t inst_rd(Instruction inst) { return BITS(inst, 11, 7); }

uint32_t inst_funct3(Instruction inst) { return BITS(inst, 14, 12); }

uint32_t inst_rs1(Instruction inst) { return BITS(inst, 19, 15); }

uint32_t inst_rs2(Instruction inst) { return BITS(inst, 24, 20); }

uint32_t inst_funct7(Instruction inst) { return BITS(inst, 31, 25); }

int32_t inst_imm_i(Instruction inst) {
        return sign_extend(BITS(inst, 31, 20), 12);
}

int32_t inst_imm_s(Instruction inst) {
        return sign_extend(BITS(inst, 31, 25) << 5 | BITS(inst, 11, 7), 12);
}

int32_t inst_imm_b(Instruction inst) {
        return sign_extend(BIT(inst, 31) << 12 | BIT(inst, 7) << 11 |
                               BITS(inst, 30, 25) << 5 | BITS(inst, 11, 8) << 1,
                           13);
}

int32_t inst_imm_u(Instruction inst) {
        return (int32_t)(BITS(inst, 31, 12) << 12);
}

int32_t inst_imm_j(Instruction inst) {
        return sign_extend(BIT(inst, 31) << 20 | BITS(inst, 19, 12) << 12 |
                               BIT(inst, 20) << 11 | BITS(inst, 30, 21) << 1,
                           21);
}

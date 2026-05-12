#include "root.h"
// i 模块指令
#include "../extensions/i/exec.h"
// m 模块指令
#include "../extensions/m/exec.h"
// zicsr 模块指令
#include "../extensions/zicsr/exec.h"
// ecall 指令
#include "../extensions/ecall/exec.h"

int hart_dispatch(Hart *hart, Instruction inst) {
        hart->pc_next = hart_pc_read(hart) + 4;

        uint32_t opcode = inst_opcode(inst);
        uint32_t rd = inst_rd(inst);
        uint32_t funct3 = inst_funct3(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t funct7 = inst_funct7(inst);
        int32_t imm_i = inst_imm_i(inst);

        switch (opcode) {
        // U LUI
        case 0x37: // 0b0110111
                exec_lui(hart, inst);

                goto done;
        // U AUIPC
        case 0x17: // 0b0010111
                exec_auipc(hart, inst);

                goto done;
        // J JAL
        case 0x6f: // 0b1101111
                exec_jal(hart, inst);

                goto done;
        // I JALR
        case 0x67: // 0b1100111
                exec_jalr(hart, inst);

                goto done;
        // B 6
        case 0x63: // 0b1100011
                switch (funct3) {
                // BEQ
                case 0x0: // 0b000
                        exec_beq(hart, inst);

                        goto done;
                // BNE
                case 0x1: // 0b001
                        exec_bne(hart, inst);

                        goto done;
                // BLT
                case 0x4: // 0b100
                        exec_blt(hart, inst);

                        goto done;
                // BGE
                case 0x5: // 0b101
                        exec_bge(hart, inst);

                        goto done;
                // BLTU
                case 0x6: // 0b110
                        exec_bltu(hart, inst);

                        goto done;
                // BGEU
                case 0x7: // 0b111
                        exec_bgeu(hart, inst);

                        goto done;
                }

                break;
        // I 5
        case 0x3: // 0b0000011
                switch (funct3) {
                // LB
                case 0x0: // 0b000
                        exec_lb(hart, inst);

                        goto done;
                // LH
                case 0x1: // 0b001
                        exec_lh(hart, inst);

                        goto done;
                // LW
                case 0x2: // 0b010
                        exec_lw(hart, inst);

                        goto done;
                // LBU
                case 0x4: // 0b100
                        exec_lbu(hart, inst);

                        goto done;
                // LHU
                case 0x5: // 0b101
                        exec_lhu(hart, inst);

                        goto done;
                }

                break;
        // S 3
        case 0x23: // 0b0100011
                switch (funct3) {
                // SB
                case 0x0: // 0b000
                        exec_sb(hart, inst);

                        goto done;
                // SH
                case 0x1: // 0b001
                        exec_sh(hart, inst);

                        goto done;
                // SW
                case 0x2: // 0b010
                        exec_sw(hart, inst);

                        goto done;
                }

                break;
        // I+R 9
        case 0x13: // 0b0010011
                switch (funct3) {
                // I 6
                // ADDI
                case 0x0: // 0b000
                        exec_addi(hart, inst);

                        goto done;
                // SLTI
                case 0x2: // 0b010
                        exec_slti(hart, inst);

                        goto done;
                // SLTIU
                case 0x3: // 0b011
                        exec_sltiu(hart, inst);

                        goto done;
                // XORI
                case 0x4: // 0b100
                        exec_xori(hart, inst);

                        goto done;
                // ORI
                case 0x6: // 0b110
                        exec_ori(hart, inst);

                        goto done;
                // ANDI
                case 0x7: // 0b111
                        exec_andi(hart, inst);

                        goto done;
                // R 3
                case 0x1: // 0b001
                        switch (funct7) {
                        // SLLI
                        case 0x0: // 0b0000000
                                exec_slli(hart, inst);

                                goto done;
                        }

                        break;
                case 0x5: // 0b101
                        switch (funct7) {
                        // SRLI
                        case 0x0: // 0b0000000
                                exec_srli(hart, inst);

                                goto done;
                        // SRAI
                        case 0x20: // 0b0100000
                                exec_srai(hart, inst);

                                goto done;
                        }

                        break;
                }

                break;
        // R 10
        case 0x33: // 0b0110011
                switch (funct3) {
                case 0x00: // 0b000
                        switch (funct7) {
                        // ADD
                        case 0x00: // 0b0000000
                                exec_add(hart, inst);

                                goto done;
                        // MUL
                        case 0x01: // 0b0000001
                                exec_mul(hart, inst);

                                goto done;
                        // SUB
                        case 0x20: // 0b0100000
                                exec_sub(hart, inst);

                                goto done;
                        }

                        break;
                case 0x01: // 0b001
                        switch (funct7) {
                        // SLL
                        case 0x00: // 0b0000000
                                exec_sll(hart, inst);

                                goto done;
                        // MULH
                        case 0x01: // 0b0000001
                                exec_mulh(hart, inst);

                                goto done;
                        }

                        break;
                case 0x02: // 0b010
                        switch (funct7) {
                        // SLT
                        case 0x00: // 0b0000000
                                exec_slt(hart, inst);

                                goto done;
                        // MULHSU
                        case 0x01: // 0b0000001
                                exec_mulhsu(hart, inst);

                                goto done;
                        }

                        break;
                case 0x03: // 0b011
                        switch (funct7) {
                        // SLTU
                        case 0x00: // 0b0000000
                                exec_sltu(hart, inst);

                                goto done;
                        // MULHU
                        case 0x01: // 0b0000001
                                exec_mulhu(hart, inst);

                                goto done;
                        }

                        break;
                case 0x04: // 0b100
                        switch (funct7) {
                        // XOR
                        case 0x00: // 0b0000000
                                exec_xor(hart, inst);

                                goto done;
                        // DIV
                        case 0x01: // 0b0000001
                                exec_div(hart, inst);

                                goto done;
                        }

                        break;
                case 0x05: // 0b101
                        switch (funct7) {
                        // SRL
                        case 0x00: // 0b0000000
                                exec_srl(hart, inst);

                                goto done;
                        // DIVU
                        case 0x01: // 0b0000001
                                exec_divu(hart, inst);

                                goto done;
                        // SRA
                        case 0x20: // 0b0100000
                                exec_sra(hart, inst);

                                goto done;
                        }

                        break;
                case 0x06: // 0b110
                        switch (funct7) {
                        // OR
                        case 0x00: // 0b0000000
                                exec_or(hart, inst);

                                goto done;
                        // REM
                        case 0x01: // 0b0000001
                                exec_rem(hart, inst);

                                goto done;
                        }

                        break;
                case 0x07: // 0b111
                        switch (funct7) {
                        // AND
                        case 0x00: // 0b0000000
                                exec_and(hart, inst);

                                goto done;
                        // REMU
                        case 0x01: // 0b0000001
                                exec_remu(hart, inst);

                                goto done;
                        }

                        break;
                }

                break;
        // I 3
        case 0xf: // 0b0001111
                switch (funct3) {
                case 0x0: { // 0b000
                        uint32_t succ = BITS(inst, 23, 20);
                        uint32_t pred = BITS(inst, 27, 24);
                        uint32_t fm = BITS(inst, 31, 28);

                        // FENCE.TSO
                        if (rd == 0x0      // 0b00000
                            && rs1 == 0x0  // 0b00000
                            && succ == 0x3 // 0b0011
                            && pred == 0x3 // 0b0011
                            && fm == 0x8   // 0b1000
                        ) {

                                break;
                        }
                        // PAUSE
                        else if (rd == 0x0      // 0b00000
                                 && rs1 == 0x0  // 0b00000
                                 && succ == 0x0 // 0b0000
                                 && pred == 0x1 // 0b0001
                                 && fm == 0x0   // 0b0000
                        ) {

                                break;
                        }
                        // FENCE
                        else {

                                break;
                        }
                }
                }

                break;
        // I 2
        case 0x73: // 0b1110011
                switch (funct3) {
                case 0x00: // 0b000
                        // ECALL
                        if (rd == 0x0       // 0b00000
                            && rs1 == 0x0   // 0b00000
                            && imm_i == 0x0 // 0b000000000000
                        ) {
                                handle_ecall(hart);

                                goto done;
                        }
                        // EBREAK
                        else if (rd == 0x0       // 0b00000
                                 && rs1 == 0x0   // 0b00000
                                 && imm_i == 0x1 // 0b000000000001
                        ) {

                                break;
                        }

                        break;
                // CSRRW
                case 0x01: // 0b001
                        exec_csrrw(hart, inst);

                        goto done;
                // CSRRS
                case 0x02: // 0b010
                        exec_csrrs(hart, inst);

                        goto done;
                // CSRRC
                case 0x03: // 0b011
                        exec_csrrc(hart, inst);

                        goto done;
                // CSRRWI
                case 0x05: // 0b101
                        exec_csrrwi(hart, inst);

                        goto done;
                // CSRRSI
                case 0x06: // 0b110
                        exec_csrrsi(hart, inst);

                        goto done;
                // CSRRCI
                case 0x07: // 0b111
                        exec_csrrci(hart, inst);

                        goto done;
                }

                break;
        }

        return 1;

done:
        hart_pc_write(hart, hart->pc_next);

        return 0;
}

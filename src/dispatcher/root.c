#include "root.h"
// i 模块指令
#include "../modules/i/exec.h"
// m 模块指令
#include "../modules/m/exec.h"
// ecall 指令
#include "../modules/ecall/exec.h"

Instruction vm_fetch(VM *vm) {
        Instruction inst = (Instruction)(*vm_mem_ptr_word(vm, vm_pc_read(vm)));

        return inst;
}

int vm_dispatch(VM *vm, Instruction inst) {
        vm->pc_next = vm_pc_read(vm) + 4;

        uint32_t opcode = inst_opcode(inst);
        uint32_t rd = inst_rd(inst);
        uint32_t funct3 = inst_funct3(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t funct7 = inst_funct7(inst);
        int32_t imm_i = inst_imm_i(inst);

        switch (opcode) {
        // U LUI
        case 0x37: // 0b0110111
                exec_lui(vm, inst);

                goto done;
        // U AUIPC
        case 0x17: // 0b0010111
                exec_auipc(vm, inst);

                goto done;
        // J JAL
        case 0x6f: // 0b1101111
                exec_jal(vm, inst);

                goto done;
        // I JALR
        case 0x67: // 0b1100111
                exec_jalr(vm, inst);

                goto done;
        // B 6
        case 0x63: // 0b1100011
                switch (funct3) {
                // BEQ
                case 0x0: // 0b000
                        exec_beq(vm, inst);

                        goto done;
                // BNE
                case 0x1: // 0b001
                        exec_bne(vm, inst);

                        goto done;
                // BLT
                case 0x4: // 0b100
                        exec_blt(vm, inst);

                        goto done;
                // BGE
                case 0x5: // 0b101
                        exec_bge(vm, inst);

                        goto done;
                // BLTU
                case 0x6: // 0b110
                        exec_bltu(vm, inst);

                        goto done;
                // BGEU
                case 0x7: // 0b111
                        exec_bgeu(vm, inst);

                        goto done;
                }

                break;
        // I 5
        case 0x3: // 0b0000011
                switch (funct3) {
                // LB
                case 0x0: // 0b000
                        exec_lb(vm, inst);

                        goto done;
                // LH
                case 0x1: // 0b001
                        exec_lh(vm, inst);

                        goto done;
                // LW
                case 0x2: // 0b010
                        exec_lw(vm, inst);

                        goto done;
                // LBU
                case 0x4: // 0b100
                        exec_lbu(vm, inst);

                        goto done;
                // LHU
                case 0x5: // 0b101
                        exec_lhu(vm, inst);

                        goto done;
                }

                break;
        // S 3
        case 0x23: // 0b0100011
                switch (funct3) {
                // SB
                case 0x0: // 0b000
                        exec_sb(vm, inst);

                        goto done;
                // SH
                case 0x1: // 0b001
                        exec_sh(vm, inst);

                        goto done;
                // SW
                case 0x2: // 0b010
                        exec_sw(vm, inst);

                        goto done;
                }

                break;
        // I+R 9
        case 0x13: // 0b0010011
                switch (funct3) {
                // I 6
                // ADDI
                case 0x0: // 0b000
                        exec_addi(vm, inst);

                        goto done;
                // SLTI
                case 0x2: // 0b010
                        exec_slti(vm, inst);

                        goto done;
                // SLTIU
                case 0x3: // 0b011
                        exec_sltiu(vm, inst);

                        goto done;
                // XORI
                case 0x4: // 0b100
                        exec_xori(vm, inst);

                        goto done;
                // ORI
                case 0x6: // 0b110
                        exec_ori(vm, inst);

                        goto done;
                // ANDI
                case 0x7: // 0b111
                        exec_andi(vm, inst);

                        goto done;
                // R 3
                case 0x1: // 0b001
                        switch (funct7) {
                        // SLLI
                        case 0x0: // 0b0000000
                                exec_slli(vm, inst);

                                goto done;
                        }

                        break;
                case 0x5: // 0b101
                        switch (funct7) {
                        // SRLI
                        case 0x0: // 0b0000000
                                exec_srli(vm, inst);

                                goto done;
                        // SRAI
                        case 0x20: // 0b0100000
                                exec_srai(vm, inst);

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
                                exec_add(vm, inst);

                                goto done;
                        // MUL
                        case 0x01: // 0b0000001
                                exec_mul(vm, inst);

                                goto done;
                        // SUB
                        case 0x20: // 0b0100000
                                exec_sub(vm, inst);

                                goto done;
                        }

                        break;
                case 0x01: // 0b001
                        switch (funct7) {
                        // SLL
                        case 0x00: // 0b0000000
                                exec_sll(vm, inst);

                                goto done;
                        // MULH
                        case 0x01: // 0b0000001
                                exec_mulh(vm, inst);

                                goto done;
                        }

                        break;
                case 0x02: // 0b010
                        switch (funct7) {
                        // SLT
                        case 0x00: // 0b0000000
                                exec_slt(vm, inst);

                                goto done;
                        // MULHSU
                        case 0x01: // 0b0000001
                                exec_mulhsu(vm, inst);

                                goto done;
                        }

                        break;
                case 0x03: // 0b011
                        switch (funct7) {
                        // SLTU
                        case 0x00: // 0b0000000
                                exec_sltu(vm, inst);

                                goto done;
                        // MULHU
                        case 0x01: // 0b0000001
                                exec_mulhu(vm, inst);

                                goto done;
                        }

                        break;
                case 0x04: // 0b100
                        switch (funct7) {
                        // XOR
                        case 0x00: // 0b0000000
                                exec_xor(vm, inst);

                                goto done;
                        // DIV
                        case 0x01: // 0b0000001
                                exec_div(vm, inst);

                                goto done;
                        }

                        break;
                case 0x05: // 0b101
                        switch (funct7) {
                        // SRL
                        case 0x00: // 0b0000000
                                exec_srl(vm, inst);

                                goto done;
                        // DIVU
                        case 0x01: // 0b0000001
                                exec_divu(vm, inst);

                                goto done;
                        // SRA
                        case 0x20: // 0b0100000
                                exec_sra(vm, inst);

                                goto done;
                        }

                        break;
                case 0x06: // 0b110
                        switch (funct7) {
                        // OR
                        case 0x00: // 0b0000000
                                exec_or(vm, inst);

                                goto done;
                        // REM
                        case 0x01: // 0b0000001
                                exec_rem(vm, inst);

                                goto done;
                        }

                        break;
                case 0x07: // 0b111
                        switch (funct7) {
                        // AND
                        case 0x00: // 0b0000000
                                exec_and(vm, inst);

                                goto done;
                        // REMU
                        case 0x01: // 0b0000001
                                exec_remu(vm, inst);

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
                                handle_ecall(vm);

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
                        break;
                // CSRRS
                case 0x02: // 0b010
                        break;
                // CSRRC
                case 0x03: // 0b011
                        break;
                // CSRRWI
                case 0x05: // 0b101
                        break;
                // CSRRSI
                case 0x06: // 0b110
                        break;
                // CSRRCI
                case 0x07: // 0b111
                        break;
                }

                break;
        }

        return 1;

done:
        vm_pc_write(vm, vm->pc_next);

        return 0;
}

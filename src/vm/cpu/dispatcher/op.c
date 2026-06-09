#include "../extensions/i/exec.h"
#include "../extensions/m/exec.h"

#include "../hart/privileged.h"

void dispatch_op(Hart *hart, Instruction inst) {
        uint32_t funct3 = inst_funct3(inst);
        uint32_t funct7 = inst_funct7(inst);

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

        hart_trap_sync(hart, CAUSE_ILLEGAL_INSTRUCTION, inst);

done:
        return;
}

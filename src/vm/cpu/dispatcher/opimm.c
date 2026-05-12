#include "../extensions/i/exec.h"

void dispatch_opimm(Hart *hart, Instruction inst) {
        uint32_t funct3 = inst_funct3(inst);
        uint32_t funct7 = inst_funct7(inst);

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

        hart->trap_pending = 1;

done:
        return;
}

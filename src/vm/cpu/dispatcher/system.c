#include "system.h"
#include "../extensions/system/exec.h"
#include "../extensions/zicsr/exec.h"

#include "../hart/privileged.h"

void dispatch_system(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t funct3 = inst_funct3(inst);
        uint32_t rs1 = inst_rs1(inst);
        int32_t imm_i = inst_imm_i(inst);

        switch (funct3) {
        case 0x00: // 0b000
                // ECALL
                if (rd == 0x0       // 0b00000
                    && rs1 == 0x0   // 0b00000
                    && imm_i == 0x0 // 0b000000000000
                ) {
                        exec_ecall(hart);

                        goto done;
                }
                // EBREAK
                else if (rd == 0x0       // 0b00000
                         && rs1 == 0x0   // 0b00000
                         && imm_i == 0x1 // 0b000000000001
                ) {

                        break;
                }
                // MRET
                else if (rd == 0x0       // 0b00000
                         && rs1 == 0x0   // 0b00000
                         && imm_i == 0x302 // 0b0000001100000010
                ) {
                        exec_mret(hart);

                        goto done;
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

        hart_trap_sync(hart, CAUSE_ILLEGAL_INSTRUCTION, inst);

done:
        return;
}

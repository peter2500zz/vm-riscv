#include "branch.h"
#include "../extensions/i/exec.h"

void dispatch_branch(Hart *hart, Instruction inst) {
        uint32_t funct3 = inst_funct3(inst);

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

        hart->trap_pending = 1;

done:
        return;
}

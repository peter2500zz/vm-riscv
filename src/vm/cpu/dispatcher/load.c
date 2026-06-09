#include "load.h"
#include "../extensions/i/exec.h"

#include "../hart/privileged.h"

void dispatch_load(Hart *hart, Instruction inst) {
        uint32_t funct3 = inst_funct3(inst);

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

        hart_trap_sync(hart, CAUSE_ILLEGAL_INSTRUCTION, inst);

done:
        return;
}

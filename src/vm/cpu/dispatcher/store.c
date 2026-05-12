#include "store.h"
#include "../extensions/i/exec.h"

void dispatch_store(Hart *hart, Instruction inst) {
        uint32_t funct3 = inst_funct3(inst);

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

        hart->trap_pending = 1;

done:
        return;
}

// 根分发器
#include "root.h"
// branch 分发器
#include "branch.h"
// load 分发器
#include "load.h"
// store 分发器
#include "store.h"
// op 分发器
#include "op.h"
// opimm 分发器
#include "opimm.h"
// system 分发器
#include "system.h"
// compressed 分发器
#include "compressed.h"

// i 模块指令
#include "../extensions/i/exec.h"

#include "../hart/privileged.h"

int hart_dispatch(Hart *hart, Instruction inst) {
        hart->trap_pending = 0;
        hart->pc_next = hart_pc_read(hart) + 4;
        // hart_debug(hart);
        // printf("PC at 0x%08X: 0x%08X\n", hart_pc_read(hart), inst);
        // // wait for enter
        // while (getchar() != '\n')
        //         ;

        uint32_t opcode = inst_opcode(inst);

        if ((opcode & 0x3) != 0x3) {
                // C 扩展指令
                dispatch_compressed(hart, (CInstruction)inst);

                goto done;
        }

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
                dispatch_branch(hart, inst);

                goto done;
        // I 5
        case 0x3: // 0b0000011
                dispatch_load(hart, inst);

                goto done;
        // S 3
        case 0x23: // 0b0100011
                dispatch_store(hart, inst);

                goto done;
        // I+R 9
        case 0x13: // 0b0010011
                dispatch_opimm(hart, inst);

                goto done;
        // R 10
        case 0x33: // 0b0110011
                dispatch_op(hart, inst);

                goto done;
        // I 3
        case 0xf: // 0b0001111
                // switch (funct3) {
                // case 0x0: { // 0b000
                //         uint32_t succ = BITS(inst, 23, 20);
                //         uint32_t pred = BITS(inst, 27, 24);
                //         uint32_t fm = BITS(inst, 31, 28);

                //         // FENCE.TSO
                //         if (rd == 0x0      // 0b00000
                //             && rs1 == 0x0  // 0b00000
                //             && succ == 0x3 // 0b0011
                //             && pred == 0x3 // 0b0011
                //             && fm == 0x8   // 0b1000
                //         ) {

                //                 break;
                //         }
                //         // PAUSE
                //         else if (rd == 0x0      // 0b00000
                //                  && rs1 == 0x0  // 0b00000
                //                  && succ == 0x0 // 0b0000
                //                  && pred == 0x1 // 0b0001
                //                  && fm == 0x0   // 0b0000
                //         ) {

                //                 break;
                //         }
                //         // FENCE
                //         else {

                //                 break;
                //         }
                // }
                // }

                break;
        // I 2
        case 0x73: // 0b1110011
                dispatch_system(hart, inst);

                goto done;
        }

        hart_trap_sync(hart, CAUSE_ILLEGAL_INSTRUCTION, inst);

done:
        hart_pc_write(hart, hart->pc_next);

        return 0;
}

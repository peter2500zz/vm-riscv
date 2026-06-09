#include "compressed.h"
#include "../extensions/c/exec.h"
#include <stdint.h>
#include <stdio.h>

#include "../hart/privileged.h"

void dispatch_compressed(Hart *hart, CInstruction inst) {
        uint32_t opcode = cinst_opcode(inst);
        uint32_t funct2 = cinst_funct2(inst);
        uint32_t funct3 = cinst_funct3(inst);
        uint32_t funct4 = cinst_funct4(inst);
        uint32_t funct6 = cinst_funct6(inst);
        uint32_t r_11_7 = cinst_r_11_7(inst);
        uint32_t r_6_2 = cinst_r_6_2(inst);

        hart->pc_next = hart_pc_read(hart) + 2;

        // printf("Compressed instruction: opcode=0x%x, funct2=0x%x, funct3=0x%x, "
        //        "funct4=0x%x, funct6=0x%x, r_11_7=0x%x, r_6_2=0x%x\n",
        //        opcode, funct2, funct3, funct4, funct6, r_11_7, r_6_2);

        // 永久非法指令
        if (!inst) {
                printf("pure zero instruction\n");
                goto illegal_instruction;
        }

        switch (opcode) {
        // C 0
        case 0x0: // 0b00
                switch (funct3) {
                case 0x0: // 0b000
                        exec_caddi4spn(hart, inst);

                        goto done;
                case 0x2: // 0b010
                        exec_clw(hart, inst);

                        goto done;
                case 0x6: // 0b110
                        exec_csw(hart, inst);

                        goto done;
                }

                break;
        // C 1
        case 0x1: // 0b01
                switch (funct3) {
                case 0x0: // 0b000
                        exec_caddi(hart, inst);

                        goto done;
                case 0x1: // 0b001
                        exec_cjal(hart, inst);

                        goto done;
                case 0x2: // 0b010
                        exec_cli(hart, inst);

                        goto done;
                case 0x3: // 0b011
                        if (r_11_7 == 2) {
                                exec_caddi16sp(hart, inst);
                        } else {
                                exec_clui(hart, inst);
                        }

                        goto done;
                case 0x4: // 0b100
                        switch (funct6 & 0x3) {
                        case 0x0: // 0b00
                                exec_csrli(hart, inst);

                                goto done;
                        case 0x1: // 0b01
                                exec_csrai(hart, inst);

                                goto done;
                        case 0x2: // 0b10
                                exec_candi(hart, inst);

                                goto done;
                        case 0x3: // 0b11
                                switch (funct4) {
                                case 0x8: // 0b1000
                                        switch (funct2) {
                                        case 0x0: // 0b00
                                                exec_csub(hart, inst);

                                                goto done;
                                        case 0x1: // 0b01
                                                exec_cxor(hart, inst);

                                                goto done;
                                        case 0x2: // 0b10
                                                exec_cor(hart, inst);

                                                goto done;
                                        case 0x3: // 0b11
                                                exec_cand(hart, inst);

                                                goto done;
                                        }

                                        break;
                                }

                                break;
                        }

                        break;
                case 0x5: // 0b101
                        exec_cj(hart, inst);

                        goto done;
                case 0x6: // 0b110
                        exec_cbeqz(hart, inst);

                        goto done;
                case 0x7: // 0b111
                        exec_cbnez(hart, inst);

                        goto done;
                }

                break;
        // C 2
        case 0x2: // 0b10
                switch (funct3) {
                case 0x0: // 0b000
                        exec_cslli(hart, inst);

                        goto done;
                case 0x2: // 0b010
                        exec_clwsp(hart, inst);

                        goto done;
                case 0x4: { // 0b100
                        uint32_t sign = (uint32_t)(!!r_11_7 << 1) | (!!r_6_2);

                        switch (funct4) {
                        case 0x8: // 0b1000
                                switch (sign) {
                                case 0x2: // 0b10
                                        exec_cjr(hart, inst);

                                        goto done;
                                case 0x3: // 0b11
                                        exec_cmv(hart, inst);

                                        goto done;
                                }

                                break;
                        case 0x9: // 0b1001
                                switch (sign) {
                                case 0x0: // 0b00
                                        // exec_cebreak(hart, inst);

                                        // goto done;
                                        break;
                                case 0x2: // 0b10
                                        exec_cjalr(hart, inst);

                                        goto done;
                                case 0x3: // 0b11
                                        exec_cadd(hart, inst);

                                        goto done;
                                }

                                break;
                        }

                        break;
                }
                case 0x6: // 0b110
                        exec_cswsp(hart, inst);

                        goto done;
                }
        }

illegal_instruction:
        hart_trap_sync(hart, CAUSE_ILLEGAL_INSTRUCTION, inst);
done:
        return;
}

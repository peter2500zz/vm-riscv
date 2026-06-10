#include "privileged.h"
#include "privileged/types.h"
#include <stdint.h>

uint32_t hart_csr_read(Hart *hart, CSR csr) { return hart->_csr[csr.addr]; }

void hart_csr_write(Hart *hart, CSR csr, uint32_t value) {
        hart->_csr[csr.addr] = value;
}

void hart_trap_sync(Hart *hart, uint32_t cause, uint32_t tval) {
        hart->trap_pending = 1;

        // 1. 保存 PC
        // mepc 必须保存精确的陷入 PC。RV32 指令至少 2 字节对齐：只有 bit0
        // 恒为 0；开了 C 扩展后 bit1 可能为 1（指令落在 2 字节边界）。
        // 绝不能清 bit1，否则恢复时会把地址错位 2 字节，落进上一条指令中间。
        hart->_csr[CSR_MEPC] = hart_pc_read(hart) & ~(uint32_t)0x1;

        // 2. 记录原因
        hart->_csr[CSR_MCAUSE] = cause;
        hart->_csr[CSR_MTVAL] = tval;

        // 3. 更新 mstatus
        mstatus_t ms;
        ms.raw = hart->_csr[CSR_MSTATUS];
        ms.MPIE = ms.MIE;    // MPIE ← MIE
        ms.MIE = 0;          // 关中断
        ms.MPP = hart->priv; // MPP ← 当前特权级
        hart->_csr[CSR_MSTATUS] = ms.raw;

        // 4. 切换到 M-mode
        hart->priv = PRIV_M;

        // 5. 跳转到 handler
        uint32_t mtvec = hart->_csr[CSR_MTVEC];
        uint32_t base = mtvec & ~(uint32_t)0x3;
        hart->pc_next = base; // 同步异常始终跳 BASE

        if (cause == CAUSE_ILLEGAL_INSTRUCTION) {
                fprintf(stderr, "==== Unknown instruction ====\n");
                fprintf(stderr, "At 0x%08X\n", hart_pc_read(hart));
                fprintf(stderr, "Instruction: 0x%08X\n", tval);
                fprintf(stderr, "\topcode: 0x%02X\n", inst_opcode(tval));
                fprintf(stderr, "\tfunct3: 0x%02X\n", inst_funct3(tval));
                fprintf(stderr, "\tfunct7: 0x%02X\n", inst_funct7(tval));
                fprintf(stderr, "\trd: %d\n", inst_rd(tval));
                fprintf(stderr, "\trs1: %d\n", inst_rs1(tval));
                fprintf(stderr, "\trs2: %d\n", inst_rs2(tval));
                fprintf(stderr, "\timm_i: %d\n", inst_imm_i(tval));
                fprintf(stderr, "\timm_b: %d\n", inst_imm_b(tval));
                fprintf(stderr, "\timm_s: %d\n", inst_imm_s(tval));
                fprintf(stderr, "\timm_u: %d\n", inst_imm_u(tval));
                fprintf(stderr, "\timm_j: %d\n", inst_imm_j(tval));
                fprintf(stderr, "\nPress enter to continue.\n");

                getchar();
        }
        // printf("Trap: at 0x%08x cause=0x%08x, tval=0x%08x goto 0x%08x\n",
        //        hart_pc_read(hart), cause, tval, hart->pc_next);
}

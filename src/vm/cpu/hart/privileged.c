#include "privileged.h"
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
        // printf("Trap: at 0x%08x cause=0x%08x, tval=0x%08x goto 0x%08x\n",
        //        hart_pc_read(hart), cause, tval, hart->pc_next);
}

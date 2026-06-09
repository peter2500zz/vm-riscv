#include "privileged.h"
#include <stdint.h>

uint32_t hart_csr_read(Hart *hart, CSR csr) { return hart->_csr[csr.addr]; }

void hart_csr_write(Hart *hart, CSR csr, uint32_t value) {
        hart->_csr[csr.addr] = value;
}

void hart_trap_sync(Hart *hart, uint32_t cause, uint32_t tval) {
        hart->trap_pending = 1;

        // 1. 保存 PC
        hart->_csr[CSR_MEPC] =
            hart_pc_read(hart) & ~(uint32_t)0x3; // 无C扩展清低两位

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
        printf("Trap: at 0x%08x cause=0x%08x, tval=0x%08x goto 0x%08x\n",
               hart_pc_read(hart), cause, tval, hart->pc_next);
}

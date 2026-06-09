#include "exec.h"
#include "../../../cpu/hart/privileged.h"

void exec_ecall(Hart *hart) { hart_trap_sync(hart, CAUSE_ECALL_M, 0); }

void exec_mret(Hart *hart) {
        mstatus_t ms;
        ms.raw = hart->_csr[CSR_MSTATUS];

        // 1. 恢复特权级
        hart->priv = ms.MPP;

        // 2. 恢复中断使能
        ms.MIE = ms.MPIE;
        ms.MPIE = 1;

        // 3. 清空 MPP（降权）
        ms.MPP = PRIV_U;

        hart->_csr[CSR_MSTATUS] = ms.raw;

        // 4. 跳转（mepc 已由 handler 修正，如 ecall 已 +4）
        hart->pc_next = hart->_csr[CSR_MEPC];
}

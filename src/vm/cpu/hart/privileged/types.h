#ifndef PRIVILEGED_INST_H
#define PRIVILEGED_INST_H

#include <stdint.h>

#include "../unprivileged/types.h"

/**
 * @brief 特权级别
 *
 */
typedef enum {
        PRIV_U = 0,
        PRIV_S = 1,
        PRIV_M = 3,
} PrivMode;

typedef union {
        uint32_t addr;
} CSR;

static inline CSR inst_csr(Instruction inst) {
        return (CSR){.addr = (uint32_t)inst_imm_i(inst)};
}

#endif // PRIVILEGED_INST_H

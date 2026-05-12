#ifndef PRIVILEGED_INST_H
#define PRIVILEGED_INST_H

#include <stdint.h>

#include "../unprivileged/inst.h"

typedef union {
        uint32_t addr;
} CSR;

static inline CSR inst_csr(Instruction inst) {
        return (CSR){.addr = (uint32_t)inst_imm_i(inst)};
}

#endif // PRIVILEGED_INST_H

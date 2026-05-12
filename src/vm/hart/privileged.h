#ifndef HART_PRIVILEGED_H
#define HART_PRIVILEGED_H

#include <stdint.h>

#include "privileged/inst.h"
#include "unprivileged.h"

uint32_t hart_csr_read(Hart *hart, CSR csr);

void hart_csr_write(Hart *hart, CSR csr, uint32_t value);

#define CSR_READ(hart, csr, out)                                               \
        do {                                                                   \
                (out) = hart_csr_read(hart, csr);                              \
                if ((hart)->trap_pending)                                      \
                        return;                                                \
        } while (0)

#define CSR_WRITE(hart, csr, val)                                              \
        do {                                                                   \
                hart_csr_write(hart, csr, val);                                \
                if ((hart)->trap_pending)                                      \
                        return;                                                \
        } while (0)

#endif // HART_PRIVILEGED_H
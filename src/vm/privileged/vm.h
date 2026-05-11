#ifndef VM_PRIVILEGED_H
#define VM_PRIVILEGED_H

#include <stdint.h>

#include "../spec/vm.h"
#include "inst.h"

uint32_t vm_csr_read(VM *vm, CSR csr);

void vm_csr_write(VM *vm, CSR csr, uint32_t value);

#define CSR_READ(vm, csr, out)                                                 \
        do {                                                                   \
                (out) = vm_csr_read(vm, csr);                                  \
                if ((vm)->trap_pending)                                        \
                        return;                                                \
        } while (0)

#define CSR_WRITE(vm, csr, val)                                                \
        do {                                                                   \
                vm_csr_write(vm, csr, val);                                    \
                if ((vm)->trap_pending)                                        \
                        return;                                                \
        } while (0)

#endif // VM_PRIVILEGED_H
#include "vm.h"

uint32_t vm_csr_read(VM *vm, CSR csr) {
        return vm->_csr[csr.addr];
}

void vm_csr_write(VM *vm, CSR csr, uint32_t value) {
        vm->_csr[csr.addr] = value;
}

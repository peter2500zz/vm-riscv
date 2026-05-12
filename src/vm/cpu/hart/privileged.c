#include "privileged.h"

uint32_t hart_csr_read(Hart *hart, CSR csr) { return hart->_csr[csr.addr]; }

void hart_csr_write(Hart *hart, CSR csr, uint32_t value) {
        hart->_csr[csr.addr] = value;
}

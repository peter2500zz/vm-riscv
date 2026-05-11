
#include "exec.h"
#include "../../vm/privileged/vm.h"

void exec_csrrw(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        CSR csr = inst_csr(inst);

        uint32_t rs1_value = vm_reg_read(vm, rs1);

        if (rd == 0) {
                CSR_WRITE(vm, csr, rs1_value);
        } else {
                uint32_t csr_value;
                CSR_READ(vm, csr, csr_value);
                CSR_WRITE(vm, csr, rs1_value);
                vm_reg_write(vm, rd, csr_value);
        }
}

void exec_csrrs(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        CSR csr = inst_csr(inst);

        uint32_t rs1_value = vm_reg_read(vm, rs1);

        uint32_t csr_value;
        CSR_READ(vm, csr, csr_value);

        if (rs1 == 0) {
                vm_reg_write(vm, rd, csr_value);
        } else {
                CSR_WRITE(vm, csr, csr_value | rs1_value);
                vm_reg_write(vm, rd, csr_value);
        }
}

void exec_csrrc(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        CSR csr = inst_csr(inst);

        uint32_t rs1_value = vm_reg_read(vm, rs1);

        uint32_t csr_value;
        CSR_READ(vm, csr, csr_value);

        if (rs1 == 0) {
                vm_reg_write(vm, rd, csr_value);
        } else {
                CSR_WRITE(vm, csr, csr_value & ~rs1_value);
                vm_reg_write(vm, rd, csr_value);
        }
}

void exec_csrrwi(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t uimm = inst_rs1(inst);
        CSR csr = inst_csr(inst);

        if (rd == 0) {
                CSR_WRITE(vm, csr, uimm);
        } else {
                uint32_t csr_value;
                CSR_READ(vm, csr, csr_value);
                CSR_WRITE(vm, csr, uimm);
                vm_reg_write(vm, rd, csr_value);
        }
}

void exec_csrrsi(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t uimm = inst_rs1(inst);
        CSR csr = inst_csr(inst);

        uint32_t csr_value;
        CSR_READ(vm, csr, csr_value);

        if (uimm == 0) {
                vm_reg_write(vm, rd, csr_value);
        } else {
                CSR_WRITE(vm, csr, csr_value | uimm);
                vm_reg_write(vm, rd, csr_value);
        }
}

void exec_csrrci(VM *vm, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t uimm = inst_rs1(inst);
        CSR csr = inst_csr(inst);

        uint32_t csr_value;
        CSR_READ(vm, csr, csr_value);

        if (uimm == 0) {
                vm_reg_write(vm, rd, csr_value);
        } else {
                CSR_WRITE(vm, csr, csr_value & ~uimm);
                vm_reg_write(vm, rd, csr_value);
        }
}

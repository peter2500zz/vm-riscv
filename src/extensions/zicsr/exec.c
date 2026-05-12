
#include "exec.h"
#include "../../vm/hart/privileged.h"

void exec_csrrw(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        CSR csr = inst_csr(inst);

        uint32_t rs1_value = hart_reg_read(hart, rs1);

        if (rd == 0) {
                CSR_WRITE(hart, csr, rs1_value);
        } else {
                uint32_t csr_value;
                CSR_READ(hart, csr, csr_value);
                CSR_WRITE(hart, csr, rs1_value);
                hart_reg_write(hart, rd, csr_value);
        }
}

void exec_csrrs(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        CSR csr = inst_csr(inst);

        uint32_t rs1_value = hart_reg_read(hart, rs1);

        uint32_t csr_value;
        CSR_READ(hart, csr, csr_value);

        if (rs1 == 0) {
                hart_reg_write(hart, rd, csr_value);
        } else {
                CSR_WRITE(hart, csr, csr_value | rs1_value);
                hart_reg_write(hart, rd, csr_value);
        }
}

void exec_csrrc(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t rs1 = inst_rs1(inst);
        CSR csr = inst_csr(inst);

        uint32_t rs1_value = hart_reg_read(hart, rs1);

        uint32_t csr_value;
        CSR_READ(hart, csr, csr_value);

        if (rs1 == 0) {
                hart_reg_write(hart, rd, csr_value);
        } else {
                CSR_WRITE(hart, csr, csr_value & ~rs1_value);
                hart_reg_write(hart, rd, csr_value);
        }
}

void exec_csrrwi(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t uimm = inst_rs1(inst);
        CSR csr = inst_csr(inst);

        if (rd == 0) {
                CSR_WRITE(hart, csr, uimm);
        } else {
                uint32_t csr_value;
                CSR_READ(hart, csr, csr_value);
                CSR_WRITE(hart, csr, uimm);
                hart_reg_write(hart, rd, csr_value);
        }
}

void exec_csrrsi(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t uimm = inst_rs1(inst);
        CSR csr = inst_csr(inst);

        uint32_t csr_value;
        CSR_READ(hart, csr, csr_value);

        if (uimm == 0) {
                hart_reg_write(hart, rd, csr_value);
        } else {
                CSR_WRITE(hart, csr, csr_value | uimm);
                hart_reg_write(hart, rd, csr_value);
        }
}

void exec_csrrci(Hart *hart, Instruction inst) {
        uint32_t rd = inst_rd(inst);
        uint32_t uimm = inst_rs1(inst);
        CSR csr = inst_csr(inst);

        uint32_t csr_value;
        CSR_READ(hart, csr, csr_value);

        if (uimm == 0) {
                hart_reg_write(hart, rd, csr_value);
        } else {
                CSR_WRITE(hart, csr, csr_value & ~uimm);
                hart_reg_write(hart, rd, csr_value);
        }
}

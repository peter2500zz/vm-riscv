#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "exec.h"
#include "instruction.h"
#include "vm.h"

const char *reg_name[] = {"zero", "ra", "sp",  "gp",  "tp", "t0", "t1", "t2",
                          "s0",   "s1", "a0",  "a1",  "a2", "a3", "a4", "a5",
                          "a6",   "a7", "s2",  "s3",  "s4", "s5", "s6", "s7",
                          "s8",   "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

VM *vm_new(int n) {
        uint32_t size = (uint32_t)(1 << n); // 2^n

        VM *vm = calloc(1, sizeof(VM));
        if (vm == NULL) {
                goto out;
        }
        vm->pc = 0;
        vm->mem_size = size;
        vm->mem = calloc(1, size);
        if (vm->mem == NULL) {
                goto out_free_vm;
        }

        return vm;

out_free_vm:
        free(vm);
out:
        return NULL;
}

void vm_free(VM *vm) {
        if (vm == NULL) {
                return;
        }
        free(vm->mem);
        free(vm);
}

void vm_debug(VM *vm) {
        printf("===== VM stats =====\n");
        printf("Memsize: %d\n", vm->mem_size);
        printf("pc: %d\n", vm->pc);
        for (uint32_t i = 0; i < VM_REG_NUM; i++) {
                printf("%s: 0x%08X\n", reg_name[i], vm_reg_read(vm, i));
        }
        printf("===== End =====\n");
}

int vm_load(VM *vm, const char *filename) {
        int result = 0;
        FILE *fp = fopen(filename, "rb");
        if (fp == NULL) {
                result = 1;
                goto out;
        }

        fseek(fp, 0, SEEK_END);
        size_t file_size = (size_t)ftell(fp);
        if (file_size < 0) {
                result = 1;
                goto out_close_file;
        }
        fseek(fp, 0, SEEK_SET);

        if ((uint32_t)file_size > vm->mem_size) {
                result = 1;
                goto out_close_file;
        }

        size_t bytes_read = fread(vm->mem, 1, file_size, fp);
        if (bytes_read != file_size) {
                result = 1;
                goto out_close_file;
        }

out_close_file:
        fclose(fp);
out:
        return result;
}

Instruction vm_fetch(VM *vm) {
        Instruction inst =
            (Instruction)(vm->mem[(vm->pc) % vm->mem_size] |
                          (vm->mem[(vm->pc + 1) % vm->mem_size] << 8) |
                          (vm->mem[(vm->pc + 2) % vm->mem_size] << 16) |
                          (vm->mem[(vm->pc + 3) % vm->mem_size] << 24));

        // 循环地址
        vm->pc = (vm->pc + 4) % vm->mem_size;

        return inst;
}

int vm_exec(VM *vm, Instruction inst) {
        uint32_t opcode = inst_opcode(inst);
        uint32_t rd = inst_rd(inst);
        uint32_t funct3 = inst_funct3(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t funct7 = inst_funct7(inst);
        int32_t imm_i = inst_imm_i(inst);

        switch (opcode) {
        // U LUI
        case 0x37: // 0b0110111
                break;
        // U AUIPC
        case 0x17: // 0b0010111
                break;
        // J JAL
        case 0x6f: // 0b1101111
                break;
        // I JALR
        case 0x67: // 0b1100111
                break;
        // B 6
        case 0x63: // 0b1100011
                switch (funct3) {
                // BEQ
                case 0x0: // 0b000
                        break;
                // BNE
                case 0x1: // 0b001
                        break;
                // BLT
                case 0x4: // 0b100
                        break;
                // BGE
                case 0x5: // 0b101
                        break;
                // BLTU
                case 0x6: // 0b110
                        break;
                // BGEU
                case 0x7: // 0b111
                        break;
                }

                break;
        // I 5
        case 0x3: // 0b0000011
                switch (funct3) {
                // LB
                case 0x0: // 0b000
                        break;
                // LH
                case 0x1: // 0b001
                        break;
                // LW
                case 0x2: // 0b010
                        break;
                // LBU
                case 0x4: // 0b100
                        break;
                // LHU
                case 0x5: // 0b101
                        break;
                }

                break;
        // S 3
        case 0x23: // 0b0100011
                switch (funct3) {
                // SB
                case 0x0: // 0b000
                        break;
                // SH
                case 0x1: // 0b001
                        break;
                // SW
                case 0x2: // 0b010
                        break;
                }

                break;
        // I+R 9
        case 0x13: // 0b0010011
                switch (funct3) {
                // I 6
                // ADDI
                case 0x0: // 0b000
                        exec_addi(vm, inst);

                        goto done;
                // SLTI
                case 0x2: // 0b010
                        break;
                // SLTIU
                case 0x3: // 0b011
                        break;
                // XORI
                case 0x4: // 0b100
                        break;
                // ORI
                case 0x6: // 0b110
                        break;
                // ANDI
                case 0x7: // 0b111
                        break;
                // R 3
                case 0x1: // 0b001
                        switch (funct7) {
                        // SLLI
                        case 0x0: // 0b0000000
                                break;
                        }

                        break;
                case 0x5: // 0b101
                        switch (funct7) {
                        // SRLI
                        case 0x0: // 0b0000000
                                break;
                        // SRAI
                        case 0x20: // 0b0100000
                                break;
                        }

                        break;
                }

                break;
        // R 10
        case 0x33: // 0b0110011
                switch (funct3) {
                case 0x0: // 0b000
                        switch (funct7) {
                        // ADD
                        case 0x0: // 0b0000000
                                break;
                        // SUB
                        case 0x20: // 0b0100000
                                break;
                        }

                        break;
                case 0x1: // 0b001
                        switch (funct7) {
                        // SLL
                        case 0x0: // 0b0000000
                                break;
                        }

                        break;
                case 0x2: // 0b010
                        switch (funct7) {
                        // SLT
                        case 0x0: // 0b0000000
                                break;
                        }

                        break;
                case 0x3: // 0b011
                        switch (funct7) {
                        // SLTU
                        case 0x0: // 0b0000000
                                break;
                        }

                        break;
                case 0x4: // 0b100
                        switch (funct7) {
                        // XOR
                        case 0x0: // 0b0000000
                                break;
                        }

                        break;
                case 0x5: // 0b101
                        switch (funct7) {
                        // SRL
                        case 0x0: // 0b0000000
                                break;
                        // SRA
                        case 0x20: // 0b0100000
                                break;
                        }

                        break;
                case 0x6: // 0b110
                        switch (funct7) {
                        // OR
                        case 0x0: // 0b0000000
                                break;
                        }

                        break;
                case 0x7: // 0b111
                        switch (funct7) {
                        // AND
                        case 0x0: // 0b0000000
                                break;
                        }

                        break;
                }

                break;
        // I 3
        case 0xf: // 0b0001111
                switch (funct3) {
                case 0x0: { // 0b000
                        uint32_t succ = BITS(inst, 23, 20);
                        uint32_t pred = BITS(inst, 27, 24);
                        uint32_t fm = BITS(inst, 31, 28);

                        // FENCE.TSO
                        if (rd == 0x0      // 0b00000
                            && rs1 == 0x0  // 0b00000
                            && succ == 0x3 // 0b0011
                            && pred == 0x3 // 0b0011
                            && fm == 0x8   // 0b1000
                        ) {

                                break;
                        }
                        // PAUSE
                        else if (rd == 0x0      // 0b00000
                                 && rs1 == 0x0  // 0b00000
                                 && succ == 0x0 // 0b0000
                                 && pred == 0x1 // 0b0001
                                 && fm == 0x0   // 0b0000
                        ) {

                                break;
                        }
                        // FENCE
                        else {

                                break;
                        }
                }
                }

                break;
        // I 2
        case 0x73: // 0b1110011
                switch (funct3) {
                case 0x0: // 0b000
                        // ECALL
                        if (rd == 0x0       // 0b00000
                            && rs1 == 0x0   // 0b00000
                            && imm_i == 0x0 // 0b000000000000
                        ) {

                                break;
                        }
                        // EBREAK
                        else if (rd == 0x0       // 0b00000
                                 && rs1 == 0x0   // 0b00000
                                 && imm_i == 0x1 // 0b000000000001
                        ) {

                                break;
                        }

                        break;
                }

                break;
        }

        return 1;

done:
        return 0;
}

void vm_step(VM *vm) {
        Instruction inst = vm_fetch(vm);
        printf("opcode: 0x%08X\n", inst_opcode(inst));

        if (vm_exec(vm, inst) != 0) {
                fprintf(stderr, "Unknown opcode\n");
        }
}

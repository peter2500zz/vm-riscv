#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "instruction.h"
#include "vm.h"

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
        Instruction inst = vm->mem[(vm->pc) % vm->mem_size] |
                           (vm->mem[(vm->pc + 1) % vm->mem_size] << 8) |
                           (vm->mem[(vm->pc + 2) % vm->mem_size] << 16) |
                           (vm->mem[(vm->pc + 3) % vm->mem_size] << 24);

        // 循环地址
        vm->pc = (vm->pc + 4) % vm->mem_size;

        return inst;
}

int vm_exec(Instruction inst) {
        uint32_t opcode = inst_opcode(inst);
        uint32_t rd = inst_rd(inst);
        uint32_t funct3 = inst_funct3(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t funct7 = inst_funct7(inst);
        uint32_t imm_i = inst_imm_i(inst);

        switch (opcode) {
        // U LUI
        case 0b0110111:
                break;
        // U AUIPC
        case 0b0010111:
                break;
        // J JAL
        case 0b1101111:
                break;
        // I JALR
        case 0b1100111:
                break;
        // B 6
        case 0b1100011:
                switch (funct3) {
                // BEQ
                case 0b000:
                        break;
                // BNE
                case 0b001:
                        break;
                // BLT
                case 0b100:
                        break;
                // BGE
                case 0b101:
                        break;
                // BLTU
                case 0b110:
                        break;
                // BGEU
                case 0b111:
                        break;
                }

                break;
        // I 5
        case 0b0000011:
                switch (funct3) {
                // LB
                case 0b000:
                        break;
                // LH
                case 0b001:
                        break;
                // LW
                case 0b010:
                        break;
                // LBU
                case 0b100:
                        break;
                // LHU
                case 0b101:
                        break;
                }

                break;
        // S 3
        case 0b0100011:
                switch (funct3) {
                // SB
                case 0b000:
                        break;
                // SH
                case 0b001:
                        break;
                // SW
                case 0b010:
                        break;
                }

                break;
        // I+R 9
        case 0b0010011:
                switch (funct3) {
                // I 6
                // ADDI
                case 0b000:
                        break;
                // SLTI
                case 0b010:
                        break;
                // SLTIU
                case 0b011:
                        break;
                // XORI
                case 0b100:
                        break;
                // ORI
                case 0b110:
                        break;
                // ANDI
                case 0b111:
                        break;
                // R 3
                case 0b001:
                        switch (funct7) {
                        // SLLI
                        case 0b0000000:
                                break;
                        }

                        break;
                case 0b101:
                        switch (funct7) {
                        // SRLI
                        case 0b0000000:
                                break;
                        // SRAI
                        case 0b0100000:
                                break;
                        }

                        break;
                }

                break;
        // R 10
        case 0b0110011:
                switch (funct3) {
                case 0b000:
                        switch (funct7) {
                        // ADD
                        case 0b0000000:
                                break;
                        // SUB
                        case 0b0100000:
                                break;
                        }

                        break;
                case 0b001:
                        switch (funct7) {
                        // SLL
                        case 0b0000000:
                                break;
                        }

                        break;
                case 0b010:
                        switch (funct7) {
                        // SLT
                        case 0b0000000:
                                break;
                        }

                        break;
                case 0b011:
                        switch (funct7) {
                        // SLTU
                        case 0b0000000:
                                break;
                        }

                        break;
                case 0b100:
                        switch (funct7) {
                        // XOR
                        case 0b0000000:
                                break;
                        }

                        break;
                case 0b101:
                        switch (funct7) {
                        // SRL
                        case 0b0000000:
                                break;
                        // SRA
                        case 0b0100000:
                                break;
                        }

                        break;
                case 0b110:
                        switch (funct7) {
                        // OR
                        case 0b0000000:
                                break;
                        }

                        break;
                case 0b111:
                        switch (funct7) {
                        // AND
                        case 0b0000000:
                                break;
                        }

                        break;
                }

                break;
        // I 3
        case 0b0001111:
                switch (funct3) {
                case 0b000: {
                        uint32_t succ = BITS(inst, 23, 20);
                        uint32_t pred = BITS(inst, 27, 24);
                        uint32_t fm = BITS(inst, 31, 28);

                        // FENCE.TSO
                        if (rd == 0b00000 && rs1 == 0b00000 && succ == 0b0011 &&
                            pred == 0b0011 && fm == 0b1000) {

                                break;
                        }
                        // PAUSE
                        else if (rd == 0b00000 && rs1 == 0b00000 &&
                                 succ == 0b0000 && pred == 0b0001 &&
                                 fm == 0b0000) {

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
        case 0b1110011:
                switch (funct3) {
                case 0b000:
                        // ECALL
                        if (rd == 0b00000 && rs1 == 0b00000 &&
                            imm_i == 0b000000000000) {

                                break;
                        }
                        // EBREAK
                        else if (rd == 0b00000 && rs1 == 0b00000 &&
                                 imm_i == 0b000000000001) {

                                break;
                        }

                        break;
                }

                break;
        default:
                return 1;
        }

        return 0;
}

void vm_step(VM *vm) {
        Instruction inst = vm_fetch(vm);
        printf("opcode: 0x%08X\n", inst_opcode(inst));

        if (vm_exec(inst) != 0) {
                fprintf(stderr, "Unknown opcode\n");
        }
}

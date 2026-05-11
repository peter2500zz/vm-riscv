#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ecall.h"
#include "exec/i.h"
#include "exec/m.h"
#include "instruction.h"
#include "vm.h"

const char *reg_name[] = {"zero", "ra", "sp",  "gp",  "tp", "t0", "t1", "t2",
                          "s0",   "s1", "a0",  "a1",  "a2", "a3", "a4", "a5",
                          "a6",   "a7", "s2",  "s3",  "s4", "s5", "s6", "s7",
                          "s8",   "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

VM *vm_new(int n) {
        // 越界
        if (n < 0 || n > 31) {
                goto out;
        }
        uint32_t size = (uint32_t)1 << n; // 2^n

        VM *vm = calloc(1, sizeof(VM));
        if (vm == NULL) {
                goto out;
        }
        vm->mem_size = size;
        vm->_mem = calloc(1, size);
        if (vm->_mem == NULL) {
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
        free(vm->_mem);
        free(vm);
}

void vm_debug(VM *vm) {
        printf("===== VM stats =====\n");
        printf("Memsize: %d\n", vm->mem_size);
        printf("pc: %d\n", vm_pc_read(vm));
        for (uint32_t i = 0; i < VM_REG_NUM; i++) {
                printf("%s: 0x%08X\n", reg_name[i], vm_reg_read(vm, i));
        }
        printf("===== End =====\n");
}

int vm_load(VM *vm, uint32_t offset, uint8_t *buffer, uint32_t size) {
        if (offset >= vm->mem_size || vm->mem_size - offset < size) {
                return 1;
        }

        memcpy(vm->_mem + offset, buffer, size);

        return 0;
}

Instruction vm_fetch(VM *vm) {
        Instruction inst = (Instruction)(*vm_mem_ptr_word(vm, vm_pc_read(vm)));

        return inst;
}

typedef struct {
        uint8_t e_ident[16];
        uint16_t e_type, e_machine;
        uint32_t e_version, e_entry, e_phoff, e_shoff, e_flags;
        uint16_t e_ehsize, e_phentsize, e_phnum;
        uint16_t e_shentsize, e_shnum, e_shstrndx;
} Elf32_Ehdr;

typedef struct {
        uint32_t p_type, p_offset, p_vaddr, p_paddr;
        uint32_t p_filesz, p_memsz, p_flags, p_align;
} Elf32_Phdr;

int vm_load_elf(VM *vm, uint8_t *buffer, uint32_t size) {
        if (size < sizeof(Elf32_Ehdr)) {
                fprintf(stderr, "Buffer too small for ELF header\n");
                return 1;
        }

        Elf32_Ehdr *eh = (Elf32_Ehdr *)buffer;

        // 校验魔数
        if (eh->e_ident[0] != 0x7f || eh->e_ident[1] != 'E' ||
            eh->e_ident[2] != 'L' || eh->e_ident[3] != 'F') {
                fprintf(stderr, "Not an ELF file\n");
                return 1;
        }
        // 校验 32 位 / 小端 / RISC-V
        if (eh->e_ident[4] != 1) {
                fprintf(stderr, "Not a 32-bit ELF\n");
                return 1;
        }
        if (eh->e_ident[5] != 1) {
                fprintf(stderr, "Not a little-endian ELF\n");
                return 1;
        }
        if (eh->e_machine != 0xF3) {
                fprintf(stderr, "Not a RISC-V ELF (e_machine=0x%X)\n",
                        eh->e_machine);
                return 1;
        }

        // 校验 Program Header Table 边界
        if (eh->e_phoff + (uint32_t)eh->e_phnum * eh->e_phentsize > size) {
                fprintf(stderr, "Program header table out of buffer bounds\n");
                return 1;
        }

        // 遍历所有 Program Header，只处理 PT_LOAD
        for (int i = 0; i < eh->e_phnum; i++) {
                Elf32_Phdr *ph =
                    (Elf32_Phdr *)(buffer + eh->e_phoff + i * eh->e_phentsize);
                if (ph->p_type != 1)
                        continue; // 跳过非 PT_LOAD

                // 校验段数据在 buffer 内
                if (ph->p_offset + ph->p_filesz > size) {
                        fprintf(stderr,
                                "Segment %d file data out of buffer bounds\n",
                                i);
                        return 1;
                }

                // 复制文件内容到虚拟地址
                if (ph->p_filesz > 0) {
                        if (vm_load(vm, ph->p_vaddr, buffer + ph->p_offset,
                                    ph->p_filesz) != 0) {
                                fprintf(stderr, "Failed to load segment %d\n",
                                        i);
                                return 1;
                        }
                }

                // BSS：memsz 比 filesz 大的部分清零
                if (ph->p_memsz > ph->p_filesz) {
                        uint32_t bss_addr = ph->p_vaddr + ph->p_filesz;
                        uint32_t bss_size = ph->p_memsz - ph->p_filesz;
                        if (bss_addr + bss_size > vm->mem_size) {
                                fprintf(stderr,
                                        "BSS segment %d out of VM memory\n", i);
                                return 1;
                        }
                        memset(vm->_mem + bss_addr, 0, bss_size);
                }
        }

        // 设置 PC 为入口地址
        vm_pc_write(vm, eh->e_entry);
        return 0;
}

int vm_exec(VM *vm, Instruction inst) {
        vm->pc_next = vm_pc_read(vm) + 4;

        uint32_t opcode = inst_opcode(inst);
        uint32_t rd = inst_rd(inst);
        uint32_t funct3 = inst_funct3(inst);
        uint32_t rs1 = inst_rs1(inst);
        uint32_t funct7 = inst_funct7(inst);
        int32_t imm_i = inst_imm_i(inst);

        switch (opcode) {
        // U LUI
        case 0x37: // 0b0110111
                exec_lui(vm, inst);

                goto done;
        // U AUIPC
        case 0x17: // 0b0010111
                exec_auipc(vm, inst);

                goto done;
        // J JAL
        case 0x6f: // 0b1101111
                exec_jal(vm, inst);

                goto done;
        // I JALR
        case 0x67: // 0b1100111
                exec_jalr(vm, inst);

                goto done;
        // B 6
        case 0x63: // 0b1100011
                switch (funct3) {
                // BEQ
                case 0x0: // 0b000
                        exec_beq(vm, inst);

                        goto done;
                // BNE
                case 0x1: // 0b001
                        exec_bne(vm, inst);

                        goto done;
                // BLT
                case 0x4: // 0b100
                        exec_blt(vm, inst);

                        goto done;
                // BGE
                case 0x5: // 0b101
                        exec_bge(vm, inst);

                        goto done;
                // BLTU
                case 0x6: // 0b110
                        exec_bltu(vm, inst);

                        goto done;
                // BGEU
                case 0x7: // 0b111
                        exec_bgeu(vm, inst);

                        goto done;
                }

                break;
        // I 5
        case 0x3: // 0b0000011
                switch (funct3) {
                // LB
                case 0x0: // 0b000
                        exec_lb(vm, inst);

                        goto done;
                // LH
                case 0x1: // 0b001
                        exec_lh(vm, inst);

                        goto done;
                // LW
                case 0x2: // 0b010
                        exec_lw(vm, inst);

                        goto done;
                // LBU
                case 0x4: // 0b100
                        exec_lbu(vm, inst);

                        goto done;
                // LHU
                case 0x5: // 0b101
                        exec_lhu(vm, inst);

                        goto done;
                }

                break;
        // S 3
        case 0x23: // 0b0100011
                switch (funct3) {
                // SB
                case 0x0: // 0b000
                        exec_sb(vm, inst);

                        goto done;
                // SH
                case 0x1: // 0b001
                        exec_sh(vm, inst);

                        goto done;
                // SW
                case 0x2: // 0b010
                        exec_sw(vm, inst);

                        goto done;
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
                        exec_slti(vm, inst);

                        goto done;
                // SLTIU
                case 0x3: // 0b011
                        exec_sltiu(vm, inst);

                        goto done;
                // XORI
                case 0x4: // 0b100
                        exec_xori(vm, inst);

                        goto done;
                // ORI
                case 0x6: // 0b110
                        exec_ori(vm, inst);

                        goto done;
                // ANDI
                case 0x7: // 0b111
                        exec_andi(vm, inst);

                        goto done;
                // R 3
                case 0x1: // 0b001
                        switch (funct7) {
                        // SLLI
                        case 0x0: // 0b0000000
                                exec_slli(vm, inst);

                                goto done;
                        }

                        break;
                case 0x5: // 0b101
                        switch (funct7) {
                        // SRLI
                        case 0x0: // 0b0000000
                                exec_srli(vm, inst);

                                goto done;
                        // SRAI
                        case 0x20: // 0b0100000
                                exec_srai(vm, inst);

                                goto done;
                        }

                        break;
                }

                break;
        // R 10
        case 0x33: // 0b0110011
                switch (funct3) {
                case 0x00: // 0b000
                        switch (funct7) {
                        // ADD
                        case 0x00: // 0b0000000
                                exec_add(vm, inst);

                                goto done;
                        // MUL
                        case 0x01: // 0b0000001
                                exec_mul(vm, inst);

                                goto done;
                        // SUB
                        case 0x20: // 0b0100000
                                exec_sub(vm, inst);

                                goto done;
                        }

                        break;
                case 0x01: // 0b001
                        switch (funct7) {
                        // SLL
                        case 0x00: // 0b0000000
                                exec_sll(vm, inst);

                                goto done;
                        // MULH
                        case 0x01: // 0b0000001
                                exec_mulh(vm, inst);

                                goto done;
                        }

                        break;
                case 0x02: // 0b010
                        switch (funct7) {
                        // SLT
                        case 0x00: // 0b0000000
                                exec_slt(vm, inst);

                                goto done;
                        // MULHSU
                        case 0x01: // 0b0000001
                                exec_mulhsu(vm, inst);

                                goto done;
                        }

                        break;
                case 0x03: // 0b011
                        switch (funct7) {
                        // SLTU
                        case 0x00: // 0b0000000
                                exec_sltu(vm, inst);

                                goto done;
                        // MULHU
                        case 0x01: // 0b0000001
                                exec_mulhu(vm, inst);

                                goto done;
                        }

                        break;
                case 0x04: // 0b100
                        switch (funct7) {
                        // XOR
                        case 0x00: // 0b0000000
                                exec_xor(vm, inst);

                                goto done;
                        // DIV
                        case 0x01: // 0b0000001
                                exec_div(vm, inst);

                                goto done;
                        }

                        break;
                case 0x05: // 0b101
                        switch (funct7) {
                        // SRL
                        case 0x00: // 0b0000000
                                exec_srl(vm, inst);

                                goto done;
                        // DIVU
                        case 0x01: // 0b0000001
                                exec_divu(vm, inst);

                                goto done;
                        // SRA
                        case 0x20: // 0b0100000
                                exec_sra(vm, inst);

                                goto done;
                        }

                        break;
                case 0x06: // 0b110
                        switch (funct7) {
                        // OR
                        case 0x00: // 0b0000000
                                exec_or(vm, inst);

                                goto done;
                        // REM
                        case 0x01: // 0b0000001
                                exec_rem(vm, inst);

                                goto done;
                        }

                        break;
                case 0x07: // 0b111
                        switch (funct7) {
                        // AND
                        case 0x00: // 0b0000000
                                exec_and(vm, inst);

                                goto done;
                        // REMU
                        case 0x01: // 0b0000001
                                exec_remu(vm, inst);

                                goto done;
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
                                handle_ecall(vm);

                                goto done;
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
        vm_pc_write(vm, vm->pc_next);

        return 0;
}

void vm_step(VM *vm) {
        Instruction inst = vm_fetch(vm);
        // printf("opcode: 0x%08X\n", inst_opcode(inst));

        if (vm_exec(vm, inst) != 0) {
                fprintf(stderr, "==== Unknown instruction ====\n");
                fprintf(stderr, "At 0x%08X\n", vm_pc_read(vm));
                fprintf(stderr, "Instruction: 0x%08X\n", inst);
                fprintf(stderr, "\topcode: 0x%02X\n", inst_opcode(inst));
                fprintf(stderr, "\tfunct3: 0x%02X\n", inst_funct3(inst));
                fprintf(stderr, "\tfunct7: 0x%02X\n", inst_funct7(inst));
                fprintf(stderr, "\trd: %d\n", inst_rd(inst));
                fprintf(stderr, "\trs1: %d\n", inst_rs1(inst));
                fprintf(stderr, "\trs2: %d\n", inst_rs2(inst));
                fprintf(stderr, "\timm_i: %d\n", inst_imm_i(inst));
                fprintf(stderr, "\timm_b: %d\n", inst_imm_b(inst));
                fprintf(stderr, "\timm_s: %d\n", inst_imm_s(inst));
                fprintf(stderr, "\timm_u: %d\n", inst_imm_u(inst));
                fprintf(stderr, "\timm_j: %d\n", inst_imm_j(inst));
                fprintf(stderr, "\nPress enter to exit.\n");

                getchar();
                exit(1);
        }
}

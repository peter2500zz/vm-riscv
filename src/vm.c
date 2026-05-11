#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vm.h"
#include "instruction.h"
#include "dispatcher/root.h"

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

void vm_step(VM *vm) {
        Instruction inst = vm_fetch(vm);
        // printf("opcode: 0x%08X\n", inst_opcode(inst));

        if (vm_dispatch(vm, inst) != 0) {
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

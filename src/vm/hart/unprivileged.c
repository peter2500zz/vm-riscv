#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../dispatcher/root.h"
#include "unprivileged.h"

const char *reg_name[] = {"zero", "ra", "sp",  "gp",  "tp", "t0", "t1", "t2",
                          "s0",   "s1", "a0",  "a1",  "a2", "a3", "a4", "a5",
                          "a6",   "a7", "s2",  "s3",  "s4", "s5", "s6", "s7",
                          "s8",   "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

Hart *hart_new(int n) {
        // 越界
        if (n < 0 || n > 31) {
                goto out;
        }
        uint32_t size = (uint32_t)1 << n; // 2^n

        Hart *hart = calloc(1, sizeof(Hart));
        if (hart == NULL) {
                goto out;
        }
        hart->mem_size = size;
        hart->_mem = calloc(1, size);
        if (hart->_mem == NULL) {
                goto out_free_hart;
        }

        return hart;

out_free_hart:
        free(hart);
out:
        return NULL;
}

void hart_free(Hart *hart) {
        if (hart == NULL) {
                return;
        }
        free(hart->_mem);
        free(hart);
}

void hart_debug(Hart *hart) {
        printf("===== Hart stats =====\n");
        printf("Memsize: %d\n", hart->mem_size);
        printf("pc: %d\n", hart_pc_read(hart));
        for (uint32_t i = 0; i < HART_REG_NUM; i++) {
                printf("%s: 0x%08X\n", reg_name[i], hart_reg_read(hart, i));
        }
        printf("===== End =====\n");
}

int hart_load(Hart *hart, uint32_t offset, uint8_t *buffer, uint32_t size) {
        if (offset >= hart->mem_size || hart->mem_size - offset < size) {
                return 1;
        }

        memcpy(hart->_mem + offset, buffer, size);

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

int hart_load_elf(Hart *hart, uint8_t *buffer, uint32_t size) {
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
                        if (hart_load(hart, ph->p_vaddr, buffer + ph->p_offset,
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
                        if (bss_addr + bss_size > hart->mem_size) {
                                fprintf(stderr,
                                        "BSS segment %d out of Hart memory\n", i);
                                return 1;
                        }
                        memset(hart->_mem + bss_addr, 0, bss_size);
                }
        }

        // 设置 PC 为入口地址
        hart_pc_write(hart, eh->e_entry);
        return 0;
}

Instruction hart_fetch(Hart *hart) {
        Instruction inst = (Instruction)(*hart_mem_ptr_word(hart, hart_pc_read(hart)));

        return inst;
}

void hart_step(Hart *hart) {
        Instruction inst = hart_fetch(hart);
        // printf("opcode: 0x%08X\n", inst_opcode(inst));

        if (hart_dispatch(hart, inst) != 0) {
                fprintf(stderr, "==== Unknown instruction ====\n");
                fprintf(stderr, "At 0x%08X\n", hart_pc_read(hart));
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

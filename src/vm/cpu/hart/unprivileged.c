#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../memory/memory.h"
#include "../dispatcher/root.h"
#include "unprivileged.h"

const char *reg_name[] = {"zero", "ra", "sp",  "gp",  "tp", "t0", "t1", "t2",
                          "s0",   "s1", "a0",  "a1",  "a2", "a3", "a4", "a5",
                          "a6",   "a7", "s2",  "s3",  "s4", "s5", "s6", "s7",
                          "s8",   "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

int hart_init(Hart *hart, Memory *mem) {
        int result = 0;

        hart->mem = mem;

        // 特权模式
        hart->priv = PRIV_M;

        return result;
}

void hart_debug(Hart *hart) {
        printf("===== Hart stats =====\n");
        printf("Memsize: %u\n", hart->mem->ram->size);
        printf("pc: 0x%08X\n", hart_pc_read(hart));
        for (uint32_t i = 0; i < HART_REG_NUM; i++) {
                printf("%s: 0x%08X\n", reg_name[i], hart_reg_read(hart, i));
        }
        printf("===== End =====\n");
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

Instruction hart_fetch(Hart *hart) {
        Instruction inst;
        readMemory(hart->mem, hart_pc_read(hart), &inst, sizeof(uint32_t));
        return inst;
}

void hart_step(Hart *hart) {
        Instruction inst = hart_fetch(hart);
        // printf("opcode: 0x%08X\n", inst_opcode(inst));
        hart_dispatch(hart, inst);
}

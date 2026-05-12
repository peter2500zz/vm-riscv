#include "machine.h"
#include "cpu/hart/unprivileged.h"
#include "memory/access.h"
#include <stdint.h>
#include <string.h>

Machine *machine_new(uint32_t hart_num, uint32_t mem_size) {
        if (hart_num == 0 || mem_size == 0) {
                fprintf(stderr, "Invalid parameters for machine creation\n");
                return NULL;
        }

        // 分配 Machine 结构体
        Machine *machine = calloc(1, sizeof(Machine));
        if (machine == NULL) {
                fprintf(stderr,
                        "Failed to allocate memory for machine struct\n");
                return NULL;
        }

        // 分配内存
        machine->mem = memory_new(mem_size);
        if (machine->mem == NULL) {
                fprintf(stderr,
                        "Failed to allocate memory for machine memory\n");
                goto out_free_machine;
        }

        // 分配硬件线程数组
        machine->hart_num = hart_num;
        machine->harts = calloc(hart_num, sizeof(Hart));
        if (machine->harts == NULL) {
                fprintf(stderr, "Failed to allocate memory for harts\n");
                goto out_free_mem;
        }

        // 初始化每个硬件线程
        for (uint32_t i = 0; i < hart_num; i++) {
                if (hart_init(&machine->harts[i], machine->mem) != 0) {
                        fprintf(stderr, "Failed to initialize hart %d\n", i);
                        goto out_free_harts;
                }
        }

        return machine;

out_free_harts:
        free(machine->harts);
out_free_mem:
        free(machine->mem);
out_free_machine:
        free(machine);
        return NULL;
}

void machine_free(Machine *machine) {
        if (machine == NULL) {
                return;
        }

        // 释放硬件线程
        free(machine->harts);
        // 释放内存
        memory_free(machine->mem);
        // 释放 Machine 结构体
        free(machine);
}

void machine_load_raw(Machine *machine, uint8_t *buffer, uint32_t size) {
        memcpy(machine->mem->data, buffer, size);

        // 设置程序计数器指向 RAM 的起始地址
        machine->harts[0]._pc = RAM_ADDR;
        // 设置栈顶指向 RAM 的末尾
        machine->harts[0]._regs[2] =
            (RAM_ADDR + machine->mem->size) & ~(uint32_t)0xf;
}

typedef struct {
        uint8_t e_ident[16];
        uint16_t e_type;
        uint16_t e_machine;
        uint32_t e_version;
        uint32_t e_entry;
        uint32_t e_phoff;
        uint32_t e_shoff;
        uint32_t e_flags;
        uint16_t e_ehsize;
        uint16_t e_phentsize;
        uint16_t e_phnum;
        uint16_t e_shentsize;
        uint16_t e_shnum;
        uint16_t e_shstrndx;
} Elf32_Ehdr;

typedef struct {
        uint32_t p_type;
        uint32_t p_offset;
        uint32_t p_vaddr;
        uint32_t p_paddr;
        uint32_t p_filesz;
        uint32_t p_memsz;
        uint32_t p_flags;
        uint32_t p_align;
} Elf32_Phdr;

#define PT_LOAD 1

static void elf_mem_write(Hart *hart, uint32_t addr, uint8_t *src,
                          uint32_t size) {
        for (uint32_t i = 0; i < size; i++) {
                memory_access(hart, addr + i, &src[i], sizeof(uint8_t),
                              MEM_WRITE);
        }
}

static void elf_mem_zero(Hart *hart, uint32_t addr, uint32_t size) {
        uint8_t zero = 0;
        for (uint32_t i = 0; i < size; i++) {
                memory_access(hart, addr + i, &zero, sizeof(uint8_t),
                              MEM_WRITE);
        }
}

int machine_load_elf(Machine *machine, uint8_t *buffer, uint32_t size) {
        Elf32_Ehdr *eh = (Elf32_Ehdr *)buffer;

        if (size < sizeof(Elf32_Ehdr)) {
                fprintf(stderr, "Too small\n");
                return 1;
        }
        if (eh->e_ident[0] != 0x7f || eh->e_ident[1] != 'E' ||
            eh->e_ident[2] != 'L' || eh->e_ident[3] != 'F') {
                fprintf(stderr, "Not ELF\n");
                return 1;
        }
        if (eh->e_ident[4] != 1) {
                fprintf(stderr, "Not 32-bit\n");
                return 1;
        }
        if (eh->e_ident[5] != 1) {
                fprintf(stderr, "Not little-endian\n");
                return 1;
        }
        if (eh->e_machine != 0xF3) {
                fprintf(stderr, "Not RISC-V\n");
                return 1;
        }
        if (eh->e_phoff + (uint32_t)eh->e_phnum * eh->e_phentsize > size) {
                fprintf(stderr, "Program header out of bounds\n");
                return 1;
        }

        for (int i = 0; i < eh->e_phnum; i++) {
                Elf32_Phdr *ph =
                    (Elf32_Phdr *)(buffer + eh->e_phoff + i * eh->e_phentsize);
                if (ph->p_type != PT_LOAD)
                        continue;

                if (ph->p_offset + ph->p_filesz > size) {
                        fprintf(stderr, "Segment %d out of buffer\n", i);
                        return 1;
                }

                if (ph->p_filesz > 0)
                        elf_mem_write(&machine->harts[0], ph->p_vaddr,
                                      buffer + ph->p_offset, ph->p_filesz);

                if (ph->p_memsz > ph->p_filesz)
                        elf_mem_zero(&machine->harts[0],
                                     ph->p_vaddr + ph->p_filesz,
                                     ph->p_memsz - ph->p_filesz);
        }

        machine->harts[0]._pc = eh->e_entry;
        // 设置栈顶指向 RAM 的末尾
        machine->harts[0]._regs[2] =
            (RAM_ADDR + machine->mem->size) & ~(uint32_t)0xf;
        return 0;
}

void machine_go(Machine *machine) {
        while (1) {
                hart_step(&machine->harts[0]);
        }
}

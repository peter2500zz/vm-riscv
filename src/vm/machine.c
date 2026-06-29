#include "machine.h"
#include "cpu/hart/privileged.h"
#include "cpu/hart/unprivileged.h"
#include "memory/memory.h"
#include <stdint.h>
#include <string.h>
#include <time.h>
// tmp
#include "devices/clint/handler.h"

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
        machine->mem = newMemory(mem_size);
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
        freeMemory(&machine->mem);
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
        freeMemory(&machine->mem);
        // 释放 Machine 结构体
        free(machine);
}

void machine_load_raw(Machine *machine, uint8_t *buffer, uint32_t size) {
        memcpy(machine->mem->ram->data, buffer, size);

        // 设置程序计数器指向 RAM 的起始地址
        machine->harts[0]._pc = RAM_BASE;
        // 设置栈顶指向 RAM 的末尾
        machine->harts[0]._regs[2] =
            (RAM_BASE + machine->mem->ram->size) & ~(uint32_t)0xf;
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
                writeMemory(hart->mem, addr + i, &src[i], sizeof(uint8_t));
        }
}

static void elf_mem_zero(Hart *hart, uint32_t addr, uint32_t size) {
        uint8_t zero = 0;
        for (uint32_t i = 0; i < size; i++) {
                writeMemory(hart->mem, addr + i, &zero, sizeof(uint8_t));
        }
}

int machine_load_elf(Machine *machine, uint8_t *buffer, uint32_t size) {
        printf("==== machine_load_elf ====\n");

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
        printf("ELF header OK: 32-bit little-endian RISC-V\n");
        printf("  e_entry=0x%08x, e_phnum=%d\n", eh->e_entry, eh->e_phnum);

        if (eh->e_phoff + (uint32_t)eh->e_phnum * eh->e_phentsize > size) {
                fprintf(stderr, "Program header out of bounds\n");
                return 1;
        }

        for (int i = 0; i < eh->e_phnum; i++) {
                Elf32_Phdr *ph =
                    (Elf32_Phdr *)(buffer + eh->e_phoff + i * eh->e_phentsize);
                if (ph->p_type != PT_LOAD) {
                        printf("  segment %d: type=0x%x, skipping\n", i,
                               ph->p_type);
                        continue;
                }
                printf("  segment %d: LOAD vaddr=0x%08x paddr=0x%08x "
                       "filesz=0x%x memsz=0x%x\n",
                       i, ph->p_vaddr, ph->p_paddr, ph->p_filesz, ph->p_memsz);
                if (ph->p_offset + ph->p_filesz > size) {
                        fprintf(stderr, "Segment %d out of buffer\n", i);
                        return 1;
                }
                // 段的初始映像放到加载地址(LMA, p_paddr)，而不是运行地址
                // (VMA, p_vaddr)。裸机启动代码(crt0)会在运行时把 .data
                // 从 LMA 复制到 VMA、并清零 .bss。对于 p_paddr==p_vaddr
                // 的段(如 .text)，这没有区别。
                if (ph->p_filesz > 0) {
                        printf("    writing 0x%x bytes to paddr=0x%08x\n",
                               ph->p_filesz, ph->p_paddr);
                        elf_mem_write(&machine->harts[0], ph->p_paddr,
                                      buffer + ph->p_offset, ph->p_filesz);
                }
                if (ph->p_memsz > ph->p_filesz) {
                        printf("    zeroing 0x%x bytes at paddr=0x%08x\n",
                               ph->p_memsz - ph->p_filesz,
                               ph->p_paddr + ph->p_filesz);
                        elf_mem_zero(&machine->harts[0],
                                     ph->p_paddr + ph->p_filesz,
                                     ph->p_memsz - ph->p_filesz);
                }
        }

        machine->harts[0]._pc = eh->e_entry;
        machine->harts[0]._regs[2] =
            (RAM_BASE + machine->mem->ram->size) & ~(uint32_t)0xf;

        printf("  pc=0x%08x sp=0x%08x\n", machine->harts[0]._pc,
               machine->harts[0]._regs[2]);
        printf("==== machine_load_elf done ====\n");
        return 0;
}

uint32_t timebase_freq = 10000000; // 10 MHz

void machine_go(Machine *machine) {
        Hart *hart = &machine->harts[0];
        while (1) {
                hart_step(hart);

                // CLINT：递增 mtime，并据此驱动 mip.MTIP（定时器中断挂起位）。
                // MTIP 是硬件状态：mtime>=mtimecmp 时置位，handler 写新的
                // mtimecmp 后自动清除。
                // mtime++;
                struct timespec _ts;
                clock_gettime(CLOCK_MONOTONIC, &_ts);
                mtime = (uint64_t)(((uint64_t)_ts.tv_sec * 1000000000ULL +
                                    (uint64_t)_ts.tv_nsec) *
                                   timebase_freq / 1000000000ULL);
                if (mtime >= mtimecmp) {
                        hart->_csr[CSR_MIP] |= MIP_MTIP;
                } else {
                        hart->_csr[CSR_MIP] &= ~MIP_MTIP;
                }

                // 投递中断的条件：全局开中断(mstatus.MIE) && 该源使能(mie) &&
                // 该源挂起(mip)。客户机在临界区里清了 MIE，中断就保持挂起、
                // 不投递，直到它重新开中断——这样才不会破坏临界区。
                // 同时 !trap_pending 避免和本步刚产生的同步异常(ecall)相撞。
                mstatus_t ms;
                ms.raw = hart->_csr[CSR_MSTATUS];
                uint32_t irq = hart->_csr[CSR_MIE] & hart->_csr[CSR_MIP];
                if (!hart->trap_pending && ms.MIE && (irq & MIP_MTIP)) {
                        hart_trap_sync(hart, CAUSE_MTI, 0);
                }

                // 应用 trap 跳转（同步异常或上面投递的定时器中断）。
                if (hart->trap_pending) {
                        hart->trap_pending = 0;
                        hart->_pc = hart->pc_next;
                }
        }
}

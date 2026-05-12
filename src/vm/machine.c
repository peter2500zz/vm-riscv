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

void machine_init(Machine *machine, uint8_t *buffer, uint32_t size) {
        memcpy(machine->mem->data, buffer, size);

        // 设置程序计数器指向 RAM 的起始地址
        machine->harts[0]._pc = RAM_ADDR;
        // 设置栈顶指向 RAM 的末尾
        machine->harts[0]._regs[2] =
            (RAM_ADDR + machine->mem->size) & ~(uint32_t)0xf;
}

void machine_go(Machine *machine) {
        while (1) {
                hart_step(&machine->harts[0]);
        }
}

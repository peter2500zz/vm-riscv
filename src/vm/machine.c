#include "machine.h"
#include "hart/unprivileged.h"
#include <stdint.h>

Machine *machine_new(uint32_t hart_num, uint32_t mem_size) {
        // 分配 Machine 结构体
        Machine *machine = calloc(1, sizeof(Machine));
        if (machine == NULL) {
                fprintf(stderr,
                        "Failed to allocate memory for machine struct\n");
                return NULL;
        }

        // 分配内存
        machine->mem_size = mem_size;
        machine->mem = calloc(1, mem_size);
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
                if (hart_init(&machine->harts[i], machine->mem, mem_size) !=
                    0) {
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
        free(machine->mem);
}

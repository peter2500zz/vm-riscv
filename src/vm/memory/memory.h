//
// Created by Peter Shen on 2026/6/26.
//

#ifndef VM_RISCV_MEMORY_H
#define VM_RISCV_MEMORY_H

#include "../../utils/mmioMap.h"
#include "ram.h"
#include <stdint.h>

#define RAM_BASE 0x80000000

#define MAX_MEM_SIZE 0x40000000

typedef struct {
        uint32_t size;
        Ram *ram;
        MmioMap *mmioMap;
} Memory;

Memory *newMemory(uint32_t size);

void freeMemory(Memory **ppMem);

int readMemory(Memory *mem, uint32_t address, void *value, uint32_t size);

int writeMemory(Memory *mem, uint32_t address, void *value, uint32_t size);

#endif // VM_RISCV_MEMORY_H

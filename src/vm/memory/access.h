#ifndef MEMORY_ACCESS_H
#define MEMORY_ACCESS_H

#include "../cpu/hart/unprivileged.h"
#include "types.h"
#include <stdint.h>

Memory *memory_new(uint32_t size);

void memory_free(Memory *mem);

int memory_access(Hart *hart, uint32_t addr, void *target, uint32_t size,
                  MemAccessType type);

#define MEM_ACCESS(hart, addr, target, size, type)                             \
        do {                                                                   \
                memory_access(hart, addr, target, size, type);                  \
                if ((hart)->trap_pending)                                      \
                        return;                                                \
        } while (0)

#endif // MEMORY_ACCESS_H

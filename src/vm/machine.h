#ifndef MACHINE_H
#define MACHINE_H

#include "hart/unprivileged.h"
#include <stdint.h>

typedef struct {
        Hart *harts;
        uint32_t hart_num;
        uint8_t *mem;
        uint32_t mem_size;
} Machine;

Machine *machine_new(uint32_t hart_num, uint32_t mem_size);

void machine_free(Machine *machine);

#endif // MACHINE_H
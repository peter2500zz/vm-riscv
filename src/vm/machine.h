#ifndef MACHINE_H
#define MACHINE_H

#include "cpu/hart/unprivileged.h"
#include "memory/types.h"
#include <stdint.h>

typedef struct {
        Hart *harts;
        uint32_t hart_num;
        Memory *mem;
} Machine;

Machine *machine_new(uint32_t hart_num, uint32_t mem_size);

void machine_free(Machine *machine);

void machine_load_raw(Machine *machine, uint8_t *buffer, uint32_t size);

int machine_load_elf(Machine *machine, uint8_t *buffer, uint32_t size);

void machine_go(Machine *machine);

#endif // MACHINE_H
//
// Created by Peter Shen on 2026/6/26.
//

#ifndef VM_RISCV_RAM_H
#define VM_RISCV_RAM_H
#include <stdint.h>

typedef struct Ram Ram;

typedef int (*ramAccessFunc)(Ram *ram, uint32_t address, void *target);

struct Ram {
        uint8_t *data;
        uint32_t size;
        struct {
                ramAccessFunc read;
                ramAccessFunc write;
        } func[3];
};

Ram *newRam(uint32_t size);

void freeRam(Ram **ppRam);

#endif // VM_RISCV_RAM_H

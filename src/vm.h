#ifndef VM_H
#define VM_H

#include <stdint.h>

typedef struct {
        uint32_t pc;
        uint32_t regs[32];
        size_t mem_size;
        uint8_t *mem;
} VM;

VM *vm_new(size_t n);
void vm_free(VM *vm);
int vm_load(VM *vm, const char *filename);

#endif // VM_H

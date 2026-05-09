#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "instruction.h"
#include "vm.h"

VM *vm_new(int n) {
        uint32_t size = (uint32_t)(1 << n); // 2^n

        VM *vm = calloc(1, sizeof(VM));
        if (vm == NULL) {
                goto out;
        }
        vm->pc = 0;
        vm->mem_size = size;
        vm->mem = calloc(1, size);
        if (vm->mem == NULL) {
                goto out_free_vm;
        }

        return vm;

out_free_vm:
        free(vm);
out:
        return NULL;
}

void vm_free(VM *vm) {
        if (vm == NULL) {
                return;
        }
        free(vm->mem);
        free(vm);
}

int vm_load(VM *vm, const char *filename) {
        int result = 0;
        FILE *fp = fopen(filename, "rb");
        if (fp == NULL) {
                result = 1;
                goto out;
        }

        fseek(fp, 0, SEEK_END);
        size_t file_size = (size_t)ftell(fp);
        if (file_size < 0) {
                result = 1;
                goto out_close_file;
        }
        fseek(fp, 0, SEEK_SET);

        if ((uint32_t)file_size > vm->mem_size) {
                result = 1;
                goto out_close_file;
        }

        size_t bytes_read = fread(vm->mem, 1, file_size, fp);
        if (bytes_read != file_size) {
                result = 1;
                goto out_close_file;
        }

out_close_file:
        fclose(fp);
out:
        return result;
}

Instruction vm_fetch(VM *vm) {
        Instruction inst = vm->mem[(vm->pc) % vm->mem_size] |
                           (vm->mem[(vm->pc + 1) % vm->mem_size] << 8) |
                           (vm->mem[(vm->pc + 2) % vm->mem_size] << 16) |
                           (vm->mem[(vm->pc + 3) % vm->mem_size] << 24);

        // 循环地址
        vm->pc = (vm->pc + 4) % vm->mem_size;

        return inst;
}

void vm_step(VM *vm) {
        Instruction inst = vm_fetch(vm);
        printf("opcode: 0x%08X\n", inst_opcode(inst));
}

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
        uint32_t pc;
        uint32_t regs[32];
        size_t mem_size;
        uint8_t *mem;
} VM;


VM *vm_new(size_t n) {
        size_t size = 1 << n; // 2^n

        // 清空区域并分配内存
        VM *vm = calloc(1, sizeof(VM));
        if (vm == NULL) {
                printf("Failed to allocate memory for VM\n");
                goto out;
        }
        vm->pc = 0;
        vm->mem_size = size;
        vm->mem = malloc(size * sizeof(uint8_t));
        if (vm->mem == NULL) {
                printf("Failed to allocate memory for VM memory\n");
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
                printf("Failed to open file: %s\n", filename);
                result = 1;
                goto out;
        }

        size_t index = 0;
        uint8_t byte;
        while (fread(&byte, sizeof(uint8_t), 1, fp) == 1) {
                if (index >= vm->mem_size) {
                        printf("File is too large to fit in VM memory\n");
                        result = 1;
                        goto out_close_file;
                }
                vm->mem[index] = byte;
                index++;
        }

out_close_file:
        fclose(fp);
out:
        return result;
}

int main(int argc, char *argv[]) {
        int result = 0;

        // 参数解析
        if (argc != 2) {
                printf("Usage: %s <program.bin>\n", argv[0]);
                result = 1;
                goto out;
        }

        VM *vm = vm_new(20); // 1MiB memory
        if (vm == NULL) {
                result = 1;
                goto out;
        }

        if (vm_load(vm, argv[1]) != 0) {
                result = 1;
                goto out_free_vm;
        }

        printf("Doing something with the VM...\n");

out_free_vm:
        vm_free(vm);
out:
        return result;
}

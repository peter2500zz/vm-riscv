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
        size_t size = (size_t)1 << n; // 2^n

        VM *vm = calloc(1, sizeof(VM));
        if (vm == NULL) {
                fprintf(stderr, "Failed to allocate memory for VM\n");
                goto out;
        }
        vm->pc = 0;
        vm->mem_size = size;
        vm->mem = calloc(1, size);
        if (vm->mem == NULL) {
                fprintf(stderr, "Failed to allocate memory for VM memory\n");
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
                fprintf(stderr, "Failed to open file: %s\n", filename);
                result = 1;
                goto out;
        }

        fseek(fp, 0, SEEK_END);
        long file_size = ftell(fp);
        if (file_size < 0) {
                fprintf(stderr, "Failed to get file size\n");
                result = 1;
                goto out_close_file;
        }
        fseek(fp, 0, SEEK_SET);

        if ((size_t)file_size > vm->mem_size) {
                fprintf(
                        stderr, 
                        "File size (%ld bytes) exceeds VM memory size (%zu bytes)\n", 
                        file_size, vm->mem_size
                );
                result = 1;
                goto out_close_file;
        }

        size_t bytes_read = fread(
                vm->mem, 
                1, 
                (size_t)file_size, 
                fp
        );
        if (bytes_read != (size_t)file_size) {
                fprintf(stderr, "Failed to read file\n");
                result = 1;
                goto out_close_file;
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
        vm = NULL;
out:
        return result;
}

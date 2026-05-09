#include <stdint.h>
#include <stdio.h>

#include "vm.h"

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
                fprintf(stderr, "Failed to create VM\n");
                result = 1;
                goto out;
        }

        if (vm_load(vm, argv[1]) != 0) {
                fprintf(stderr, "Failed to load program into VM\n");
                result = 1;
                goto out_free_vm;
        }

        printf("Doing something with the VM...\n");
        vm_step(vm);

out_free_vm:
        vm_free(vm);
        vm = NULL;
out:
        return result;
}

#include "exec.h"

void handle_ecall(VM *vm) {
        int32_t syscall_num = (int32_t)vm_reg_read(vm, 17); // a7
        void *arg0 = (void *)(intptr_t)vm_reg_read(vm, 10); // a0
        void *arg1 = (void *)(intptr_t)vm_reg_read(vm, 11); // a1
        void *arg2 = (void *)(intptr_t)vm_reg_read(vm, 12); // a2
        // void *arg3 = (void *)(intptr_t)vm_reg_read(vm, 13); // a3
        // void *arg4 = (void *)(intptr_t)vm_reg_read(vm, 14); // a4
        // void *arg5 = (void *)(intptr_t)vm_reg_read(vm, 15); // a5

        switch (syscall_num) {
        // write(1, buf, len)
        case 64: {
                uint32_t buffer_addr = (uint32_t)(intptr_t)arg1;
                uint32_t buffer_len = (uint32_t)(intptr_t)arg2;

                for (uint32_t i = 0; i < buffer_len; i++) {
                        uint8_t byte = *vm_mem_ptr_byte(vm, buffer_addr + i);
                        putchar(byte);
                }

                break;
        }

        case 93: // exit
                printf("Program exited with code %d\n", (int)(intptr_t)arg0);
                exit((int)(intptr_t)arg0);

        default:
                fprintf(stderr, "Unknown syscall: %d\n", syscall_num);
                getchar();
                while (getchar() != '\n')
                        ;
        }
}

#include "exec.h"

void handle_ecall(Hart *hart) {
        int32_t syscall_num = (int32_t)hart_reg_read(hart, 17); // a7
        void *arg0 = (void *)(intptr_t)hart_reg_read(hart, 10); // a0
        void *arg1 = (void *)(intptr_t)hart_reg_read(hart, 11); // a1
        void *arg2 = (void *)(intptr_t)hart_reg_read(hart, 12); // a2
        // void *arg3 = (void *)(intptr_t)hart_reg_read(hart, 13); // a3
        // void *arg4 = (void *)(intptr_t)hart_reg_read(hart, 14); // a4
        // void *arg5 = (void *)(intptr_t)hart_reg_read(hart, 15); // a5

        switch (syscall_num) {
        // write(1, buf, len)
        case 64: {
                uint32_t buffer_addr = (uint32_t)(intptr_t)arg1;
                uint32_t buffer_len = (uint32_t)(intptr_t)arg2;

                for (uint32_t i = 0; i < buffer_len; i++) {
                        uint8_t byte = *hart_mem_ptr_byte(hart, buffer_addr + i);
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

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "vm.h"

/**
 * @brief 将给定路径的文件读取入缓冲区
 *
 * @param filename 文件路径
 * @param buffer_size 用于存储缓冲区大小的指针
 * @return 缓冲区指针
 *
 * @note 缓冲区指针在生命周期结束时需要 free
 */
uint8_t *load_file_to_buffer(const char *filename, uint32_t *buffer_size) {
        uint8_t *result = NULL;

        FILE *fp = fopen(filename, "rb");
        if (fp == NULL) {
                goto out;
        }

        fseek(fp, 0, SEEK_END);
        long file_size = ftell(fp);
        if (file_size < 0 || file_size > (long)INT32_MAX) {
                goto out_close_file;
        }
        fseek(fp, 0, SEEK_SET);

        uint8_t *buffer = malloc((uint32_t)file_size);
        if (buffer == NULL) {
                goto out_close_file;
        }
        size_t bytes_read = fread(buffer, 1, (size_t)file_size, fp);
        if ((uint32_t)file_size != (uint32_t)bytes_read) {
                free(buffer);
        } else {
                *buffer_size = (uint32_t)file_size;
                result = buffer;
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

        // 初始化虚拟机
        VM *vm = vm_new(20); // 1MiB memory
        if (vm == NULL) {
                fprintf(stderr, "Failed to create VM\n");
                result = 1;
                goto out;
        }

        // 读取文件到缓冲区
        uint32_t buffer_size = 0;
        uint8_t *buffer = load_file_to_buffer(argv[1], &buffer_size);
        if (buffer == NULL) {
                fprintf(stderr, "Failed to load file: %s\n", argv[1]);
                result = 1;
                goto out_free_vm;
        }

        // 读取缓冲区到虚拟机内存
        int load_failed = vm_load(vm, 0, buffer, buffer_size) != 0;
        free(buffer);
        if (load_failed) {
                fprintf(stderr, "Failed to load file data into VM memory\n");
                result = 1;
                goto out_free_vm;
        }

        printf("Doing something with the VM...\n");

        // 主循环
        while (1) {
                int input = getchar();
                while (getchar() != '\n')
                        ;

                switch (input) {
                case 'q':
                        goto out_free_vm;
                case 'd':
                        vm_debug(vm);
                        break;
                case 's':
                        vm_step(vm);
                        break;
                default:
                        printf("Unknown command: %c\n", input);
                        break;
                }
        }

        // 清理
out_free_vm:
        vm_free(vm);
        vm = NULL;
out:
        return result;
}

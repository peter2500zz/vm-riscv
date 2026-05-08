#include <stdio.h>
#include <stdlib.h>

#include "vm.h"

/**
 * @brief 创建一个新的虚拟机实例
 *
 * @param n 内存大小的指数，实际内存大小为2^n字节
 *
 * @return 成功返回指向VM实例的指针，失败返回NULL
 */
VM *vm_new(size_t n) {
        size_t size = (size_t)1 << n; // 2^n

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

/**
 * @brief 释放虚拟机实例
 * @param vm 要释放的VM实例指针
 *
 * @return 无返回值
 *
 * @note 无论vm是否为NULL，函数都能安全调用
 */
void vm_free(VM *vm) {
        if (vm == NULL) {
                return;
        }
        free(vm->mem);
        free(vm);
}

/**
 * @brief 从文件加载数据到虚拟机内存
 * @param vm 虚拟机实例指针
 * @param filename 文件路径
 *
 * @return: 成功返回0，失败返回1
 *
 * @warning 文件大小如果超过虚拟机内存大小，将失败
 */
int vm_load(VM *vm, const char *filename) {
        int result = 0;
        FILE *fp = fopen(filename, "rb");
        if (fp == NULL) {
                result = 1;
                goto out;
        }

        fseek(fp, 0, SEEK_END);
        long file_size = ftell(fp);
        if (file_size < 0) {
                result = 1;
                goto out_close_file;
        }
        fseek(fp, 0, SEEK_SET);

        if ((size_t)file_size > vm->mem_size) {
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
                result = 1;
                goto out_close_file;
        }

out_close_file:
        fclose(fp);
out:
        return result;
}

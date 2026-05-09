#ifndef VM_H
#define VM_H

#include <stdint.h>
#include <stdlib.h>

#include "instruction.h"

typedef struct {
        uint32_t pc;
        uint32_t regs[32];
        uint32_t mem_size;
        uint8_t *mem;
} VM;

/**
 * @brief 创建一个新的虚拟机实例
 * @param n 内存大小的指数，实际内存大小为2^n字节
 * @return 成功返回指向VM实例的指针，失败返回NULL
 */
VM *vm_new(int n);

/**
 * @brief 释放虚拟机实例
 * @param vm 要释放的VM实例指针
 * @return 无返回值
 * @note 无论vm是否为NULL，函数都能安全调用
 */
void vm_free(VM *vm);

/**
 * @brief 从文件加载数据到虚拟机内存
 * @param vm 虚拟机实例指针
 * @param filename 文件路径
 * @return: 成功返回0，失败返回1
 * @warning 文件大小如果超过虚拟机内存大小，将失败
 */
int vm_load(VM *vm, const char *filename);

/**
 * @brief 从虚拟机内存中获取PC指向的一条指令
 * @param vm 虚拟机实例指针
 * @return 从内存中获取的指令
 * @note PC自动增加4字节
 */
Instruction vm_fetch(VM *vm);

void vm_step(VM *vm);

#endif // VM_H

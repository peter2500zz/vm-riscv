#ifndef VM_H
#define VM_H

#include <stdint.h>

#include "instruction.h"

#define VM_REG_NUM 32

typedef struct {
        uint32_t pc;
        uint32_t pc_next;
        uint32_t _regs[VM_REG_NUM];
        uint32_t mem_size;
        uint8_t *mem;
} VM;

/**
 * @brief 创建一个新的虚拟机实例
 *
 * @param n 内存大小的指数，实际内存大小为2^n字节
 * @return 成功返回指向VM实例的指针，失败返回NULL
 */
VM *vm_new(int n);

/**
 * @brief 释放虚拟机实例
 *
 * @param vm 要释放的VM实例指针
 * @return 无返回值
 *
 * @note 无论vm是否为NULL，函数都能安全调用
 */
void vm_free(VM *vm);

/**
 * @brief 将当前虚拟机状态打印到标准输出
 *
 * @param vm 虚拟机实例指针
 */
void vm_debug(VM *vm);

/**
 * @brief 从指定寄存器中读取值
 *
 * @param vm 虚拟机实例指针
 * @param reg_num 寄存器编号
 * @return 寄存器中的值
 */
static inline uint32_t vm_reg_read(VM *vm, uint32_t reg_num) {
        // x0寄存器始终为0
        if (reg_num == 0) {
                return 0;
        }
        return vm->_regs[reg_num];
}

/**
 * @brief 将值写入指定寄存器
 *
 * @param vm 虚拟机实例指针
 * @param reg_num 寄存器编号
 * @param value 写入寄存器的值
 */
static inline void vm_reg_write(VM *vm, uint32_t reg_num, uint32_t value) {
        // x0寄存器始终为0
        if (reg_num == 0) {
                return;
        }
        vm->_regs[reg_num] = value;
}

/**
 * @brief 从文件加载数据到虚拟机内存
 *
 * @param vm 虚拟机实例指针
 * @param filename 文件路径
 * @return: 成功返回0，失败返回1
 *
 * @warning 文件大小如果超过虚拟机内存大小，将失败
 */
int vm_load(VM *vm, uint32_t offset, uint8_t *buffer, uint32_t size);

/**
 * @brief 从虚拟机内存中获取PC指向的指令
 *
 * @param vm 虚拟机实例指针
 * @return 从内存中获取的指令
 */
Instruction vm_fetch(VM *vm);

/**
 * @brief 执行pc指向的指令
 *
 * @param vm 虚拟机实例指针
 */
void vm_step(VM *vm);

#endif // VM_H

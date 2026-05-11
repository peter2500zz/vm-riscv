#ifndef VM_H
#define VM_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#define VM_REG_NUM 32
#define VM_CSR_NUM 4096

typedef struct {
        /**
         * @brief 应当使用 vm_pc_read 和 vm_pc_write 来访问 _pc
         *
         */
        uint32_t _pc;
        uint32_t pc_next;
        /**
         * @brief 应当使用 vm_reg_read 和 vm_reg_write 来访问 _regs
         *
         */
        uint32_t _regs[VM_REG_NUM];
        uint32_t mem_size;
        /**
         * @brief 应当使用 vm_mem_ptr_byte/half/word 来访问 _mem
         *
         */
        uint8_t *_mem;
        /**
         * @brief CSR 寄存器
         * 
         */
        uint32_t _csr[VM_CSR_NUM];
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
 * @brief 读取PC寄存器的值
 *
 * @param vm 虚拟机实例指针
 * @return PC寄存器的值
 */
static inline uint32_t vm_pc_read(VM *vm) { return vm->_pc; }

/**
 * @brief 将值写入PC寄存器
 *
 * @param vm 虚拟机实例指针
 * @param value 写入PC寄存器的值
 *
 * @note 自动根据虚拟机实例内存大小环绕
 */
static inline void vm_pc_write(VM *vm, uint32_t value) {
        if (value >= vm->mem_size) {
                fprintf(stderr,
                        "PC value 0x%08X out of memory bounds (mem_size: %d)\n",
                        value, vm->mem_size);
                exit(1);
        }
        vm->_pc = value;
}

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
 * @brief 从虚拟机内存中取一个字节指针
 *
 * @param vm 虚拟机实例指针
 * @param addr 起始地址
 * @return 起始地址处的字节指针
 */
static inline uint8_t *vm_mem_ptr_byte(VM *vm, uint32_t addr) {
        if (addr >= vm->mem_size) {
                fprintf(stderr,
                        "Memory access out of bounds: 0x%08X (mem_size: 0x%08X)\n",
                        addr, vm->mem_size);
                exit(1);
        }
        return &vm->_mem[addr];
}

/**
 * @brief 从虚拟机内存中取一个半字指针
 *
 * @param vm 虚拟机实例指针
 * @param addr 起始地址
 * @return 起始地址处的半字指针
 */
static inline uint16_t *vm_mem_ptr_half(VM *vm, uint32_t addr) {
        // if (addr & 1) {
        //         fprintf(stderr, "Unaligned half access: 0x%08X\n", addr);
        //         exit(1);
        // }
        if (addr + 1 >= vm->mem_size) {
                fprintf(stderr,
                        "Memory access out of bounds: 0x%08X (mem_size: 0x%08X)\n",
                        addr, vm->mem_size);
                exit(1);
        }
        return (uint16_t *)&vm->_mem[addr];
}

/**
 * @brief 从虚拟机内存中取一个字指针
 *
 * @param vm 虚拟机实例指针
 * @param addr 起始地址
 * @return 起始地址处的字指针
 */
static inline uint32_t *vm_mem_ptr_word(VM *vm, uint32_t addr) {
        // if (addr & 3) {
        //         fprintf(stderr, "Unaligned word access: 0x%08X\n", addr);
        //         exit(1);
        // }
        if (addr + 3 >= vm->mem_size) {
                fprintf(stderr,
                        "Memory access out of bounds: 0x%08X (mem_size: 0x%08X)\n",
                        addr, vm->mem_size);
                exit(1);
        }
        return (uint32_t *)&vm->_mem[addr];
}

/**
 * @brief 将数据从缓冲区直接拷贝到虚拟机内存
 *
 * @param vm 虚拟机实例指针
 * @param offset 虚拟机内存中的偏移地址
 * @param buffer 数据缓冲区指针
 * @param size 数据大小
 * @return: 成功返回0，失败返回1
 *
 * @warning 数据大小如果超过虚拟机内存大小，将失败
 */
int vm_load(VM *vm, uint32_t offset, uint8_t *buffer, uint32_t size);

int vm_load_elf(VM *vm, uint8_t *buffer, uint32_t size);

/**
 * @brief 执行pc指向的指令
 *
 * @param vm 虚拟机实例指针
 */
void vm_step(VM *vm);

#endif // VM_H

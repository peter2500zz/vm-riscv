#ifndef HART_UNPRIVILEGED_H
#define HART_UNPRIVILEGED_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "unprivileged/inst.h"

#define HART_REG_NUM 32
#define HART_CSR_NUM 4096

/**
 * @brief 硬件线程结构体
 *
 * @note 内部成员前加下划线表示不应直接访问，应使用提供的函数接口。
 * 同时，内存生命周期不应由 Hart 负责管理，应由外部提供并保证其有效性。
 */
typedef struct {
        /**
         * @brief 应当使用 hart_pc_read 和 hart_pc_write 来访问 _pc
         *
         */
        uint32_t _pc;
        uint32_t pc_next;
        /**
         * @brief 应当使用 hart_reg_read 和 hart_reg_write 来访问 _regs
         *
         */
        uint32_t _regs[HART_REG_NUM];
        uint32_t mem_size;
        /**
         * @brief 应当使用 hart_mem_ptr_byte/half/word 来访问 _mem
         *
         */
        uint8_t *_mem;

        // 下为特权模式

        /**
         * @brief CSR 寄存器
         *
         */
        uint32_t _csr[HART_CSR_NUM];
        char trap_pending;
} Hart;

/**
 * @brief 初始化硬件线程
 *
 * @param hart 硬件线程指针
 * @param mem 内存指针
 * @param mem_size 内存大小
 * @return 初始化结果，0表示成功，非0表示失败
 */
int hart_init(Hart *hart, uint8_t *mem, uint32_t mem_size);

/**
 * @brief 将当前虚拟机状态打印到标准输出
 *
 * @param hart 硬件线程指针
 */
void hart_debug(Hart *hart);

/**
 * @brief 读取PC寄存器的值
 *
 * @param hart 硬件线程指针
 * @return PC寄存器的值
 */
static inline uint32_t hart_pc_read(Hart *hart) { return hart->_pc; }

/**
 * @brief 将值写入PC寄存器
 *
 * @param hart 硬件线程指针
 * @param value 写入PC寄存器的值
 *
 * @note 自动根据硬件线程内存大小环绕
 */
static inline void hart_pc_write(Hart *hart, uint32_t value) {
        if (value >= hart->mem_size) {
                fprintf(stderr,
                        "PC value 0x%08X out of memory bounds (mem_size: %d)\n",
                        value, hart->mem_size);
                exit(1);
        }
        hart->_pc = value;
}

/**
 * @brief 从指定寄存器中读取值
 *
 * @param hart 硬件线程指针
 * @param reg_num 寄存器编号
 * @return 寄存器中的值
 */
static inline uint32_t hart_reg_read(Hart *hart, uint32_t reg_num) {
        // x0寄存器始终为0
        if (reg_num == 0) {
                return 0;
        }
        return hart->_regs[reg_num];
}

/**
 * @brief 将值写入指定寄存器
 *
 * @param hart 硬件线程指针
 * @param reg_num 寄存器编号
 * @param value 写入寄存器的值
 */
static inline void hart_reg_write(Hart *hart, uint32_t reg_num,
                                  uint32_t value) {
        // x0寄存器始终为0
        if (reg_num == 0) {
                return;
        }
        hart->_regs[reg_num] = value;
}

/**
 * @brief 从虚拟机内存中取一个字节指针
 *
 * @param hart 硬件线程指针
 * @param addr 起始地址
 * @return 起始地址处的字节指针
 */
static inline uint8_t *hart_mem_ptr_byte(Hart *hart, uint32_t addr) {
        if (addr >= hart->mem_size) {
                fprintf(
                    stderr,
                    "Memory access out of bounds: 0x%08X (mem_size: 0x%08X)\n",
                    addr, hart->mem_size);
                exit(1);
        }
        return &hart->_mem[addr];
}

/**
 * @brief 从虚拟机内存中取一个半字指针
 *
 * @param hart 硬件线程指针
 * @param addr 起始地址
 * @return 起始地址处的半字指针
 */
static inline uint16_t *hart_mem_ptr_half(Hart *hart, uint32_t addr) {
        // if (addr & 1) {
        //         fprintf(stderr, "Unaligned half access: 0x%08X\n", addr);
        //         exit(1);
        // }
        if (addr + 1 >= hart->mem_size) {
                fprintf(
                    stderr,
                    "Memory access out of bounds: 0x%08X (mem_size: 0x%08X)\n",
                    addr, hart->mem_size);
                exit(1);
        }
        return (uint16_t *)&hart->_mem[addr];
}

/**
 * @brief 从虚拟机内存中取一个字指针
 *
 * @param hart 硬件线程指针
 * @param addr 起始地址
 * @return 起始地址处的字指针
 */
static inline uint32_t *hart_mem_ptr_word(Hart *hart, uint32_t addr) {
        // if (addr & 3) {
        //         fprintf(stderr, "Unaligned word access: 0x%08X\n", addr);
        //         exit(1);
        // }
        if (addr + 3 >= hart->mem_size) {
                fprintf(
                    stderr,
                    "Memory access out of bounds: 0x%08X (mem_size: 0x%08X)\n",
                    addr, hart->mem_size);
                exit(1);
        }
        return (uint32_t *)&hart->_mem[addr];
}

/**
 * @brief 将数据从缓冲区直接拷贝到虚拟机内存
 *
 * @param hart 硬件线程指针
 * @param offset 虚拟机内存中的偏移地址
 * @param buffer 数据缓冲区指针
 * @param size 数据大小
 * @return: 成功返回0，失败返回1
 *
 * @warning 数据大小如果超过虚拟机内存大小，将失败
 */
int hart_load(Hart *hart, uint32_t offset, uint8_t *buffer, uint32_t size);

int hart_load_elf(Hart *hart, uint8_t *buffer, uint32_t size);

/**
 * @brief 从虚拟机内存中获取PC指向的指令
 *
 * @param hart 硬件线程指针
 * @return 从内存中获取的指令
 */
Instruction hart_fetch(Hart *hart);

/**
 * @brief 执行pc指向的指令
 *
 * @param hart 硬件线程指针
 */
void hart_step(Hart *hart);

#endif // HART_UNPRIVILEGED_H

//
// Created by Peter Shen on 2026/6/26.
//

#ifndef VM_RISCV_MEMORY_H
#define VM_RISCV_MEMORY_H

#include "../../utils/mmioMap.h"
#include "ram.h"
#include <stdint.h>

#define RAM_BASE 0x80000000

#define MAX_MEM_SIZE 0x40000000

typedef struct {
        uint32_t size;
        Ram *ram;
        MmioMap *mmioMap;
} Memory;

/**
 * @brief 分配 Memory 实例
 *
 * @param size RAM 内存大小，单位是字节
 * @return Memory 实例指针，如果分配失败则为 NULL
 */
Memory *newMemory(uint32_t size);

/**
 * @brief 释放模拟内存
 *
 * @param ppMem 指向 Memory 实例的指针
 */
void freeMemory(Memory **ppMem);

/**
 * @brief 对 Memory 进行读操作
 *
 * @param mem Memory 实例指针
 * @param address 操作地址
 * @param value 操作数指针
 * @param size 操作数字节数
 * @return 非 0 为错误
 *
 * @note 会自动进行 MMIO 地址映射
 */
int readMemory(Memory *mem, uint32_t address, void *value, uint32_t size);

/**
 * @brief 对 Memory 进行写操作
 *
 * @param mem Memory 实例指针
 * @param address 操作地址
 * @param value 操作数指针
 * @param size 操作数字节数
 * @return 非 0 为错误
 *
 * @note 会自动进行 MMIO 地址映射
 */
int writeMemory(Memory *mem, uint32_t address, void *value, uint32_t size);

#endif // VM_RISCV_MEMORY_H

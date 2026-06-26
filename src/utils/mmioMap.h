//
// Created by Peter Shen on 2026/6/25.
//

#ifndef VM_RISCV_MMIOMAP_H
#define VM_RISCV_MMIOMAP_H
#include <stdint.h>

/**
 * @brief MMIO 操作函数定义
 *
 * 对于读函数，读取到的值将会被写入传入指针中
 *
 * 对于写函数，传入指针中的值将会被写入目标
 */
typedef int (*accessFunc)(void *);

/**
 * @brief MMIO 地址单元
 */
typedef struct MmioNode {
        uint32_t address;
        struct {
                accessFunc read;
                accessFunc write;
        } func;
        struct MmioNode *next;
} MmioNode;

/**
 * @brief MMIO 映射表，通过地址作为键来获取对应函数
 *
 * @note 不同宽度的操作应当使用多个映射表
 */
typedef struct MmioMap {
        uint32_t capacity;
        int nodeCount;
        MmioNode **buckets;
} MmioMap;

/**
 * @brief 创建 MmioMap
 *
 * @return 如果内存分配成功，返回 MmioMap *
 *
 * 否则，返回 NULL
 */
MmioMap *newMmioMap(void);

/**
 * @brief 释放一个 MmioMap 和其中所有的值，并将指针置为 NULL
 *
 * @param pMmioMap 指向 MmioMap 实例的指针
 */
void freeMmioMap(MmioMap **pMmioMap);

/**
 * @brief 将一个 MmioMap 扩容到当前的两倍大小
 *
 * @param mmioMap MmioMap 实例
 * @return 非 0 为扩容失败
 */
int extendMmioMap(MmioMap *mmioMap);

/**
 * @brief 向 MmioMap 中添加读写函数
 *
 * @param mmioMap MmioMap 实例
 * @param address MMIO 地址
 * @param read 读函数
 * @param write 写函数
 * @return 非 0 为添加失败
 *
 * @note 如果地址已存在，将覆盖原先值
 */
int putMmioMap(MmioMap *mmioMap, uint32_t address, accessFunc read, accessFunc write);

/**
 * @brief 获取读函数
 *
 * @param mmioMap MmioMap 实例
 * @param address MMIO 地址
 * @return 读函数，不存在或获取失败时为 NULL
 */
accessFunc getReadFuncMmioMap(const MmioMap *mmioMap, uint32_t address);

/**
 * @brief 获取写函数
 *
 * @param mmioMap MmioMap 实例
 * @param address MMIO 地址
 * @return 读函数，不存在或获取失败时为 NULL
 */
accessFunc getWriteFuncMmioMap(const MmioMap *mmioMap, uint32_t address);

#endif // VM_RISCV_MMIOMAP_H

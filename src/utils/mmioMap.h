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
        uint32_t nodeCount;
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
 * @brief 将读函数添加进地址对应位置
 *
 * @param mmioMap MmioMap 实例
 * @param address MMIO 地址
 * @param readFunc 读函数指针
 * @return 非 0 为添加失败
 *
 * @note readFunc 为 NULL 可清空已有函数
 */
int putReadFuncMmioMap(MmioMap *mmioMap, uint32_t address, accessFunc readFunc);

/**
 * @brief 将写函数添加进地址对应位置
 *
 * @param mmioMap MmioMap 实例
 * @param address MMIO 地址
 * @param writeFunc 写函数指针
 * @return 非 0 为添加失败
 *
 * @note writeFunc 为 NULL 可清空已有函数
 */
int putWriteFuncMmioMap(MmioMap *mmioMap, uint32_t address, accessFunc writeFunc);

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

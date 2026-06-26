//
// Created by Peter Shen on 2026/6/25.
//

#include "mmioMap.h"

#include <stdlib.h>

static inline uint32_t hash_u32(uint32_t x) {
        x ^= x >> 16;
        x *= 0x7feb352du;
        x ^= x >> 15;
        x *= 0x846ca68bu;
        x ^= x >> 16;
        return x;
}

MmioMap *newMmioMap(void) {
        MmioMap *mmioMap = malloc(sizeof(MmioMap));
        if (mmioMap == NULL) {
                goto out;
        }
        // 初始化
        mmioMap->capacity = 256;
        mmioMap->buckets = calloc(mmioMap->capacity, sizeof(*mmioMap->buckets));
        if (mmioMap->buckets == NULL) {
                goto out_free_mmiomap;
        }
        mmioMap->nodeCount = 0;

        return mmioMap;

out_free_mmiomap:
        free(mmioMap);
out:
        return NULL;
}

void freeMmioMap(MmioMap **pMmioMap) {
        // 检查指针是否为 NULL
        if (pMmioMap == NULL) {
                goto out;
        }
        MmioMap *mmioMap = *pMmioMap;

        // 检查 MmioMap 是否是 NULL
        if (mmioMap == NULL) {
                goto out_clear;
        }

        // 检查桶是否为空
        if (mmioMap->buckets == NULL) {
                goto out_free_map;
        }

        // 遍历释放桶
        for (uint32_t i = 0; i < mmioMap->capacity; i++) {
                MmioNode *node = mmioMap->buckets[i];

                // 遍历释放节点
                while (node != NULL) {
                        MmioNode *next = node->next;
                        free(node);
                        node = next;
                }
        }
        free(mmioMap->buckets);

out_free_map:
        free(mmioMap);
out_clear:
        *pMmioMap = NULL;
out:;
}

int extendMmioMap(MmioMap *mmioMap) {
        if (mmioMap == NULL) {
                return 1;
        }

        // 保存当前 capacity 和桶
        const uint32_t oldCapacity = mmioMap->capacity;
        MmioNode **oldBuckets = mmioMap->buckets;

        // 创建新桶
        const uint32_t newCapacity = oldCapacity * 2;
        MmioNode **newBuckets = calloc(newCapacity, sizeof(*newBuckets));
        if (newBuckets == NULL) {
                return 1;
        }

        // 遍历旧桶
        for (uint32_t i = 0; i < oldCapacity; i++) {
                MmioNode *node = oldBuckets[i];

                // 遍历节点，rehash 并迁移内容
                while (node != NULL) {
                        MmioNode *next = node->next;

                        const uint32_t index =
                            hash_u32(node->address) & (newCapacity - 1);
                        node->next = newBuckets[index];
                        newBuckets[index] = node;

                        node = next;
                }
        }

        // 释放旧桶
        free(oldBuckets);
        mmioMap->buckets = newBuckets;
        mmioMap->capacity = newCapacity;
        return 0;
}

int putMmioMap(MmioMap *mmioMap, const uint32_t address, const accessFunc read,
               const accessFunc write) {
        if (mmioMap == NULL) {
                return 1;
        }

        // 计算 hash 并找到桶
        const uint32_t index = hash_u32(address) & (mmioMap->capacity - 1);
        MmioNode *node = mmioMap->buckets[index];

        // 遍历节点判断是否有重复地址
        while (node != NULL) {
                // 重复地址直接替换并返回
                if (node->address == address) {
                        node->func.read = read;
                        node->func.write = write;
                        return 0;
                }
                node = node->next;
        }

        // 分配新节点
        MmioNode *newNode = malloc(sizeof(MmioNode));
        if (newNode == NULL) {
                return 1;
        }
        // 初始化新节点
        newNode->address = address;
        newNode->func.read = read;
        newNode->func.write = write;

        // 插入新节点
        newNode->next = mmioMap->buckets[index];
        mmioMap->buckets[index] = newNode;
        mmioMap->nodeCount++;

        // 计算负载因子
        const float loadFactor =
            (float)mmioMap->nodeCount / (float)mmioMap->capacity;

        if (loadFactor > 0.75f) {
                // 扩容桶
                if (extendMmioMap(mmioMap)) {
                        // Failed to extend, what is happening?
                }
        }

        return 0;
}

static accessFunc getMmioMap(const MmioMap *mmioMap, const uint32_t address,
                             const int isReadFunc) {
        accessFunc func = NULL;

        if (mmioMap == NULL) {
                return func;
        }

        // 计算 hash 并获取桶
        const uint32_t index = hash_u32(address) & (mmioMap->capacity - 1);
        const MmioNode *node = mmioMap->buckets[index];

        // 遍历节点寻找值
        while (node != NULL) {
                if (node->address == address) {
                        if (isReadFunc) {
                                func = node->func.read;
                        } else {
                                func = node->func.write;
                        }
                        break;
                }
                node = node->next;
        }

        return func;
}

accessFunc getReadFuncMmioMap(const MmioMap *mmioMap, const uint32_t address) {
        return getMmioMap(mmioMap, address, 1);
}

accessFunc getWriteFuncMmioMap(const MmioMap *mmioMap, const uint32_t address) {
        return getMmioMap(mmioMap, address, 0);
}

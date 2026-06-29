//
// Created by Peter Shen on 2026/6/25.
//

#include "mmioMap.h"

#include <stdlib.h>
#include <unistd.h>

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

void freeMmioMap(MmioMap **ppMmioMap) {
        // 检查指针是否为 NULL
        if (ppMmioMap == NULL) {
                goto out;
        }
        MmioMap *pMmioMap = *ppMmioMap;

        // 检查 MmioMap 是否是 NULL
        if (pMmioMap == NULL) {
                goto out_clear;
        }

        // 检查桶是否为空
        if (pMmioMap->buckets == NULL) {
                goto out_free_map;
        }

        // 遍历释放桶
        for (uint32_t i = 0; i < pMmioMap->capacity; i++) {
                MmioNode *node = pMmioMap->buckets[i];

                // 遍历释放节点
                while (node != NULL) {
                        MmioNode *next = node->next;
                        free(node);
                        node = next;
                }
        }
        free(pMmioMap->buckets);

out_free_map:
        free(pMmioMap);
out_clear:
        *ppMmioMap = NULL;
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

static int putMmioMap(MmioMap *mmioMap, const uint32_t address,
                      const mmioAccessFunc func, const int isReadFunc,
                      const uint32_t size) {
        if (mmioMap == NULL) {
                return 1;
        }

        const int funcIndex = __builtin_ctz(size);

        // 计算 hash 并找到桶
        const uint32_t index = hash_u32(address) & (mmioMap->capacity - 1);
        MmioNode *node = mmioMap->buckets[index];

        // 遍历节点判断是否有重复地址
        while (node != NULL) {
                // 重复地址直接替换
                if (node->address == address) {
                        if (isReadFunc) {
                                node->func[funcIndex].read = func;
                        } else {
                                node->func[funcIndex].write = func;
                        }
                        // TODO: 删除双 NULL 节点
                        return 0;
                }
                node = node->next;
        }

        // 如果 func 是 NULL，则没有必要分配
        if (func == NULL) {
                return 0;
        }

        // 分配新节点
        MmioNode *newNode = calloc(1, sizeof(MmioNode));
        if (newNode == NULL) {
                return 1;
        }
        // 初始化新节点
        newNode->address = address;
        if (isReadFunc) {
                newNode->func[funcIndex].read = func;
        } else {
                newNode->func[funcIndex].write = func;
        }

        // 插入新节点
        newNode->next = mmioMap->buckets[index];
        mmioMap->buckets[index] = newNode;
        mmioMap->nodeCount++;

        // 计算负载因子，并在 > 0.75 时扩容桶
        if (mmioMap->nodeCount * 4 > mmioMap->capacity * 3) {
                if (extendMmioMap(mmioMap)) {
                        // Failed to extend, what is happening?
                }
        }

        return 0;
}

int putReadFuncMmioMap(MmioMap *mmioMap, const uint32_t address,
                       const mmioAccessFunc readFunc, const uint32_t size) {
        return putMmioMap(mmioMap, address, readFunc, 1, size);
}

int putWriteFuncMmioMap(MmioMap *mmioMap, const uint32_t address,
                        const mmioAccessFunc writeFunc, const uint32_t size) {
        return putMmioMap(mmioMap, address, writeFunc, 0, size);
}

static mmioAccessFunc getMmioMap(const MmioMap *mmioMap, const uint32_t address,
                             const int isReadFunc, const uint32_t size) {
        mmioAccessFunc func = NULL;
        const int funcIndex = __builtin_ctz(size);

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
                                func = node->func[funcIndex].read;
                        } else {
                                func = node->func[funcIndex].write;
                        }
                        break;
                }
                node = node->next;
        }

        return func;
}

mmioAccessFunc getReadFuncMmioMap(const MmioMap *mmioMap, const uint32_t address,
                              const uint32_t size) {
        return getMmioMap(mmioMap, address, 1, size);
}

mmioAccessFunc getWriteFuncMmioMap(const MmioMap *mmioMap, const uint32_t address,
                               const uint32_t size) {
        return getMmioMap(mmioMap, address, 0, size);
}

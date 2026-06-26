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
        mmioMap->capacity = 256;
        mmioMap->buckets = calloc(mmioMap->capacity, sizeof(*mmioMap->buckets));
        mmioMap->nodeCount = 0;
        if (mmioMap->buckets == NULL) {
                goto out_free_mmiomap;
        }

        return mmioMap;

out_free_mmiomap:
        free(mmioMap);
out:
        return NULL;
}

void freeMmioMap(MmioMap *mmioMap) {
        if (mmioMap == NULL) {
                goto out;
        }

        if (mmioMap->buckets == NULL) {
                goto out_free_mmiomap;
        }

        for (uint32_t i = 0; i < mmioMap->capacity; i++) {
                MmioNode *node = mmioMap->buckets[i];

                while (node != NULL) {
                        MmioNode *next = node->next;
                        free(node);
                        node = next;
                }
        }
        free(mmioMap->buckets);

out_free_mmiomap:
        free(mmioMap);
out:;
}

int extendMmioMap(MmioMap *mmioMap) {
        if (mmioMap == NULL) {
                return 1;
        }

        const uint32_t oldCapacity = mmioMap->capacity;
        MmioNode **oldBuckets = mmioMap->buckets;

        const uint32_t newCapacity = oldCapacity * 2;
        MmioNode **newBuckets = calloc(newCapacity, sizeof(*newBuckets));
        if (newBuckets == NULL) {
                return 1;
        }

        for (uint32_t i = 0; i < oldCapacity; i++) {
                MmioNode *node = oldBuckets[i];
                while (node != NULL) {
                        MmioNode *next = node->next;

                        const uint32_t index =
                            hash_u32(node->address) & (newCapacity - 1);
                        node->next = newBuckets[index];
                        newBuckets[index] = node;

                        node = next;
                }
        }

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

        const uint32_t index = hash_u32(address) & (mmioMap->capacity - 1);
        MmioNode *node = mmioMap->buckets[index];

        while (node != NULL) {
                if (node->address == address) {
                        node->func.read = read;
                        node->func.write = write;
                        return 0;
                }
                node = node->next;
        }

        MmioNode *newNode = malloc(sizeof(MmioNode));
        if (newNode == NULL) {
                return 1;
        }
        newNode->address = address;
        newNode->func.read = read;
        newNode->func.write = write;

        newNode->next = mmioMap->buckets[index];
        mmioMap->buckets[index] = newNode;
        mmioMap->nodeCount++;

        const float loadFactor =
            (float)mmioMap->nodeCount / (float)mmioMap->capacity;

        if (loadFactor > 0.75f) {
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

        const uint32_t index = hash_u32(address) & (mmioMap->capacity - 1);
        const MmioNode *node = mmioMap->buckets[index];

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

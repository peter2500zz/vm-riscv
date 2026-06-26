//
// Created by Peter Shen on 2026/6/25.
//

#ifndef VM_RISCV_MMIOMAP_H
#define VM_RISCV_MMIOMAP_H
#include <stdint.h>

typedef int (*accessFunc)(void *);

typedef struct MmioNode {
        uint32_t address;
        struct {
                accessFunc read;
                accessFunc write;
        } func;
        struct MmioNode *next;
} MmioNode;

typedef struct MmioMap {
        uint32_t capacity;
        int nodeCount;
        MmioNode **buckets;
} MmioMap;

MmioMap *newMmioMap(void);

void freeMmioMap(MmioMap *mmioMap);

int extendMmioMap(MmioMap *mmioMap);

int putMmioMap(MmioMap *mmioMap, uint32_t address, accessFunc read, accessFunc write);

accessFunc getReadFuncMmioMap(const MmioMap *mmioMap, uint32_t address);

accessFunc getWriteFuncMmioMap(const MmioMap *mmioMap, uint32_t address);

#endif // VM_RISCV_MMIOMAP_H

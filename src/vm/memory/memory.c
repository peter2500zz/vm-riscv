//
// Created by Peter Shen on 2026/6/26.
//

#include "memory.h"
#include "../devices/mmio.h"
#include <stdlib.h>

Memory *newMemory(uint32_t size) {
        if (size > MAX_MEM_SIZE) {
                goto out;
        }

        Memory *mem = malloc(sizeof(Memory));
        if (mem == NULL) {
                goto out;
        }

        mem->size = RAM_BASE + size;
        mem->ram = newRam(size);
        if (mem->ram == NULL) {
                goto out_free_mem;
        }

        mem->mmioMap = newMmioMap();
        if (mem->mmioMap == NULL) {
                goto out_free_ram;
        }

        if (initMmio(mem->mmioMap)) {
                goto out_free_mmio;
        }

        return mem;

out_free_mmio:
        freeMmioMap(&mem->mmioMap);
out_free_ram:
        freeRam(&mem->ram);
out_free_mem:
        free(mem);
out:
        return NULL;
}

void freeMemory(Memory **ppMem) {
        if (ppMem == NULL) {
                return;
        }

        Memory *pMem = *ppMem;
        if (pMem == NULL) {
                return;
        }

        freeMmioMap(&pMem->mmioMap);
        freeRam(&pMem->ram);
        free(pMem);

        *ppMem = NULL;
}

int readMemory(Memory *mem, const uint32_t address, void *value,
               const uint32_t size) {
        if (address < RAM_BASE) {
                const mmioAccessFunc func =
                    getReadFuncMmioMap(mem->mmioMap, address, size);
                if (func == NULL) {
                        // TODO: invalid func
                        return 1;
                }

                return func(value);
        } else {
                const uint32_t ramAddr = address - RAM_BASE;
                const ramAccessFunc func =
                    mem->ram->func[__builtin_ctz(size)].read;

                return func(mem->ram, ramAddr, value);
        }
}

int writeMemory(Memory *mem, const uint32_t address, void *value,
                const uint32_t size) {
        if (address < RAM_BASE) {
                const mmioAccessFunc func =
                    getWriteFuncMmioMap(mem->mmioMap, address, size);
                if (func == NULL) {
                        // TODO: invalid func
                        return 1;
                }

                return func(value);
        } else {
                const uint32_t ramAddr = address - RAM_BASE;
                const ramAccessFunc func =
                    mem->ram->func[__builtin_ctz(size)].write;

                return func(mem->ram, ramAddr, value);
        }
}

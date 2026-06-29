//
// Created by Peter Shen on 2026/6/26.
//

#include "memory.h"
#include "mmio.h"
#include <stdlib.h>

Memory *newMemory(uint32_t size) {
        // 检查 RAM 内存大小是否超过限制
        if (size > MAX_MEM_SIZE) {
                goto out;
        }

        // 分配结构体
        Memory *mem = malloc(sizeof(Memory));
        if (mem == NULL) {
                goto out;
        }

        // 内存大小为 MMIO 地址空间 + RAM 内存大小
        mem->size = RAM_BASE + size;
        // 分配 RAM
        mem->ram = newRam(size);
        if (mem->ram == NULL) {
                goto out_free_mem;
        }

        // 分配 MMIO 映射表
        mem->mmioMap = newMmioMap();
        if (mem->mmioMap == NULL) {
                goto out_free_ram;
        }

        // 初始化 MMIO 映射表
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
        // 判断地址是否是 MMIO 地址
        if (address < RAM_BASE) {
                // 尝试获取对应位置的 MMIO 操作函数
                const mmioAccessFunc func =
                    getReadFuncMmioMap(mem->mmioMap, address, size);
                if (func == NULL) {
                        // TODO: invalid func
                        return 1;
                }

                return func(value);
        } else {
                const uint32_t ramAddr = address - RAM_BASE;
                // 获取对应字节数的操作函数
                const ramAccessFunc func =
                    mem->ram->func[__builtin_ctz(size)].read;

                return func(mem->ram, ramAddr, value);
        }
}

int writeMemory(Memory *mem, const uint32_t address, void *value,
                const uint32_t size) {
        // 判断地址是否是 MMIO 地址
        if (address < RAM_BASE) {
                // 尝试获取对应位置的 MMIO 操作函数
                const mmioAccessFunc func =
                    getWriteFuncMmioMap(mem->mmioMap, address, size);
                if (func == NULL) {
                        // TODO: invalid func
                        return 1;
                }

                return func(value);
        } else {
                const uint32_t ramAddr = address - RAM_BASE;
                // 获取对应字节数的操作函数
                const ramAccessFunc func =
                    mem->ram->func[__builtin_ctz(size)].write;

                return func(mem->ram, ramAddr, value);
        }
}

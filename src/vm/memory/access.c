#include "access.h"
#include "../devices/mmio.h"
#include <stdint.h>
#include <stdlib.h>

Memory *memory_new(uint32_t size) {
        if (size > MAX_MEM_SIZE) {
                goto out;
        }

        Memory *mem = calloc(1, sizeof(Memory));
        if (mem == NULL) {
                goto out;
        }

        mem->data = calloc(1, size);
        if (mem->data == NULL) {
                goto out_free_mem;
        }
        mem->size = size;

        return mem;

out_free_mem:
        free(mem);
out:
        return NULL;
}

void memory_free(Memory *mem) {
        if (mem == NULL) {
                return;
        }
        free(mem->data);
        free(mem);
}

void memory_access(Hart *hart, uint32_t addr, void *target, uint32_t size,
                   MemAccessType type) {
        switch (size) {
        case sizeof(uint8_t):
                break;
        case sizeof(uint16_t):
                if (addr & 1) {
                        fprintf(stderr,
                                "Unaligned half-word access at 0x%08x\n", addr);
                        return;
                }
                break;
        case sizeof(uint32_t):
                // if (addr & 3) {
                //         fprintf(stderr, "Unaligned word access at 0x%08x\n",
                //                 addr);
                //         return;
                // }
                break;
        }
        if (addr < RAM_ADDR) {
                handle_mmio(hart, addr, target, size, type);

                return;
        }
        if (addr >= RAM_ADDR + hart->mem->size) {
                fprintf(stderr, "Accessing out-of-bounds memory at 0x%08x\n",
                        addr);
                hart_debug(hart);
                exit(1);
        }
        uint32_t offset = addr - RAM_ADDR;

        switch (type) {
        case MEM_READ:
                switch (size) {
                case sizeof(uint8_t):
                        *(uint8_t *)target = hart->mem->data[offset];
                        break;
                case sizeof(uint16_t):
                        *(uint16_t *)target =
                            *(uint16_t *)&hart->mem->data[offset];
                        break;
                case sizeof(uint32_t):
                        *(uint32_t *)target =
                            *(uint32_t *)&hart->mem->data[offset];
                        break;
                }
                break;
        case MEM_WRITE:
                switch (size) {
                case sizeof(uint8_t):
                        hart->mem->data[offset] = *(uint8_t *)target;
                        break;
                case sizeof(uint16_t):
                        *(uint16_t *)&hart->mem->data[offset] =
                            *(uint16_t *)target;
                        break;
                case sizeof(uint32_t):
                        *(uint32_t *)&hart->mem->data[offset] =
                            *(uint32_t *)target;
                        break;
                }
                break;
        }
}

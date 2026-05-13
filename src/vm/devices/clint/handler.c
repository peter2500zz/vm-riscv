#include "handler.h"

uint32_t msip = 0;
uint64_t mtime = 0;
uint64_t mtimecmp = 0xFFFFFFFFFFFFFFFF;

void handle_clint(Hart *hart, uint32_t addr, void *target, uint32_t size,
                  MemAccessType type) {
        switch (addr - CLINT_ADDR) {
        case 0x0000: // MSIP
                if (size != sizeof(uint32_t)) {
                        fprintf(stderr, "Invalid access size for MSIP: %u\n",
                                size);
                        exit(1);
                }
                switch (type) {
                case MEM_READ:
                        *(uint32_t *)target = msip;
                        break;
                case MEM_WRITE:
                        msip = *(uint32_t *)target;
                        break;
                }
                break;
        case 0x4000: // MTIMECMP lo
                if (size != sizeof(uint32_t)) {
                        fprintf(stderr,
                                "Invalid access size for MTIMECMP lo: %u\n",
                                size);
                        exit(1);
                }
                switch (type) {
                case MEM_READ:
                        *(uint32_t *)target = (uint32_t)(mtimecmp & 0xFFFFFFFF);
                        break;
                case MEM_WRITE:
                        mtimecmp = (mtimecmp & 0xFFFFFFFF00000000) |
                                   *(uint32_t *)target;
                        break;
                }
                break;
        case 0x4004: // MTIMECMP hi
                if (size != sizeof(uint32_t)) {
                        fprintf(stderr,
                                "Invalid access size for MTIMECMP hi: %u\n",
                                size);
                        exit(1);
                }
                switch (type) {
                case MEM_READ:
                        *(uint32_t *)target = (uint32_t)(mtimecmp >> 32);
                        break;
                case MEM_WRITE:
                        mtimecmp = (mtimecmp & 0xFFFFFFFF) |
                                   ((uint64_t)(*(uint32_t *)target) << 32);
                        break;
                }
                break;
        case 0xBFF8: // MTIME lo
                if (size != sizeof(uint32_t)) {
                        fprintf(stderr,
                                "Invalid access size for MTIME lo: %u\n", size);
                        exit(1);
                }
                switch (type) {
                case MEM_READ:
                        *(uint32_t *)target = (uint32_t)(mtime & 0xFFFFFFFF);
                        break;
                case MEM_WRITE:
                        mtime =
                            (mtime & 0xFFFFFFFF00000000) | *(uint32_t *)target;
                        break;
                }
                break;
        case 0xBFFC: // MTIME hi
                if (size != sizeof(uint32_t)) {
                        fprintf(stderr,
                                "Invalid access size for MTIME hi: %u\n", size);
                        exit(1);
                }
                switch (type) {
                case MEM_READ:
                        *(uint32_t *)target = (uint32_t)(mtime >> 32);
                        break;
                case MEM_WRITE:
                        mtime = (mtime & 0xFFFFFFFF) |
                                ((uint64_t)(*(uint32_t *)target) << 32);
                        break;
                }
                break;
        }
}
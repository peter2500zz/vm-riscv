#ifndef MMIO_H
#define MMIO_H

#include "../cpu/hart/unprivileged.h"

typedef void (*DeviceIO)(Hart *hart, uint32_t addr, void *target, uint32_t size,
                         MemAccessType type);

void handle_mmio(Hart *hart, uint32_t addr, void *target, uint32_t size,
                 MemAccessType type);

#endif // MMIO_H

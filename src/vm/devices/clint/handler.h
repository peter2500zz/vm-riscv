#ifndef CLINT_HANDLER_H
#define CLINT_HANDLER_H

#include "../../cpu/hart/unprivileged.h"

uint32_t msip;
uint64_t mtime;
uint64_t mtimecmp;

#define CLINT_ADDR 0x02000000
#define CLINT_SIZE 0x10000

void handle_clint(Hart *hart, uint32_t addr, void *target, uint32_t size,
                  MemAccessType type);

#endif // CLINT_HANDLER_H

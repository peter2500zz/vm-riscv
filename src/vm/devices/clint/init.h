//
// Created by Peter Shen on 2026/6/29.
//

#ifndef MMIO_CLINT_INIT_H
#define MMIO_CLINT_INIT_H

#include "../../../utils/mmioMap.h"

#define MMIO_CLINT_BASE 0x02000000
#define MMIO_CLINT_MSIP 0x0
#define MMIO_CLINT_MTIMECMP_LO 0x4000
#define MMIO_CLINT_MTIMECMP_HI 0x4004
#define MMIO_CLINT_MTIME_LO 0xBFF8
#define MMIO_CLINT_MTIME_HI 0xBFFC

int initClint(MmioMap *mmioMap);

#endif // MMIO_CLINT_INIT_H

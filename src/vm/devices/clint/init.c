//
// Created by Peter Shen on 2026/6/29.
//

#include "init.h"

#include "handler.h"

int initClint(MmioMap *mmioMap) {
        putReadFuncMmioMap(mmioMap, MMIO_CLINT_BASE + MMIO_CLINT_MSIP,
                           &readMSIP, sizeof(uint32_t));
        putWriteFuncMmioMap(mmioMap, MMIO_CLINT_BASE + MMIO_CLINT_MSIP,
                            &writeMSIP, sizeof(uint32_t));

        putReadFuncMmioMap(mmioMap, MMIO_CLINT_BASE + MMIO_CLINT_MTIMECMP_LO,
                           &readMTIMECMPlo, sizeof(uint32_t));
        putWriteFuncMmioMap(mmioMap, MMIO_CLINT_BASE + MMIO_CLINT_MTIMECMP_LO,
                            &writeMTIMECMPlo, sizeof(uint32_t));

        putReadFuncMmioMap(mmioMap, MMIO_CLINT_BASE + MMIO_CLINT_MTIMECMP_HI,
                           &readMTIMECMPhi, sizeof(uint32_t));
        putWriteFuncMmioMap(mmioMap, MMIO_CLINT_BASE + MMIO_CLINT_MTIMECMP_HI,
                            &writeMTIMECMPhi, sizeof(uint32_t));

        putReadFuncMmioMap(mmioMap, MMIO_CLINT_BASE + MMIO_CLINT_MTIME_LO,
                           &readMTIMElo, sizeof(uint32_t));
        putWriteFuncMmioMap(mmioMap, MMIO_CLINT_BASE + MMIO_CLINT_MTIME_LO,
                            &writeMTIMElo, sizeof(uint32_t));

        putReadFuncMmioMap(mmioMap, MMIO_CLINT_BASE + MMIO_CLINT_MTIME_HI,
                           &readMTIMEhi, sizeof(uint32_t));
        putWriteFuncMmioMap(mmioMap, MMIO_CLINT_BASE + MMIO_CLINT_MTIME_HI,
                            &writeMTIMEhi, sizeof(uint32_t));

        return 0;
}

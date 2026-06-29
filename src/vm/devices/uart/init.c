//
// Created by Peter Shen on 2026/6/26.
//

#include "init.h"

#include "console.h"

int initUart(MmioMap *mmioMap) {
        putWriteFuncMmioMap(mmioMap, MMIO_UART_BASE + MMIO_UART_THR, &writeTHR,
                            sizeof(uint8_t));
        putReadFuncMmioMap(mmioMap, MMIO_UART_BASE + MMIO_UART_LSR, &readLSR,
                           sizeof(uint8_t));

        return 0;
}

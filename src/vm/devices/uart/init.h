//
// Created by Peter Shen on 2026/6/26.
//

#ifndef MMIO_UART_INIT_H
#define MMIO_UART_INIT_H

#include "../../../utils/mmioMap.h"

#define MMIO_UART_BASE 0x10000000
#define MMIO_UART_THR 0x0
#define MMIO_UART_LSR 0x5

int initUart(MmioMap *mmioMap);

#endif // MMIO_UART_INIT_H

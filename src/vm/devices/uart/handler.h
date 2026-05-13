#ifndef UART_HANDLER_H
#define UART_HANDLER_H

#include "../../cpu/hart/unprivileged.h"

#define UART_ADDR 0x10000000
#define UART_SIZE 0x100

void handle_uart(Hart *hart, uint32_t addr, void *target, uint32_t size,
                 MemAccessType type);

#endif // UART_HANDLER_H

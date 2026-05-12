#ifndef UART_HANDLER_H
#define UART_HANDLER_H

#include "../../cpu/hart/unprivileged.h"

void handle_uart(Hart *hart, uint32_t addr, void *target, uint32_t size,
                 MemAccessType type);

#endif // UART_HANDLER_H

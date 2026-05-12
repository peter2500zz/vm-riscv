#include "handler.h"
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
        int fd;
        uint8_t cache;
        char pending;
} FakeUART;

static FakeUART *uart = NULL;

static void uart_init(void) {
        if (uart != NULL)
                return;
        uart = malloc(sizeof(FakeUART));
        if (uart == NULL) {
                fprintf(stderr, "Failed to allocate memory for UART\n");
                exit(1);
        }
        uart->pending = 0;
        uart->cache = 0;
        uart->fd = open("/tmp/ttyV1", O_RDWR | O_NONBLOCK);
        if (uart->fd < 0) {
                fprintf(stderr, "Failed to open /tmp/ttyV1\n");
                free(uart);
                uart = NULL;
                exit(1);
        }
        printf("successfully opened /tmp/ttyV1 for UART emulation\n");
}

void handle_uart(Hart *hart, uint32_t addr, void *target, uint32_t size,
                 MemAccessType type) {
        uart_init();

        switch (addr - UART_ADDR) {
        case 0x05: { // LSR
                if (!uart->pending) {
                        uint8_t ch;
                        ssize_t n = read(uart->fd, &ch, 1);
                        if (n == 1) {
                                uart->cache = ch;
                                uart->pending = 1;
                        }
                }
                *(uint8_t *)target = uart->pending ? 0x61 : 0x60;
                break;
        }
        case 0x00: { // RBR/THR
                switch (type) {
                case MEM_READ: {
                        if (uart->pending) {
                                *(uint8_t *)target = uart->cache;
                                uart->pending = 0;
                                uint8_t ch;
                                ssize_t n = read(uart->fd, &ch, 1);
                                if (n == 1) {
                                        uart->cache = ch;
                                        uart->pending = 1;
                                }
                        } else {
                                *(uint8_t *)target = 0x00;
                        }
                        break;
                }
                case MEM_WRITE: {
                        write(uart->fd, target, 1);
                        break;
                }
                }
                break;
        }
        }
}
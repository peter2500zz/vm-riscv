#ifndef MEMORY_TYPES_H
#define MEMORY_TYPES_H

#include <stdint.h>

#define UART_ADDR 0x10000000
#define RAM_ADDR 0x80000000

#define MAX_MEM_SIZE 0x40000000

typedef struct {
        uint8_t *data;
        uint32_t size;
} Memory;

typedef enum { MEM_READ, MEM_WRITE } MemAccessType;

#endif // MEMORY_TYPES_H

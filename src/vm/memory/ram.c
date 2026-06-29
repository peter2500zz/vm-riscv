//
// Created by Peter Shen on 2026/6/26.
//

#include "ram.h"

#include <stdlib.h>

int read8(Ram *ram, uint32_t address, void *value) {
        uint8_t *data = value;
        // TODO: 对其
        *data = ram->data[address];

        return 0;
}

int write8(Ram *ram, uint32_t address, void *value) {
        uint8_t *data = value;
        // TODO: 对其
        ram->data[address] = *data;

        return 0;
}

int read16(Ram *ram, uint32_t address, void *value) {
        uint16_t *data = value;
        // TODO: 对齐
        *data = (uint16_t)ram->data[address] |
                (uint16_t)((uint16_t)ram->data[address + 1] << 8);

        return 0;
}

int write16(Ram *ram, uint32_t address, void *value) {
        uint16_t *data = value;
        // TODO: 对齐
        ram->data[address] = (uint8_t)(*data & 0xFF);
        ram->data[address + 1] = (uint8_t)((*data >> 8) & 0xFF);

        return 0;
}

int read32(Ram *ram, uint32_t address, void *value) {
        uint32_t *data = value;
        // TODO: 对齐
        *data = (uint32_t)ram->data[address] |
                ((uint32_t)ram->data[address + 1] << 8) |
                ((uint32_t)ram->data[address + 2] << 16) |
                ((uint32_t)ram->data[address + 3] << 24);

        return 0;
}

int write32(Ram *ram, uint32_t address, void *value) {
        uint32_t *data = value;
        // TODO: 对齐
        ram->data[address] = (uint8_t)(*data & 0xFF);
        ram->data[address + 1] = (uint8_t)((*data >> 8) & 0xFF);
        ram->data[address + 2] = (uint8_t)((*data >> 16) & 0xFF);
        ram->data[address + 3] = (uint8_t)((*data >> 24) & 0xFF);

        return 0;
}

int initRam(Ram *ram) {
        if (ram == NULL) {
                return 1;
        }

        ram->func[__builtin_ctz(sizeof(uint8_t))].read = &read8;
        ram->func[__builtin_ctz(sizeof(uint8_t))].write = &write8;

        ram->func[__builtin_ctz(sizeof(uint16_t))].read = &read16;
        ram->func[__builtin_ctz(sizeof(uint16_t))].write = &write16;

        ram->func[__builtin_ctz(sizeof(uint32_t))].read = &read32;
        ram->func[__builtin_ctz(sizeof(uint32_t))].write = &write32;

        return 0;
}

Ram *newRam(const uint32_t size) {
        Ram *ram = malloc(sizeof(Ram));
        if (ram == NULL) {
                goto out;
        }

        ram->size = size;
        ram->data = calloc(size, sizeof(*ram->data));
        if (ram->data == NULL) {
                goto out_free_ram;
        }

        if (initRam(ram)) {
                goto out_free_ram_data;
        }

        return ram;

out_free_ram_data:
        free(ram->data);
out_free_ram:
        free(ram);
out:
        return NULL;
}

void freeRam(Ram **ppRam) {
        if (*ppRam == NULL) {
                return;
        }

        Ram *pRam = *ppRam;
        if (pRam == NULL) {
                return;
        }

        free(pRam->data);
        free(pRam);

        *ppRam = NULL;
}

//
// Created by Peter Shen on 2026/6/26.
//

#include "console.h"

#include <stdint.h>
#include <stdio.h>

int writeTHR(void *target) {
        const uint8_t *data = target;

        printf("%c", *data);

        return 0;
}
int readLSR(void *target) {
        uint8_t *data = target;

        *data = 0x60;

        return 0;
}

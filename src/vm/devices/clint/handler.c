//
// Created by Peter Shen on 2026/6/26.
//

#include "handler.h"

#include <stdint.h>

uint32_t msip = 0;
uint64_t mtime = 0;
uint64_t mtimecmp = 0xFFFFFFFFFFFFFFFF;

int readMSIP(void *target) {
        uint32_t *data = target;
        *data = msip;

        return 0;
}

int writeMSIP(void *target) {
        uint32_t *data = target;
        msip = *data;

        return 0;
}

int readMTIMECMPlo(void *target) {
        uint32_t *data = target;
        *data = mtimecmp & 0xFFFFFFFF;

        return 0;
}

int writeMTIMECMPlo(void *target) {
        uint32_t *data = target;
        mtimecmp = (mtimecmp & 0xFFFFFFFF00000000) | *data;

        return 0;
}

int readMTIMECMPhi(void *target) {
        uint32_t *data = target;
        *data = mtimecmp >> 32;

        return 0;
}

int writeMTIMECMPhi(void *target) {
        uint32_t *data = target;
        mtimecmp = (mtimecmp & 0xFFFFFFFF) | ((uint64_t)(*data) << 32);

        return 0;
}

int readMTIMElo(void *target) {
        uint32_t *data = target;
        *data = mtime & 0xFFFFFFFF;

        return 0;
}

int writeMTIMElo(void *target) {
        uint32_t *data = target;
        mtime = (mtime & 0xFFFFFFFF00000000) | *data;

        return 0;
}

int readMTIMEhi(void *target) {
        uint32_t *data = target;
        *data = mtime >> 32;

        return 0;
}

int writeMTIMEhi(void *target) {
        uint32_t *data = target;
        mtime = (mtime & 0xFFFFFFFF) | ((uint64_t)(*data) << 32);

        return 0;
}

//
// Created by Peter Shen on 2026/6/26.
//

#include "../src/utils/mmioMap.h"
#include "test_utils.h"

#include <stddef.h>

int readAlways20(void *value) {
        uint32_t *target = value;
        *target = 20;

        return 0;
}

static uint32_t WHAT_A_VALUE = 30;

int readWhatAValue(void *value) {
        uint32_t *target = value;
        *target = WHAT_A_VALUE;

        return 0;
}

int writeWhatAValue(void *value) {
        uint32_t *target = value;
        WHAT_A_VALUE = *target;

        return 0;
}

int main(void) {
        MmioMap *mmioMap = newMmioMap();

        putReadFuncMmioMap(mmioMap, 0x951, readWhatAValue, sizeof(uint32_t));
        putWriteFuncMmioMap(mmioMap, 0x951, writeWhatAValue, sizeof(uint32_t));
        putReadFuncMmioMap(mmioMap, 0x8699, readAlways20, sizeof(uint32_t));

        ASSERT_EQ(WHAT_A_VALUE, 30);

        uint32_t myVal = 144;
        mmioAccessFunc readWtv =
            getReadFuncMmioMap(mmioMap, 0x951, sizeof(uint32_t));
        readWtv(&myVal);
        ASSERT_EQ(myVal, 30);

        mmioAccessFunc writeWtv =
            getWriteFuncMmioMap(mmioMap, 0x951, sizeof(uint32_t));
        myVal = 666;
        writeWtv(&myVal);
        ASSERT_EQ(WHAT_A_VALUE, 666);

        mmioAccessFunc voidFunc =
            getWriteFuncMmioMap(mmioMap, 0x8699, sizeof(uint32_t));
        ASSERT_EQ(voidFunc, NULL);

        mmioAccessFunc alwaysFunc =
            getReadFuncMmioMap(mmioMap, 0x8699, sizeof(uint32_t));
        alwaysFunc(&myVal);
        ASSERT_EQ(myVal, 20);

        freeMmioMap(&mmioMap);

        ASSERT_EQ(mmioMap, NULL)
}

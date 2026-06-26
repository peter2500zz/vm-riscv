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

        putReadFuncMmioMap(mmioMap, 0x951, readWhatAValue);
        putWriteFuncMmioMap(mmioMap, 0x951, writeWhatAValue);
        putReadFuncMmioMap(mmioMap, 0x8699, readAlways20);

        ASSERT_EQ(WHAT_A_VALUE, 30);

        uint32_t myVal = 144;
        accessFunc readWtv = getReadFuncMmioMap(mmioMap, 0x951);
        readWtv(&myVal);
        ASSERT_EQ(myVal, 30);

        accessFunc writeWtv = getWriteFuncMmioMap(mmioMap, 0x951);
        myVal = 666;
        writeWtv(&myVal);
        ASSERT_EQ(WHAT_A_VALUE, 666);

        accessFunc voidFunc = getWriteFuncMmioMap(mmioMap, 0x8699);
        ASSERT_EQ(voidFunc, NULL);

        accessFunc alwaysFunc = getReadFuncMmioMap(mmioMap, 0x8699);
        alwaysFunc(&myVal);
        ASSERT_EQ(myVal, 20);

        freeMmioMap(&mmioMap);

        ASSERT_EQ(mmioMap, NULL)
}

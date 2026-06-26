//
// Created by Peter Shen on 2026/6/26.
//

#include "../src/utils/mmioMap.h"

int readAlways20(uint32_t *target) {
        *target = 20;

        return 0;
}

int main(void) {
        MmioMap *mmioMap = newMmioMap();
}

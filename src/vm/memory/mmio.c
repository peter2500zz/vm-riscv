
#include "mmio.h"

#include "../devices/clint/init.h"
#include "../devices/uart/init.h"

int initMmio(MmioMap *mmioMap) {
        if (initUart(mmioMap))
                return 1;

        if (initClint(mmioMap))
                return 1;

        return 0;
}
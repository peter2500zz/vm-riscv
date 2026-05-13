#include "mmio.h"
// UART
#include "uart/handler.h"

typedef struct {
        uint32_t base;
        uint32_t size;
        DeviceIO handler;
} DeviceRegion;

static DeviceRegion devices[] = {
    //     { 0x02000000, 0x10000, clint_io },
//     {0x10000000, 0x100, (DeviceIO)handle_uart},
    {0, 0, NULL}, // 终止符
};

DeviceIO find_device(uint32_t addr) {
        for (int i = 0; devices[i].handler; i++) {
                if (addr >= devices[i].base &&
                    addr < devices[i].base + devices[i].size)
                        return devices[i].handler;
        }
        return NULL;
}

void handle_mmio(Hart *hart, uint32_t addr, void *target, uint32_t size,
                 MemAccessType type) {

        DeviceIO handler = find_device(addr);
        if (handler) {
                handler(hart, addr, target, size, type);
        } else {
                fprintf(stderr, "No device found for address 0x%08X\n", addr);
                hart_debug(hart);
                exit(1);
        }
}

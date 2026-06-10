#include "mmio.h"
// UART
#include "uart/handler.h"
// CLINT
#include "clint/handler.h"

#include "../cpu/hart/privileged.h"

typedef struct {
        uint32_t base;
        uint32_t size;
        DeviceIO handler;
} DeviceRegion;

static DeviceRegion devices[] = {
    {CLINT_ADDR, CLINT_SIZE, (DeviceIO)handle_clint},
    {UART_ADDR, UART_SIZE, (DeviceIO)handle_uart},
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

int handle_mmio(Hart *hart, uint32_t addr, void *target, uint32_t size,
                MemAccessType type) {

        DeviceIO handler = find_device(addr);
        if (handler) {
                handler(hart, addr, target, size, type);
        } else {
                fprintf(stderr, "No device found for address 0x%08X\n", addr);
                hart_trap_sync(hart,
                               type == MEM_READ ? CAUSE_MISALIGNED_LOAD
                                                : CAUSE_MISALIGNED_STORE,
                               addr);
                return 1;
        }
        return 0;
}

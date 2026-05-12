#include "test_utils.h"

int main(void) {
        SETUP_HART(4);

        // lui x1, 1048575
        hart_dispatch(hart, 0xfffff0b7);
        ASSERT_EQ(hart->_regs[1], 1048575 << 12);

        // lui x1, 0
        hart_dispatch(hart, 0x000000b7);
        ASSERT_EQ(hart->_regs[1], 0);

        // lui x2, 2500
        hart_dispatch(hart, 0x009c4137);
        ASSERT_EQ(hart->_regs[1], 0);
        ASSERT_EQ(hart->_regs[2], 2500 << 12);

        return 0;
}

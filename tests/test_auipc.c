#include "test_utils.h"

int main(void) {
        SETUP_HART(4);

        hart->_pc = 0;

        // auipc x1, 8
        hart_dispatch(hart, 0x00008097);
        ASSERT_EQ(hart->_regs[1], 8 << 12);

        hart->_pc = 4;

        // auipc x1, 4
        hart_dispatch(hart, 0x00004097);
        ASSERT_EQ(hart->_regs[1], (4 << 12) + 4);

        hart->_pc = 0;

        // auipc x2, 12
        hart_dispatch(hart, 0x0000c117);
        ASSERT_EQ(hart->_regs[1], (4 << 12) + 4);
        ASSERT_EQ(hart->_regs[2], 12 << 12);

        return 0;
}

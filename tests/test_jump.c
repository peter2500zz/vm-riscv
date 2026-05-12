#include "test_utils.h"

int main(void) {
        SETUP_HART(4);

        // JAL

        hart->_pc = 0;

        // jal x1, 8
        hart_dispatch(hart, 0x008000ef);
        ASSERT_EQ(hart->_pc, 8);
        ASSERT_EQ(hart->_regs[1], 4);

        // JALR

        hart->_pc = 0;
        hart->_regs[1] = 4;

        // jalr x2, x1, 4
        hart_dispatch(hart, 0x00408167);
        ASSERT_EQ(hart->_pc, 8);
        ASSERT_EQ(hart->_regs[2], 4);

        // jalr x2, x2, 8
        hart_dispatch(hart, 0x00810167);
        ASSERT_EQ(hart->_pc, 12);
        ASSERT_EQ(hart->_regs[2], 12);

        return 0;
}

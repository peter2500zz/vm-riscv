#include "test_utils.h"

int main(void) {
        SETUP_HART(5);

        // AND

        hart->_pc = 0;

        hart->_regs[4] = 32;
        // add x1, x0, x4
        hart_dispatch(hart, 0x004000b3);
        ASSERT_EQ(hart->_regs[1], 32);

        hart->_regs[4] = 96;
        // add x1, x1, x4
        hart_dispatch(hart, 0x004080b3);
        ASSERT_EQ(hart->_regs[1], 128);

        hart->_regs[4] = -64;
        // add x2, x1, x4
        hart_dispatch(hart, 0x00408133);
        ASSERT_EQ(hart->_regs[1], 128);
        ASSERT_EQ(hart->_regs[2], 64);

        // SLT

        hart->_pc = 0;
        hart->_regs[2] = 50;

        hart->_regs[4] = 100;
        // slt x1, x2, x4
        hart_dispatch(hart, 0x004120b3);
        ASSERT_EQ(hart->_regs[1], 1);

        hart->_regs[4] = 25;
        // slt x1, x2, x4
        hart_dispatch(hart, 0x004120b3);
        ASSERT_EQ(hart->_regs[1], 0);

        hart->_regs[4] = 50;
        // slt x1, x2, x4
        hart_dispatch(hart, 0x004120b3);
        ASSERT_EQ(hart->_regs[1], 0);

        hart->_pc = 0;
        hart->_regs[2] = 0;

        hart->_regs[4] = 100;
        // slt x1, x2, x4
        hart_dispatch(hart, 0x004120b3);
        ASSERT_EQ(hart->_regs[1], 1);

        hart->_regs[4] = -75;
        // slt x1, x2, x4
        hart_dispatch(hart, 0x004120b3);
        ASSERT_EQ(hart->_regs[1], 0);

        hart->_regs[4] = 0;
        // slt x1, x2, x4
        hart_dispatch(hart, 0x004120b3);
        ASSERT_EQ(hart->_regs[1], 0);

        // SLTU

        hart->_pc = 0;
        hart->_regs[2] = 50;
        
        hart->_regs[4] = 100;
        // sltu x1, x2, x4
        hart_dispatch(hart, 0x004130b3);
        ASSERT_EQ(hart->_regs[1], 1);

        hart->_regs[4] = 25;
        // sltu x1, x2, x4
        hart_dispatch(hart, 0x004130b3);
        ASSERT_EQ(hart->_regs[1], 0);

        hart->_regs[4] = 50;
        // sltu x1, x2, x4
        hart_dispatch(hart, 0x004130b3);
        ASSERT_EQ(hart->_regs[1], 0);

        hart->_pc = 0;
        hart->_regs[2] = 0;

        hart->_regs[4] = 100;
        // sltu x1, x2, x4
        hart_dispatch(hart, 0x004130b3);
        ASSERT_EQ(hart->_regs[1], 1);

        hart->_regs[4] = -75;
        // sltu x1, x2, x4
        hart_dispatch(hart, 0x004130b3);
        ASSERT_EQ(hart->_regs[1], 1);

        hart->_regs[4] = 0;
        // sltu x1, x2, x4
        hart_dispatch(hart, 0x004130b3);
        ASSERT_EQ(hart->_regs[1], 0);

        // XOR

        hart->_pc = 0;
        hart->_regs[2] = 0xdeadbeef;

        hart->_regs[4] = 1750;
        // xor x1, x2, x4
        hart_dispatch(hart, 0x004140b3);
        ASSERT_EQ(hart->_regs[1], 0xDEADB839);

        hart->_regs[4] = -799;
        // xor x1, x2, x4
        hart_dispatch(hart, 0x004140b3);
        ASSERT_EQ(hart->_regs[1], 0x2152420E);

        // OR

        hart->_pc = 0;
        hart->_regs[2] = 0xdeadbeef;

        hart->_regs[4] = 1750;
        // or x1, x2, x4
        hart_dispatch(hart, 0x004160b3);
        ASSERT_EQ(hart->_regs[1], 0xDEADBEFF);

        hart->_regs[4] = -799;
        // or x1, x2, x4
        hart_dispatch(hart, 0x004160b3);
        ASSERT_EQ(hart->_regs[1], 0xFFFFFEEF);

        // AND

        hart->_pc = 0;
        hart->_regs[2] = 0xdeadbeef;

        hart->_regs[4] = 1750;
        // and x1, x2, x4
        hart_dispatch(hart, 0x004170b3);
        ASSERT_EQ(hart->_regs[1], 0x6C6);

        hart->_regs[4] = -799;
        // and x1, x2, x4
        hart_dispatch(hart, 0x004170b3);
        ASSERT_EQ(hart->_regs[1], 0xDEADBCE1);

        // SLL

        hart->_pc = 0;
        hart->_regs[2] = 0xdeadbeef;

        hart->_regs[4] = 4;
        // sll x1, x2, x4
        hart_dispatch(hart, 0x004110b3);
        ASSERT_EQ(hart->_regs[1], 0xeadbeef0);

        // SRL

        hart->_pc = 0;
        hart->_regs[2] = 0xdeadbeef;

        hart->_regs[4] = 4;
        // srl x1, x2, x4
        hart_dispatch(hart, 0x004150b3);
        ASSERT_EQ(hart->_regs[1], 0x0deadbee);

        // SRA

        hart->_pc = 0;
        hart->_regs[2] = 0xdeadbeef;

        hart->_regs[4] = 4;
        // sra x1, x2, x4
        hart_dispatch(hart, 0x404150b3);
        ASSERT_EQ(hart->_regs[1], 0xfdeadbee);

        return 0;
}

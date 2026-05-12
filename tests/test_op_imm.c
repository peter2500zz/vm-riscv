#include "test_utils.h"

int main(void) {
        SETUP_HART(5);

        // ANDI

        hart->_pc = 0;

        // addi x1, x0, 32
        hart_dispatch(hart, 0x02000093);
        ASSERT_EQ(hart->_regs[1], 32);

        // addi x1, x1, 96
        hart_dispatch(hart, 0x06008093);
        ASSERT_EQ(hart->_regs[1], 128);

        // addi x2, x1, -64
        hart_dispatch(hart, 0xfc008113);
        ASSERT_EQ(hart->_regs[1], 128);
        ASSERT_EQ(hart->_regs[2], 64);

        // SLTI

        hart->_pc = 0;
        hart->_regs[2] = 50;

        // slti x1, x2, 100
        hart_dispatch(hart, 0x06412093);
        ASSERT_EQ(hart->_regs[1], 1);

        // slti x1, x2, 25
        hart_dispatch(hart, 0x01912093);
        ASSERT_EQ(hart->_regs[1], 0);

        // slti x1, x2, 50
        hart_dispatch(hart, 0x03212093);
        ASSERT_EQ(hart->_regs[1], 0);

        hart->_pc = 0;
        hart->_regs[2] = 0;

        // slti x1, x2, 100
        hart_dispatch(hart, 0x06412093);
        ASSERT_EQ(hart->_regs[1], 1);

        // slti x1, x2, -75
        hart_dispatch(hart, 0xfb512093);
        ASSERT_EQ(hart->_regs[1], 0);

        // slti x1, x2, 0
        hart_dispatch(hart, 0x00012093);
        ASSERT_EQ(hart->_regs[1], 0);

        // SLTIU

        hart->_pc = 0;
        hart->_regs[2] = 50;

        // sltiu x1, x2, 100
        hart_dispatch(hart, 0x06413093);
        ASSERT_EQ(hart->_regs[1], 1);

        // sltiu x1, x2, 25
        hart_dispatch(hart, 0x01913093);
        ASSERT_EQ(hart->_regs[1], 0);

        // sltiu x1, x2, 50
        hart_dispatch(hart, 0x03213093);
        ASSERT_EQ(hart->_regs[1], 0);

        hart->_pc = 0;
        hart->_regs[2] = 0;

        // sltiu x1, x2, 100
        hart_dispatch(hart, 0x06413093);
        ASSERT_EQ(hart->_regs[1], 1);

        // sltiu x1, x2, -75
        hart_dispatch(hart, 0xfb513093);
        ASSERT_EQ(hart->_regs[1], 1);

        // sltiu x1, x2, 0
        hart_dispatch(hart, 0x00013093);
        ASSERT_EQ(hart->_regs[1], 0);

        // XORI

        hart->_pc = 0;
        hart->_regs[2] = 0xdeadbeef;

        // xori x1, x2, 1750
        hart_dispatch(hart, 0x6d614093);
        ASSERT_EQ(hart->_regs[1], 0xDEADB839);

        // xori x1, x2, -799
        hart_dispatch(hart, 0xce114093);
        ASSERT_EQ(hart->_regs[1], 0x2152420E);

        // ORI

        hart->_pc = 0;
        hart->_regs[2] = 0xdeadbeef;

        // ori x1, x2, 1750
        hart_dispatch(hart, 0x6d616093);
        ASSERT_EQ(hart->_regs[1], 0xDEADBEFF);

        // ori x1, x2, -799
        hart_dispatch(hart, 0xce116093);
        ASSERT_EQ(hart->_regs[1], 0xFFFFFEEF);

        // ANDI

        hart->_pc = 0;
        hart->_regs[2] = 0xdeadbeef;

        // andi x1, x2, 1750
        hart_dispatch(hart, 0x6d617093);
        ASSERT_EQ(hart->_regs[1], 0x6C6);

        // andi x1, x2, -799
        hart_dispatch(hart, 0xce117093);
        ASSERT_EQ(hart->_regs[1], 0xDEADBCE1);

        // SLLI

        hart->_pc = 0;
        hart->_regs[2] = 0xdeadbeef;

        // slli x1, x2, 4
        hart_dispatch(hart, 0x00411093);
        ASSERT_EQ(hart->_regs[1], 0xeadbeef0);

        // SRLI

        hart->_pc = 0;
        hart->_regs[2] = 0xdeadbeef;

        // srli x1, x2, 4
        hart_dispatch(hart, 0x00415093);
        ASSERT_EQ(hart->_regs[1], 0x0deadbee);

        // SRAI

        hart->_pc = 0;
        hart->_regs[2] = 0xdeadbeef;

        // srai x1, x2, 4
        hart_dispatch(hart, 0x40415093);
        ASSERT_EQ(hart->_regs[1], 0xfdeadbee);

        return 0;
}

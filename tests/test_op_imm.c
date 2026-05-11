#include "test_utils.h"

int main(void) {
        SETUP_VM(5);

        // ANDI

        vm->_pc = 0;

        // addi x1, x0, 32
        vm_dispatch(vm, 0x02000093);
        ASSERT_EQ(vm->_regs[1], 32);

        // addi x1, x1, 96
        vm_dispatch(vm, 0x06008093);
        ASSERT_EQ(vm->_regs[1], 128);

        // addi x2, x1, -64
        vm_dispatch(vm, 0xfc008113);
        ASSERT_EQ(vm->_regs[1], 128);
        ASSERT_EQ(vm->_regs[2], 64);

        // SLTI

        vm->_pc = 0;
        vm->_regs[2] = 50;

        // slti x1, x2, 100
        vm_dispatch(vm, 0x06412093);
        ASSERT_EQ(vm->_regs[1], 1);

        // slti x1, x2, 25
        vm_dispatch(vm, 0x01912093);
        ASSERT_EQ(vm->_regs[1], 0);

        // slti x1, x2, 50
        vm_dispatch(vm, 0x03212093);
        ASSERT_EQ(vm->_regs[1], 0);

        vm->_pc = 0;
        vm->_regs[2] = 0;

        // slti x1, x2, 100
        vm_dispatch(vm, 0x06412093);
        ASSERT_EQ(vm->_regs[1], 1);

        // slti x1, x2, -75
        vm_dispatch(vm, 0xfb512093);
        ASSERT_EQ(vm->_regs[1], 0);

        // slti x1, x2, 0
        vm_dispatch(vm, 0x00012093);
        ASSERT_EQ(vm->_regs[1], 0);

        // SLTIU

        vm->_pc = 0;
        vm->_regs[2] = 50;

        // sltiu x1, x2, 100
        vm_dispatch(vm, 0x06413093);
        ASSERT_EQ(vm->_regs[1], 1);

        // sltiu x1, x2, 25
        vm_dispatch(vm, 0x01913093);
        ASSERT_EQ(vm->_regs[1], 0);

        // sltiu x1, x2, 50
        vm_dispatch(vm, 0x03213093);
        ASSERT_EQ(vm->_regs[1], 0);

        vm->_pc = 0;
        vm->_regs[2] = 0;

        // sltiu x1, x2, 100
        vm_dispatch(vm, 0x06413093);
        ASSERT_EQ(vm->_regs[1], 1);

        // sltiu x1, x2, -75
        vm_dispatch(vm, 0xfb513093);
        ASSERT_EQ(vm->_regs[1], 1);

        // sltiu x1, x2, 0
        vm_dispatch(vm, 0x00013093);
        ASSERT_EQ(vm->_regs[1], 0);

        // XORI

        vm->_pc = 0;
        vm->_regs[2] = 0xdeadbeef;

        // xori x1, x2, 1750
        vm_dispatch(vm, 0x6d614093);
        ASSERT_EQ(vm->_regs[1], 0xDEADB839);

        // xori x1, x2, -799
        vm_dispatch(vm, 0xce114093);
        ASSERT_EQ(vm->_regs[1], 0x2152420E);

        // ORI

        vm->_pc = 0;
        vm->_regs[2] = 0xdeadbeef;

        // ori x1, x2, 1750
        vm_dispatch(vm, 0x6d616093);
        ASSERT_EQ(vm->_regs[1], 0xDEADBEFF);

        // ori x1, x2, -799
        vm_dispatch(vm, 0xce116093);
        ASSERT_EQ(vm->_regs[1], 0xFFFFFEEF);

        // ANDI

        vm->_pc = 0;
        vm->_regs[2] = 0xdeadbeef;

        // andi x1, x2, 1750
        vm_dispatch(vm, 0x6d617093);
        ASSERT_EQ(vm->_regs[1], 0x6C6);

        // andi x1, x2, -799
        vm_dispatch(vm, 0xce117093);
        ASSERT_EQ(vm->_regs[1], 0xDEADBCE1);

        // SLLI

        vm->_pc = 0;
        vm->_regs[2] = 0xdeadbeef;

        // slli x1, x2, 4
        vm_dispatch(vm, 0x00411093);
        ASSERT_EQ(vm->_regs[1], 0xeadbeef0);

        // SRLI

        vm->_pc = 0;
        vm->_regs[2] = 0xdeadbeef;

        // srli x1, x2, 4
        vm_dispatch(vm, 0x00415093);
        ASSERT_EQ(vm->_regs[1], 0x0deadbee);

        // SRAI

        vm->_pc = 0;
        vm->_regs[2] = 0xdeadbeef;

        // srai x1, x2, 4
        vm_dispatch(vm, 0x40415093);
        ASSERT_EQ(vm->_regs[1], 0xfdeadbee);

        return 0;
}

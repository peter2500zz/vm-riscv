#include "test_utils.h"

int main(void) {
        SETUP_VM(5);

        // MUL
        vm->_pc = 0;

        vm->_regs[10] = 0x799;
        vm->_regs[11] = 0x1750;
        // mul x12, x10, x11
        vm_exec(vm, 0x02b50633);
        ASSERT_EQ(vm->_regs[12], 0x00b11ed0);

        // MULH
        vm->_pc = 0;

        vm->_regs[10] = 0x799;
        vm->_regs[11] = 0x79991750;
        // mulh x12, x10, x11
        vm_exec(vm, 0x02b51633);
        ASSERT_EQ(vm->_regs[12], 0x39b);

        // MULHSU
        vm->_pc = 0;

        vm->_regs[10] = 0xffff1799;
        vm->_regs[11] = 0xffff1750;
        // mulhsu x12, x10, x11
        vm_exec(vm, 0x02b52633);
        ASSERT_EQ(vm->_regs[12], 0xffff1799);

        // MULHU
        vm->_pc = 0;

        vm->_regs[10] = 0xffff1799;
        vm->_regs[11] = 0xffff1750;
        // mulhu x12, x10, x11
        vm_exec(vm, 0x02b53633);
        ASSERT_EQ(vm->_regs[12], 0xfffe2ee9);

        // DIV

        vm->_pc = 0;
        vm->_regs[10] = 0x00001750;
        vm->_regs[11] = 0x00000799;
        // div x12, x10, x11
        vm_exec(vm, 0x02b54633);
        ASSERT_EQ(vm->_regs[12], 0x00000003);

        vm->_pc = 0;
        vm->_regs[10] = 0xFFFFF6B0;
        vm->_regs[11] = 0x00000005;
        // div x12, x10, x11
        vm_exec(vm, 0x02b54633);
        ASSERT_EQ(vm->_regs[12], 0xfffffe24);

        vm->_pc = 0;
        vm->_regs[10] = 0xfffffee8;
        vm->_regs[11] = 0xFFFFFFFC;
        // div x12, x10, x11
        vm_exec(vm, 0x02b54633);
        ASSERT_EQ(vm->_regs[12], 0x00000046);

        vm->_pc = 0;
        vm->_regs[10] = 0x00001750;
        vm->_regs[11] = 0x00000000;
        // div x12, x10, x11
        vm_exec(vm, 0x02b54633);
        ASSERT_EQ(vm->_regs[12], 0xFFFFFFFF);

        vm->_pc = 0;
        vm->_regs[10] = 0x80000000;
        vm->_regs[11] = 0xFFFFFFFF;
        // div x12, x10, x11
        vm_exec(vm, 0x02b54633);
        ASSERT_EQ(vm->_regs[12], 0x80000000);

        vm->_pc = 0;
        vm->_regs[10] = 0x00000000;
        vm->_regs[11] = 0x00001750;
        // div x12, x10, x11
        vm_exec(vm, 0x02b54633);
        ASSERT_EQ(vm->_regs[12], 0x00000000);

        // DIVU

        vm->_pc = 0;
        vm->_regs[10] = 0x00000799;
        vm->_regs[11] = 0x00001750;
        // divu x12, x10, x11
        vm_exec(vm, 0x02b55633);
        ASSERT_EQ(vm->_regs[12], 0x00000000);

        vm->_pc = 0;
        vm->_regs[10] = 0xFFFF1799;
        vm->_regs[11] = 0x00000010;
        // divu x12, x10, x11
        vm_exec(vm, 0x02b55633);
        ASSERT_EQ(vm->_regs[12], 0x0FFFF179);

        vm->_pc = 0;
        vm->_regs[10] = 0xFFFF1799;
        vm->_regs[11] = 0x00000000;
        // divu x12, x10, x11
        vm_exec(vm, 0x02b55633);
        ASSERT_EQ(vm->_regs[12], 0xFFFFFFFF);

        vm->_pc = 0;
        vm->_regs[10] = 0xFFFFFFFF;
        vm->_regs[11] = 0x00000001;
        // divu x12, x10, x11
        vm_exec(vm, 0x02b55633);
        ASSERT_EQ(vm->_regs[12], 0xFFFFFFFF);

        vm->_pc = 0;
        vm->_regs[10] = 0x00000000;
        vm->_regs[11] = 0xFFFFFFFF;
        // divu x12, x10, x11
        vm_exec(vm, 0x02b55633);
        ASSERT_EQ(vm->_regs[12], 0x00000000);

        // REM

        vm->_pc = 0;
        vm->_regs[10] = 0x00001750;
        vm->_regs[11] = 0x00000799;
        // rem x12, x10, x11
        vm_exec(vm, 0x02b56633);
        ASSERT_EQ(vm->_regs[12], 0x00000085);

        vm->_pc = 0;
        vm->_regs[10] = 0xFFFFF6B0;
        vm->_regs[11] = 0x00000005;
        // rem x12, x10, x11
        vm_exec(vm, 0x02b56633);
        ASSERT_EQ(vm->_regs[12], 0xFFFFFFFC);

        vm->_pc = 0;
        vm->_regs[10] = 0x00000011;
        vm->_regs[11] = 0xFFFFFFFB;
        // rem x12, x10, x11
        vm_exec(vm, 0x02b56633);
        ASSERT_EQ(vm->_regs[12], 0x00000002);

        vm->_pc = 0;
        vm->_regs[10] = 0x00001750;
        vm->_regs[11] = 0x00000000;
        // rem x12, x10, x11
        vm_exec(vm, 0x02b56633);
        ASSERT_EQ(vm->_regs[12], 0x00001750);

        vm->_pc = 0;
        vm->_regs[10] = 0x80000000;
        vm->_regs[11] = 0xFFFFFFFF;
        // rem x12, x10, x11
        vm_exec(vm, 0x02b56633);
        ASSERT_EQ(vm->_regs[12], 0x00000000);

        vm->_pc = 0;
        vm->_regs[10] = 0x00000064;
        vm->_regs[11] = 0x00000005;
        // rem x12, x10, x11
        vm_exec(vm, 0x02b56633);
        ASSERT_EQ(vm->_regs[12], 0x00000000);

        // REMU

        vm->_pc = 0;
        vm->_regs[10] = 0x00001750;
        vm->_regs[11] = 0x00000799;
        // remu x12, x10, x11
        vm_exec(vm, 0x02b57633);
        ASSERT_EQ(vm->_regs[12], 0x00000085);

        vm->_pc = 0;
        vm->_regs[10] = 0xFFFF1799;
        vm->_regs[11] = 0x00000010;
        // remu x12, x10, x11
        vm_exec(vm, 0x02b57633);
        ASSERT_EQ(vm->_regs[12], 0x00000009);

        vm->_pc = 0;
        vm->_regs[10] = 0xFFFF1799;
        vm->_regs[11] = 0x00000000;
        // remu x12, x10, x11
        vm_exec(vm, 0x02b57633);
        ASSERT_EQ(vm->_regs[12], 0xFFFF1799);

        vm->_pc = 0;
        vm->_regs[10] = 0x00000799;
        vm->_regs[11] = 0x00001750;
        // remu x12, x10, x11
        vm_exec(vm, 0x02b57633);
        ASSERT_EQ(vm->_regs[12], 0x00000799);

        vm->_pc = 0;
        vm->_regs[10] = 0xFFFFFFFF;
        vm->_regs[11] = 0xFFFFFFFF;
        // remu x12, x10, x11
        vm_exec(vm, 0x02b57633);
        ASSERT_EQ(vm->_regs[12], 0x00000000);

        return 0;
}

#include "test_utils.h"
#include <stdint.h>

int main(void) {
        SETUP_VM(5);

        vm->_pc = 0;
        vm->_regs[1] = 0;

        vm->_regs[3] = 0x01;
        // sb x3, 0(x1)
        vm_exec(vm, 0x00308023);

        // lbu x2, 0(x1)
        vm_exec(vm, 0x0000c103);
        ASSERT_EQ(vm->_regs[2], 0x01);

        vm->_regs[3] = -0x02;
        // sb x3, 4(x1)
        vm_exec(vm, 0x00308223);

        // lb x2, 4(x1)
        vm_exec(vm, 0x00408103);
        ASSERT_EQ(vm->_regs[2], -(uint32_t)0x02)

        // lbu x2, 4(x1)
        vm_exec(vm, 0x0040c103);
        ASSERT_EQ(vm->_regs[2], 0xFE);

        vm->_pc = 0;
        vm->_regs[1] = 16;

        vm->_regs[3] = 0x03;
        // sb x3, -8(x1)
        vm_exec(vm, 0xfe308c23);

        // lb x2, -8(x1)
        vm_exec(vm, 0xff808103);
        ASSERT_EQ(vm->_regs[2], 0x03);

        vm->_regs[3] = -0x04;
        // sb x3, -4(x1)
        vm_exec(vm, 0xfe308e23);

        // lb x2, -4(x1)
        vm_exec(vm, 0xffc08103);
        ASSERT_EQ(vm->_regs[2], -(uint32_t)0x04)

        // lbu x2, -4(x1)
        vm_exec(vm, 0xffc0c103);
        ASSERT_EQ(vm->_regs[2], 0xFC);

        vm->_pc = 0;
        vm->_regs[1] = 20;

        vm->_regs[3] = 0x6666;
        // sh x3, -4(x1)
        vm_exec(vm, 0xfe309e23);

        // lhu x2, -4(x1)
        vm_exec(vm, 0xffc0d103);
        ASSERT_EQ(vm->_regs[2], 0x6666);

        vm->_regs[3] = -0x7999;
        // sh x3, 0(x1)
        vm_exec(vm, 0x00309023);

        // lh x2, 0(x1)
        vm_exec(vm, 0x00009103);
        ASSERT_EQ(vm->_regs[2], -(uint32_t)0x7999);

        // lhu x2, 0(x1)
        vm_exec(vm, 0x0000d103);
        ASSERT_EQ(vm->_regs[2], 0x8667);

        vm->_pc = 0;
        vm->_regs[1] = 24;

        vm->_regs[3] = 0xdeadbeef;
        // sw x3, 0(x1)
        vm_exec(vm, 0x0030a023);

        // lw x2, 0(x1)
        vm_exec(vm, 0x0000a103);
        ASSERT_EQ(vm->_regs[2], 0xdeadbeef);

        return 0;
}

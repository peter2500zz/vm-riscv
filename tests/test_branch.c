#include "test_utils.h"

int main(void) {
        SETUP_VM(4);

        // BEQ

        vm->_pc = 0;
        vm->_regs[1] = 4;
        vm->_regs[2] = 4;
        vm->_regs[3] = 0;

        // beq x1, x2, 8
        vm_exec(vm, 0x00208463);
        ASSERT_EQ(vm->_pc, 8);

        // beq x1, x3, -8
        vm_exec(vm, 0xfe308ce3);
        ASSERT_EQ(vm->_pc, 12)

        vm->_pc = 0;

        // addi x3, x0, 4
        vm_exec(vm, 0x00400193);
        // beq x1, x3, -4
        vm_exec(vm, 0xfe308ee3);
        ASSERT_EQ(vm->_pc, 0);

        // BNE

        vm->_pc = 0;
        vm->_regs[1] = 0;
        vm->_regs[2] = 4;
        vm->_regs[3] = 0;

        // bne x1, x2, 8
        vm_exec(vm, 0x00209463);
        ASSERT_EQ(vm->_pc, 8);

        // bne x1, x3, -8
        vm_exec(vm, 0xfe309ce3);
        ASSERT_EQ(vm->_pc, 12)

        vm->_pc = 0;

        // addi x3, x0, 4
        vm_exec(vm, 0x00400193);
        // bne x1, x3, -4
        vm_exec(vm, 0xfe309ee3);
        ASSERT_EQ(vm->_pc, 0);

        // BLT

        vm->_pc = 0;
        vm->_regs[1] = 4;
        vm->_regs[2] = 8;
        vm->_regs[3] = 0;

        // blt x1, x2, 8
        vm_exec(vm, 0x0020c463);
        ASSERT_EQ(vm->_pc, 8);

        // blt x1, x3, -8
        vm_exec(vm, 0xfe30cce3);
        ASSERT_EQ(vm->_pc, 12)

        vm->_pc = 0;

        // addi x3, x0, 8
        vm_exec(vm, 0x00800193);
        // blt x1, x3, -4
        vm_exec(vm, 0xfe30cee3);
        ASSERT_EQ(vm->_pc, 0);

        // addi x3, x0, -4
        vm_exec(vm, 0xffc00193);
        // blt x1, x3, -4
        vm_exec(vm, 0xfe30cee3);
        ASSERT_EQ(vm->_pc, 8);

        // BLTU

        vm->_pc = 0;
        vm->_regs[1] = 4;
        vm->_regs[2] = 8;
        vm->_regs[3] = 0;

        // bltu x1, x2, 8
        vm_exec(vm, 0x0020e463);
        ASSERT_EQ(vm->_pc, 8);

        // bltu x1, x3, -8
        vm_exec(vm, 0xfe30ece3);
        ASSERT_EQ(vm->_pc, 12)

        vm->_pc = 0;

        // addi x3, x0, 8
        vm_exec(vm, 0x00800193);
        // bltu x1, x3, -4
        vm_exec(vm, 0xfe30eee3);
        ASSERT_EQ(vm->_pc, 0);

        // addi x3, x0, -4
        vm_exec(vm, 0xffc00193);
        // bltu x1, x3, -4
        vm_exec(vm, 0xfe30eee3);
        ASSERT_EQ(vm->_pc, 0);

        // BGE

        vm->_pc = 0;
        vm->_regs[1] = 8;
        vm->_regs[2] = 4;
        vm->_regs[3] = 16;

        // bge x1, x2, 8
        vm_exec(vm, 0x0020d463);
        ASSERT_EQ(vm->_pc, 8);

        // bge x1, x3, -8
        vm_exec(vm, 0xfe30dce3);
        ASSERT_EQ(vm->_pc, 12)

        vm->_pc = 0;

        // addi x3, x0, 8
        vm_exec(vm, 0x00800193);
        // bge x1, x3, -4
        vm_exec(vm, 0xfe30dee3);
        ASSERT_EQ(vm->_pc, 0);

        // addi x3, x0, -4
        vm_exec(vm, 0xffc00193);
        // bge x1, x3, -4
        vm_exec(vm, 0xfe30dee3);
        ASSERT_EQ(vm->_pc, 0);

        // BGEU

        vm->_pc = 0;
        vm->_regs[1] = 8;
        vm->_regs[2] = 4;
        vm->_regs[3] = 16;

        // bgeu x1, x2, 8
        vm_exec(vm, 0x0020f463);
        ASSERT_EQ(vm->_pc, 8);

        // bgeu x1, x3, -8
        vm_exec(vm, 0xfe30fce3);
        ASSERT_EQ(vm->_pc, 12)

        vm->_pc = 0;

        // addi x3, x0, 8
        vm_exec(vm, 0x00800193);
        // bgeu x1, x3, -4
        vm_exec(vm, 0xfe30fee3);
        ASSERT_EQ(vm->_pc, 0);

        // addi x3, x0, -4
        vm_exec(vm, 0xffc00193);
        // bgeu x1, x3, -4
        vm_exec(vm, 0xfe30fee3);
        ASSERT_EQ(vm->_pc, 8);

        return 0;
}

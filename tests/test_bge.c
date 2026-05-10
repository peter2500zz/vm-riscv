#include "test_utils.h"

int main(void) {
        SETUP_VM(4);

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

        return 0;
}

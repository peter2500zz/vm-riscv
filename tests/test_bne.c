#include "test_utils.h"

int main(void) {
        SETUP_VM(4);

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

        return 0;
}

#include "test_utils.h"

int main(void) {
        SETUP_VM(4);

        // addi x1, x0, 32
        vm_exec(vm, 0x02000093);
        ASSERT_EQ(vm->_regs[1], 32);

        // addi x1, x1, 96
        vm_exec(vm, 0x06008093);
        ASSERT_EQ(vm->_regs[1], 128);

        // addi x2, x1, -64
        vm_exec(vm, 0xfc008113);
        ASSERT_EQ(vm->_regs[1], 128);
        ASSERT_EQ(vm->_regs[2], 64);

        return 0;
}

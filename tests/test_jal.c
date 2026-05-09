#include "test_utils.h"

int main(void) {
        SETUP_VM(4);

        vm->_pc = 0;

        // jal x1, 8
        vm_exec(vm, 0x008000ef);
        ASSERT_EQ(vm->_pc, 8);
        ASSERT_EQ(vm->_regs[1], 4);

        return 0;
}

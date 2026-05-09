#include "test_utils.h"

int main(void) {
        SETUP_VM(4);

        vm->_pc = 0;
        vm->_regs[1] = 4;

        // jalr x2, x1, 4
        vm_exec(vm, 0x00408167);
        ASSERT_EQ(vm->_pc, 8);
        ASSERT_EQ(vm->_regs[2], 4);

        // jalr x2, x2, 8
        vm_exec(vm, 0x00810167);
        ASSERT_EQ(vm->_pc, 12);
        ASSERT_EQ(vm->_regs[2], 12);

        return 0;
}

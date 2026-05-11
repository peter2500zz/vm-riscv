#include "test_utils.h"

int main(void) {
        SETUP_VM(4);

        vm->_pc = 0;

        // auipc x1, 8
        vm_dispatch(vm, 0x00008097);
        ASSERT_EQ(vm->_regs[1], 8 << 12);

        vm->_pc = 4;

        // auipc x1, 4
        vm_dispatch(vm, 0x00004097);
        ASSERT_EQ(vm->_regs[1], (4 << 12) + 4);

        vm->_pc = 0;

        // auipc x2, 12
        vm_dispatch(vm, 0x0000c117);
        ASSERT_EQ(vm->_regs[1], (4 << 12) + 4);
        ASSERT_EQ(vm->_regs[2], 12 << 12);

        return 0;
}

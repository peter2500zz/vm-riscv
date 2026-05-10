#include "test_utils.h"

int main(void) {
        SETUP_VM(4);

        vm->_pc = 0;
        vm->_regs[1] = 4;
        vm->_regs[2] = 8;
        vm->_regs[3] = 0;

        // bltu x1, x2, 8
        vm_exec(vm, 	0x0020e463);
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

        return 0;
}

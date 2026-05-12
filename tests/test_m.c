#include "../src/dispatcher/root.h"
#include "../src/vm/hart/unprivileged.h"
#include "test_utils.h"
#include <stdint.h>
#include <stdlib.h>

int main() {
        // ==== 创建独立 Hart ====
        uint32_t mem_size = 1024;
        uint8_t *mem = calloc(1, mem_size);
        Hart *hart = calloc(1, sizeof(Hart));
        hart_init(hart, mem, mem_size);

        // MUL
        hart->_pc = 0;

        hart->_regs[10] = 0x799;
        hart->_regs[11] = 0x1750;
        // mul x12, x10, x11
        hart_dispatch(hart, 0x02b50633);
        ASSERT_EQ(hart->_regs[12], 0x00b11ed0);

        // MULH
        hart->_pc = 0;

        hart->_regs[10] = 0x799;
        hart->_regs[11] = 0x79991750;
        // mulh x12, x10, x11
        hart_dispatch(hart, 0x02b51633);
        ASSERT_EQ(hart->_regs[12], 0x39b);

        // MULHSU
        hart->_pc = 0;

        hart->_regs[10] = 0xffff1799;
        hart->_regs[11] = 0xffff1750;
        // mulhsu x12, x10, x11
        hart_dispatch(hart, 0x02b52633);
        ASSERT_EQ(hart->_regs[12], 0xffff1799);

        // MULHU
        hart->_pc = 0;

        hart->_regs[10] = 0xffff1799;
        hart->_regs[11] = 0xffff1750;
        // mulhu x12, x10, x11
        hart_dispatch(hart, 0x02b53633);
        ASSERT_EQ(hart->_regs[12], 0xfffe2ee9);

        // DIV

        hart->_pc = 0;
        hart->_regs[10] = 0x00001750;
        hart->_regs[11] = 0x00000799;
        // div x12, x10, x11
        hart_dispatch(hart, 0x02b54633);
        ASSERT_EQ(hart->_regs[12], 0x00000003);

        hart->_pc = 0;
        hart->_regs[10] = 0xFFFFF6B0;
        hart->_regs[11] = 0x00000005;
        // div x12, x10, x11
        hart_dispatch(hart, 0x02b54633);
        ASSERT_EQ(hart->_regs[12], 0xfffffe24);

        hart->_pc = 0;
        hart->_regs[10] = 0xfffffee8;
        hart->_regs[11] = 0xFFFFFFFC;
        // div x12, x10, x11
        hart_dispatch(hart, 0x02b54633);
        ASSERT_EQ(hart->_regs[12], 0x00000046);

        hart->_pc = 0;
        hart->_regs[10] = 0x00001750;
        hart->_regs[11] = 0x00000000;
        // div x12, x10, x11
        hart_dispatch(hart, 0x02b54633);
        ASSERT_EQ(hart->_regs[12], 0xFFFFFFFF);

        hart->_pc = 0;
        hart->_regs[10] = 0x80000000;
        hart->_regs[11] = 0xFFFFFFFF;
        // div x12, x10, x11
        hart_dispatch(hart, 0x02b54633);
        ASSERT_EQ(hart->_regs[12], 0x80000000);

        hart->_pc = 0;
        hart->_regs[10] = 0x00000000;
        hart->_regs[11] = 0x00001750;
        // div x12, x10, x11
        hart_dispatch(hart, 0x02b54633);
        ASSERT_EQ(hart->_regs[12], 0x00000000);

        // DIVU

        hart->_pc = 0;
        hart->_regs[10] = 0x00000799;
        hart->_regs[11] = 0x00001750;
        // divu x12, x10, x11
        hart_dispatch(hart, 0x02b55633);
        ASSERT_EQ(hart->_regs[12], 0x00000000);

        hart->_pc = 0;
        hart->_regs[10] = 0xFFFF1799;
        hart->_regs[11] = 0x00000010;
        // divu x12, x10, x11
        hart_dispatch(hart, 0x02b55633);
        ASSERT_EQ(hart->_regs[12], 0x0FFFF179);

        hart->_pc = 0;
        hart->_regs[10] = 0xFFFF1799;
        hart->_regs[11] = 0x00000000;
        // divu x12, x10, x11
        hart_dispatch(hart, 0x02b55633);
        ASSERT_EQ(hart->_regs[12], 0xFFFFFFFF);

        hart->_pc = 0;
        hart->_regs[10] = 0xFFFFFFFF;
        hart->_regs[11] = 0x00000001;
        // divu x12, x10, x11
        hart_dispatch(hart, 0x02b55633);
        ASSERT_EQ(hart->_regs[12], 0xFFFFFFFF);

        hart->_pc = 0;
        hart->_regs[10] = 0x00000000;
        hart->_regs[11] = 0xFFFFFFFF;
        // divu x12, x10, x11
        hart_dispatch(hart, 0x02b55633);
        ASSERT_EQ(hart->_regs[12], 0x00000000);

        // REM

        hart->_pc = 0;
        hart->_regs[10] = 0x00001750;
        hart->_regs[11] = 0x00000799;
        // rem x12, x10, x11
        hart_dispatch(hart, 0x02b56633);
        ASSERT_EQ(hart->_regs[12], 0x00000085);

        hart->_pc = 0;
        hart->_regs[10] = 0xFFFFF6B0;
        hart->_regs[11] = 0x00000005;
        // rem x12, x10, x11
        hart_dispatch(hart, 0x02b56633);
        ASSERT_EQ(hart->_regs[12], 0xFFFFFFFC);

        hart->_pc = 0;
        hart->_regs[10] = 0x00000011;
        hart->_regs[11] = 0xFFFFFFFB;
        // rem x12, x10, x11
        hart_dispatch(hart, 0x02b56633);
        ASSERT_EQ(hart->_regs[12], 0x00000002);

        hart->_pc = 0;
        hart->_regs[10] = 0x00001750;
        hart->_regs[11] = 0x00000000;
        // rem x12, x10, x11
        hart_dispatch(hart, 0x02b56633);
        ASSERT_EQ(hart->_regs[12], 0x00001750);

        hart->_pc = 0;
        hart->_regs[10] = 0x80000000;
        hart->_regs[11] = 0xFFFFFFFF;
        // rem x12, x10, x11
        hart_dispatch(hart, 0x02b56633);
        ASSERT_EQ(hart->_regs[12], 0x00000000);

        hart->_pc = 0;
        hart->_regs[10] = 0x00000064;
        hart->_regs[11] = 0x00000005;
        // rem x12, x10, x11
        hart_dispatch(hart, 0x02b56633);
        ASSERT_EQ(hart->_regs[12], 0x00000000);

        // REMU

        hart->_pc = 0;
        hart->_regs[10] = 0x00001750;
        hart->_regs[11] = 0x00000799;
        // remu x12, x10, x11
        hart_dispatch(hart, 0x02b57633);
        ASSERT_EQ(hart->_regs[12], 0x00000085);

        hart->_pc = 0;
        hart->_regs[10] = 0xFFFF1799;
        hart->_regs[11] = 0x00000010;
        // remu x12, x10, x11
        hart_dispatch(hart, 0x02b57633);
        ASSERT_EQ(hart->_regs[12], 0x00000009);

        hart->_pc = 0;
        hart->_regs[10] = 0xFFFF1799;
        hart->_regs[11] = 0x00000000;
        // remu x12, x10, x11
        hart_dispatch(hart, 0x02b57633);
        ASSERT_EQ(hart->_regs[12], 0xFFFF1799);

        hart->_pc = 0;
        hart->_regs[10] = 0x00000799;
        hart->_regs[11] = 0x00001750;
        // remu x12, x10, x11
        hart_dispatch(hart, 0x02b57633);
        ASSERT_EQ(hart->_regs[12], 0x00000799);

        hart->_pc = 0;
        hart->_regs[10] = 0xFFFFFFFF;
        hart->_regs[11] = 0xFFFFFFFF;
        // remu x12, x10, x11
        hart_dispatch(hart, 0x02b57633);
        ASSERT_EQ(hart->_regs[12], 0x00000000);

        // ==== 释放资源 ====
        free(hart);
        free(mem);

        return 0;
}

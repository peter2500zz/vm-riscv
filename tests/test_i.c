#include "../src/vm/cpu/dispatcher/root.h"
#include "../src/vm/memory/memory.h"
#include "test_utils.h"
#include <stdint.h>
#include <stdlib.h>

int main() {
        // ==== 创建独立 Hart ====
        Memory *mem = newMemory(1024);
        Hart *hart = calloc(1, sizeof(Hart));
        hart_init(hart, mem);

        hart->_pc = 0;

        // auipc x1, 8
        hart_dispatch(hart, 0x00008097);
        ASSERT_EQ(hart->_regs[1], 8 << 12);

        hart->_pc = 4;

        // auipc x1, 4
        hart_dispatch(hart, 0x00004097);
        ASSERT_EQ(hart->_regs[1], (4 << 12) + 4);

        hart->_pc = 0;

        // auipc x2, 12
        hart_dispatch(hart, 0x0000c117);
        ASSERT_EQ(hart->_regs[1], (4 << 12) + 4);
        ASSERT_EQ(hart->_regs[2], 12 << 12);

        // BEQ

        hart->_pc = 0;
        hart->_regs[1] = 4;
        hart->_regs[2] = 4;
        hart->_regs[3] = 0;

        // beq x1, x2, 8
        hart_dispatch(hart, 0x00208463);
        ASSERT_EQ(hart->_pc, 8);

        // beq x1, x3, -8
        hart_dispatch(hart, 0xfe308ce3);
        ASSERT_EQ(hart->_pc, 12)

        hart->_pc = 0;

        // addi x3, x0, 4
        hart_dispatch(hart, 0x00400193);
        // beq x1, x3, -4
        hart_dispatch(hart, 0xfe308ee3);
        ASSERT_EQ(hart->_pc, 0);

        // BNE

        hart->_pc = 0;
        hart->_regs[1] = 0;
        hart->_regs[2] = 4;
        hart->_regs[3] = 0;

        // bne x1, x2, 8
        hart_dispatch(hart, 0x00209463);
        ASSERT_EQ(hart->_pc, 8);

        // bne x1, x3, -8
        hart_dispatch(hart, 0xfe309ce3);
        ASSERT_EQ(hart->_pc, 12)

        hart->_pc = 0;

        // addi x3, x0, 4
        hart_dispatch(hart, 0x00400193);
        // bne x1, x3, -4
        hart_dispatch(hart, 0xfe309ee3);
        ASSERT_EQ(hart->_pc, 0);

        // BLT

        hart->_pc = 0;
        hart->_regs[1] = 4;
        hart->_regs[2] = 8;
        hart->_regs[3] = 0;

        // blt x1, x2, 8
        hart_dispatch(hart, 0x0020c463);
        ASSERT_EQ(hart->_pc, 8);

        // blt x1, x3, -8
        hart_dispatch(hart, 0xfe30cce3);
        ASSERT_EQ(hart->_pc, 12)

        hart->_pc = 0;

        // addi x3, x0, 8
        hart_dispatch(hart, 0x00800193);
        // blt x1, x3, -4
        hart_dispatch(hart, 0xfe30cee3);
        ASSERT_EQ(hart->_pc, 0);

        // addi x3, x0, -4
        hart_dispatch(hart, 0xffc00193);
        // blt x1, x3, -4
        hart_dispatch(hart, 0xfe30cee3);
        ASSERT_EQ(hart->_pc, 8);

        // BLTU

        hart->_pc = 0;
        hart->_regs[1] = 4;
        hart->_regs[2] = 8;
        hart->_regs[3] = 0;

        // bltu x1, x2, 8
        hart_dispatch(hart, 0x0020e463);
        ASSERT_EQ(hart->_pc, 8);

        // bltu x1, x3, -8
        hart_dispatch(hart, 0xfe30ece3);
        ASSERT_EQ(hart->_pc, 12)

        hart->_pc = 0;

        // addi x3, x0, 8
        hart_dispatch(hart, 0x00800193);
        // bltu x1, x3, -4
        hart_dispatch(hart, 0xfe30eee3);
        ASSERT_EQ(hart->_pc, 0);

        // addi x3, x0, -4
        hart_dispatch(hart, 0xffc00193);
        // bltu x1, x3, -4
        hart_dispatch(hart, 0xfe30eee3);
        ASSERT_EQ(hart->_pc, 0);

        // BGE

        hart->_pc = 0;
        hart->_regs[1] = 8;
        hart->_regs[2] = 4;
        hart->_regs[3] = 16;

        // bge x1, x2, 8
        hart_dispatch(hart, 0x0020d463);
        ASSERT_EQ(hart->_pc, 8);

        // bge x1, x3, -8
        hart_dispatch(hart, 0xfe30dce3);
        ASSERT_EQ(hart->_pc, 12)

        hart->_pc = 0;

        // addi x3, x0, 8
        hart_dispatch(hart, 0x00800193);
        // bge x1, x3, -4
        hart_dispatch(hart, 0xfe30dee3);
        ASSERT_EQ(hart->_pc, 0);

        // addi x3, x0, -4
        hart_dispatch(hart, 0xffc00193);
        // bge x1, x3, -4
        hart_dispatch(hart, 0xfe30dee3);
        ASSERT_EQ(hart->_pc, 0);

        // BGEU

        hart->_pc = 0;
        hart->_regs[1] = 8;
        hart->_regs[2] = 4;
        hart->_regs[3] = 16;

        // bgeu x1, x2, 8
        hart_dispatch(hart, 0x0020f463);
        ASSERT_EQ(hart->_pc, 8);

        // bgeu x1, x3, -8
        hart_dispatch(hart, 0xfe30fce3);
        ASSERT_EQ(hart->_pc, 12)

        hart->_pc = 0;

        // addi x3, x0, 8
        hart_dispatch(hart, 0x00800193);
        // bgeu x1, x3, -4
        hart_dispatch(hart, 0xfe30fee3);
        ASSERT_EQ(hart->_pc, 0);

        // addi x3, x0, -4
        hart_dispatch(hart, 0xffc00193);
        // bgeu x1, x3, -4
        hart_dispatch(hart, 0xfe30fee3);
        ASSERT_EQ(hart->_pc, 8);

        // JAL

        hart->_pc = 0;

        // jal x1, 8
        hart_dispatch(hart, 0x008000ef);
        ASSERT_EQ(hart->_pc, 8);
        ASSERT_EQ(hart->_regs[1], 4);

        // JALR

        hart->_pc = 0;
        hart->_regs[1] = 4;

        // jalr x2, x1, 4
        hart_dispatch(hart, 0x00408167);
        ASSERT_EQ(hart->_pc, 8);
        ASSERT_EQ(hart->_regs[2], 4);

        // jalr x2, x2, 8
        hart_dispatch(hart, 0x00810167);
        ASSERT_EQ(hart->_pc, 12);
        ASSERT_EQ(hart->_regs[2], 12);

        // lui x1, 1048575
        hart_dispatch(hart, 0xfffff0b7);
        ASSERT_EQ(hart->_regs[1], 1048575 << 12);

        // lui x1, 0
        hart_dispatch(hart, 0x000000b7);
        ASSERT_EQ(hart->_regs[1], 0);

        // lui x2, 2500
        hart_dispatch(hart, 0x009c4137);
        ASSERT_EQ(hart->_regs[1], 0);
        ASSERT_EQ(hart->_regs[2], 2500 << 12);

        // ANDI

        hart->_pc = 0;

        // addi x1, x0, 32
        hart_dispatch(hart, 0x02000093);
        ASSERT_EQ(hart->_regs[1], 32);

        // addi x1, x1, 96
        hart_dispatch(hart, 0x06008093);
        ASSERT_EQ(hart->_regs[1], 128);

        // addi x2, x1, -64
        hart_dispatch(hart, 0xfc008113);
        ASSERT_EQ(hart->_regs[1], 128);
        ASSERT_EQ(hart->_regs[2], 64);

        // SLTI

        hart->_pc = 0;
        hart->_regs[2] = 50;

        // slti x1, x2, 100
        hart_dispatch(hart, 0x06412093);
        ASSERT_EQ(hart->_regs[1], 1);

        // slti x1, x2, 25
        hart_dispatch(hart, 0x01912093);
        ASSERT_EQ(hart->_regs[1], 0);

        // slti x1, x2, 50
        hart_dispatch(hart, 0x03212093);
        ASSERT_EQ(hart->_regs[1], 0);

        hart->_pc = 0;
        hart->_regs[2] = 0;

        // slti x1, x2, 100
        hart_dispatch(hart, 0x06412093);
        ASSERT_EQ(hart->_regs[1], 1);

        // slti x1, x2, -75
        hart_dispatch(hart, 0xfb512093);
        ASSERT_EQ(hart->_regs[1], 0);

        // slti x1, x2, 0
        hart_dispatch(hart, 0x00012093);
        ASSERT_EQ(hart->_regs[1], 0);

        // SLTIU

        hart->_pc = 0;
        hart->_regs[2] = 50;

        // sltiu x1, x2, 100
        hart_dispatch(hart, 0x06413093);
        ASSERT_EQ(hart->_regs[1], 1);

        // sltiu x1, x2, 25
        hart_dispatch(hart, 0x01913093);
        ASSERT_EQ(hart->_regs[1], 0);

        // sltiu x1, x2, 50
        hart_dispatch(hart, 0x03213093);
        ASSERT_EQ(hart->_regs[1], 0);

        hart->_pc = 0;
        hart->_regs[2] = 0;

        // sltiu x1, x2, 100
        hart_dispatch(hart, 0x06413093);
        ASSERT_EQ(hart->_regs[1], 1);

        // sltiu x1, x2, -75
        hart_dispatch(hart, 0xfb513093);
        ASSERT_EQ(hart->_regs[1], 1);

        // sltiu x1, x2, 0
        hart_dispatch(hart, 0x00013093);
        ASSERT_EQ(hart->_regs[1], 0);

        // XORI

        hart->_pc = 0;
        hart->_regs[2] = 0xdeadbeef;

        // xori x1, x2, 1750
        hart_dispatch(hart, 0x6d614093);
        ASSERT_EQ(hart->_regs[1], 0xDEADB839);

        // xori x1, x2, -799
        hart_dispatch(hart, 0xce114093);
        ASSERT_EQ(hart->_regs[1], 0x2152420E);

        // ORI

        hart->_pc = 0;
        hart->_regs[2] = 0xdeadbeef;

        // ori x1, x2, 1750
        hart_dispatch(hart, 0x6d616093);
        ASSERT_EQ(hart->_regs[1], 0xDEADBEFF);

        // ori x1, x2, -799
        hart_dispatch(hart, 0xce116093);
        ASSERT_EQ(hart->_regs[1], 0xFFFFFEEF);

        // ANDI

        hart->_pc = 0;
        hart->_regs[2] = 0xdeadbeef;

        // andi x1, x2, 1750
        hart_dispatch(hart, 0x6d617093);
        ASSERT_EQ(hart->_regs[1], 0x6C6);

        // andi x1, x2, -799
        hart_dispatch(hart, 0xce117093);
        ASSERT_EQ(hart->_regs[1], 0xDEADBCE1);

        // SLLI

        hart->_pc = 0;
        hart->_regs[2] = 0xdeadbeef;

        // slli x1, x2, 4
        hart_dispatch(hart, 0x00411093);
        ASSERT_EQ(hart->_regs[1], 0xeadbeef0);

        // SRLI

        hart->_pc = 0;
        hart->_regs[2] = 0xdeadbeef;

        // srli x1, x2, 4
        hart_dispatch(hart, 0x00415093);
        ASSERT_EQ(hart->_regs[1], 0x0deadbee);

        // SRAI

        hart->_pc = 0;
        hart->_regs[2] = 0xdeadbeef;

        // srai x1, x2, 4
        hart_dispatch(hart, 0x40415093);
        ASSERT_EQ(hart->_regs[1], 0xfdeadbee);

        // AND

        hart->_pc = 0;

        hart->_regs[4] = 32;
        // add x1, x0, x4
        hart_dispatch(hart, 0x004000b3);
        ASSERT_EQ(hart->_regs[1], 32);

        hart->_regs[4] = 96;
        // add x1, x1, x4
        hart_dispatch(hart, 0x004080b3);
        ASSERT_EQ(hart->_regs[1], 128);

        hart->_regs[4] = -64;
        // add x2, x1, x4
        hart_dispatch(hart, 0x00408133);
        ASSERT_EQ(hart->_regs[1], 128);
        ASSERT_EQ(hart->_regs[2], 64);

        // SLT

        hart->_pc = 0;
        hart->_regs[2] = 50;

        hart->_regs[4] = 100;
        // slt x1, x2, x4
        hart_dispatch(hart, 0x004120b3);
        ASSERT_EQ(hart->_regs[1], 1);

        hart->_regs[4] = 25;
        // slt x1, x2, x4
        hart_dispatch(hart, 0x004120b3);
        ASSERT_EQ(hart->_regs[1], 0);

        hart->_regs[4] = 50;
        // slt x1, x2, x4
        hart_dispatch(hart, 0x004120b3);
        ASSERT_EQ(hart->_regs[1], 0);

        hart->_pc = 0;
        hart->_regs[2] = 0;

        hart->_regs[4] = 100;
        // slt x1, x2, x4
        hart_dispatch(hart, 0x004120b3);
        ASSERT_EQ(hart->_regs[1], 1);

        hart->_regs[4] = -75;
        // slt x1, x2, x4
        hart_dispatch(hart, 0x004120b3);
        ASSERT_EQ(hart->_regs[1], 0);

        hart->_regs[4] = 0;
        // slt x1, x2, x4
        hart_dispatch(hart, 0x004120b3);
        ASSERT_EQ(hart->_regs[1], 0);

        // SLTU

        hart->_pc = 0;
        hart->_regs[2] = 50;

        hart->_regs[4] = 100;
        // sltu x1, x2, x4
        hart_dispatch(hart, 0x004130b3);
        ASSERT_EQ(hart->_regs[1], 1);

        hart->_regs[4] = 25;
        // sltu x1, x2, x4
        hart_dispatch(hart, 0x004130b3);
        ASSERT_EQ(hart->_regs[1], 0);

        hart->_regs[4] = 50;
        // sltu x1, x2, x4
        hart_dispatch(hart, 0x004130b3);
        ASSERT_EQ(hart->_regs[1], 0);

        hart->_pc = 0;
        hart->_regs[2] = 0;

        hart->_regs[4] = 100;
        // sltu x1, x2, x4
        hart_dispatch(hart, 0x004130b3);
        ASSERT_EQ(hart->_regs[1], 1);

        hart->_regs[4] = -75;
        // sltu x1, x2, x4
        hart_dispatch(hart, 0x004130b3);
        ASSERT_EQ(hart->_regs[1], 1);

        hart->_regs[4] = 0;
        // sltu x1, x2, x4
        hart_dispatch(hart, 0x004130b3);
        ASSERT_EQ(hart->_regs[1], 0);

        // XOR

        hart->_pc = 0;
        hart->_regs[2] = 0xdeadbeef;

        hart->_regs[4] = 1750;
        // xor x1, x2, x4
        hart_dispatch(hart, 0x004140b3);
        ASSERT_EQ(hart->_regs[1], 0xDEADB839);

        hart->_regs[4] = -799;
        // xor x1, x2, x4
        hart_dispatch(hart, 0x004140b3);
        ASSERT_EQ(hart->_regs[1], 0x2152420E);

        // OR

        hart->_pc = 0;
        hart->_regs[2] = 0xdeadbeef;

        hart->_regs[4] = 1750;
        // or x1, x2, x4
        hart_dispatch(hart, 0x004160b3);
        ASSERT_EQ(hart->_regs[1], 0xDEADBEFF);

        hart->_regs[4] = -799;
        // or x1, x2, x4
        hart_dispatch(hart, 0x004160b3);
        ASSERT_EQ(hart->_regs[1], 0xFFFFFEEF);

        // AND

        hart->_pc = 0;
        hart->_regs[2] = 0xdeadbeef;

        hart->_regs[4] = 1750;
        // and x1, x2, x4
        hart_dispatch(hart, 0x004170b3);
        ASSERT_EQ(hart->_regs[1], 0x6C6);

        hart->_regs[4] = -799;
        // and x1, x2, x4
        hart_dispatch(hart, 0x004170b3);
        ASSERT_EQ(hart->_regs[1], 0xDEADBCE1);

        // SLL

        hart->_pc = 0;
        hart->_regs[2] = 0xdeadbeef;

        hart->_regs[4] = 4;
        // sll x1, x2, x4
        hart_dispatch(hart, 0x004110b3);
        ASSERT_EQ(hart->_regs[1], 0xeadbeef0);

        // SRL

        hart->_pc = 0;
        hart->_regs[2] = 0xdeadbeef;

        hart->_regs[4] = 4;
        // srl x1, x2, x4
        hart_dispatch(hart, 0x004150b3);
        ASSERT_EQ(hart->_regs[1], 0x0deadbee);

        // SRA

        hart->_pc = 0;
        hart->_regs[2] = 0xdeadbeef;

        hart->_regs[4] = 4;
        // sra x1, x2, x4
        hart_dispatch(hart, 0x404150b3);
        ASSERT_EQ(hart->_regs[1], 0xfdeadbee);

        /*

        hart->_pc = 0;
        hart->_regs[1] = 0;

        hart->_regs[3] = 0x01;
        // sb x3, 0(x1)
        hart_dispatch(hart, 0x00308023);

        // lbu x2, 0(x1)
        hart_dispatch(hart, 0x0000c103);
        ASSERT_EQ(hart->_regs[2], 0x01);

        hart->_regs[3] = -0x02;
        // sb x3, 4(x1)
        hart_dispatch(hart, 0x00308223);

        // lb x2, 4(x1)
        hart_dispatch(hart, 0x00408103);
        ASSERT_EQ(hart->_regs[2], -(uint32_t)0x02)

        // lbu x2, 4(x1)
        hart_dispatch(hart, 0x0040c103);
        ASSERT_EQ(hart->_regs[2], 0xFE);

        hart->_pc = 0;
        hart->_regs[1] = 16;

        hart->_regs[3] = 0x03;
        // sb x3, -8(x1)
        hart_dispatch(hart, 0xfe308c23);

        // lb x2, -8(x1)
        hart_dispatch(hart, 0xff808103);
        ASSERT_EQ(hart->_regs[2], 0x03);

        hart->_regs[3] = -0x04;
        // sb x3, -4(x1)
        hart_dispatch(hart, 0xfe308e23);

        // lb x2, -4(x1)
        hart_dispatch(hart, 0xffc08103);
        ASSERT_EQ(hart->_regs[2], -(uint32_t)0x04)

        // lbu x2, -4(x1)
        hart_dispatch(hart, 0xffc0c103);
        ASSERT_EQ(hart->_regs[2], 0xFC);

        hart->_pc = 0;
        hart->_regs[1] = 20;

        hart->_regs[3] = 0x6666;
        // sh x3, -4(x1)
        hart_dispatch(hart, 0xfe309e23);

        // lhu x2, -4(x1)
        hart_dispatch(hart, 0xffc0d103);
        ASSERT_EQ(hart->_regs[2], 0x6666);

        hart->_regs[3] = -0x7999;
        // sh x3, 0(x1)
        hart_dispatch(hart, 0x00309023);

        // lh x2, 0(x1)
        hart_dispatch(hart, 0x00009103);
        ASSERT_EQ(hart->_regs[2], -(uint32_t)0x7999);

        // lhu x2, 0(x1)
        hart_dispatch(hart, 0x0000d103);
        ASSERT_EQ(hart->_regs[2], 0x8667);

        hart->_pc = 0;
        hart->_regs[1] = 24;

        hart->_regs[3] = 0xdeadbeef;
        // sw x3, 0(x1)
        hart_dispatch(hart, 0x0030a023);

        // lw x2, 0(x1)
        hart_dispatch(hart, 0x0000a103);
        ASSERT_EQ(hart->_regs[2], 0xdeadbeef);

        */

        // ==== 释放资源 ====
        free(hart);
        freeMemory(&mem);

        return 0;
}

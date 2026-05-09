#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdint.h>

#define BITS(x, hi, lo) (((x) >> (lo)) & ((1u << ((hi) - (lo) + 1)) - 1))

typedef uint32_t Instruction;

uint32_t inst_opcode(Instruction inst);

#endif // INSTRUCTION_H

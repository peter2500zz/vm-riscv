#ifndef ZICSR_EXEC_H
#define ZICSR_EXEC_H

#include "../../vm.h"
#include "../../instruction.h"

void exec_csrrw(VM *vm, Instruction inst);
// void exec_csrrs(VM *vm, Instruction inst);
// void exec_csrrc(VM *vm, Instruction inst);
// void exec_csrrwi(VM *vm, Instruction inst);
// void exec_csrrsi(VM *vm, Instruction inst);
// void exec_csrrci(VM *vm, Instruction inst);

#endif // ZICSR_EXEC_H
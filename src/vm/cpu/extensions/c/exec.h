#ifndef C_EXEC_H
#define C_EXEC_H

#include "../../hart/unprivileged.h"

void exec_clwsp(Hart *hart, CInstruction inst);

void exec_cswsp(Hart *hart, CInstruction inst);

void exec_clw(Hart *hart, CInstruction inst);

void exec_csw(Hart *hart, CInstruction inst);

void exec_cj(Hart *hart, CInstruction inst);

void exec_cjal(Hart *hart, CInstruction inst);

void exec_cjr(Hart *hart, CInstruction inst);

void exec_cjalr(Hart *hart, CInstruction inst);

void exec_cbeqz(Hart *hart, CInstruction inst);

void exec_cbnez(Hart *hart, CInstruction inst);

void exec_cli(Hart *hart, CInstruction inst);

void exec_clui(Hart *hart, CInstruction inst);

void exec_caddi(Hart *hart, CInstruction inst);

void exec_caddi16sp(Hart *hart, CInstruction inst);

void exec_caddi4spn(Hart *hart, CInstruction inst);

void exec_cslli(Hart *hart, CInstruction inst);

void exec_csrli(Hart *hart, CInstruction inst);

void exec_csrai(Hart *hart, CInstruction inst);

void exec_candi(Hart *hart, CInstruction inst);

void exec_cmv(Hart *hart, CInstruction inst);

void exec_cadd(Hart *hart, CInstruction inst);

void exec_cand(Hart *hart, CInstruction inst);

void exec_cor(Hart *hart, CInstruction inst);

void exec_cxor(Hart *hart, CInstruction inst);

void exec_csub(Hart *hart, CInstruction inst);

#endif // C_EXEC_H

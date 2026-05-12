#ifndef DISPATCHER_OPIMM_H
#define DISPATCHER_OPIMM_H

#include "../hart/unprivileged.h"

void dispatch_opimm(Hart *hart, Instruction inst);

#endif // DISPATCHER_OPIMM_H

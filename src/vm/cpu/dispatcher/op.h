#ifndef DISPATCHER_OP_H
#define DISPATCHER_OP_H

#include "../hart/unprivileged.h"

void dispatch_op(Hart *hart, Instruction inst);

#endif // DISPATCHER_OP_H

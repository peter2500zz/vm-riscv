#ifndef DISPATCHER_BRANCH_H
#define DISPATCHER_BRANCH_H

#include "../hart/unprivileged.h"

void dispatch_branch(Hart *hart, Instruction inst);

#endif // DISPATCHER_BRANCH_H

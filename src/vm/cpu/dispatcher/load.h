#ifndef DISPATCHER_LOAD_H
#define DISPATCHER_LOAD_H

#include "../hart/unprivileged.h"

void dispatch_load(Hart *hart, Instruction inst);

#endif // DISPATCHER_LOAD_H

#ifndef DISPATCHER_SYSTEM_H
#define DISPATCHER_SYSTEM_H

#include "../hart/unprivileged.h"

void dispatch_system(Hart *hart, Instruction inst);

#endif // DISPATCHER_SYSTEM_H

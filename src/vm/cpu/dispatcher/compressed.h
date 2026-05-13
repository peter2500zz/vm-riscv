#ifndef COMPRESSED_DISPATCHER_H
#define COMPRESSED_DISPATCHER_H

#include "../hart/unprivileged.h"

void dispatch_compressed(Hart *hart, CInstruction inst);

#endif // COMPRESSED_DISPATCHER_H

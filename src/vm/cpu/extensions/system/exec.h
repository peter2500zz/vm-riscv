#ifndef SYSTEM_H
#define SYSTEM_H

#include "../../hart/unprivileged.h"

void exec_ecall(Hart *hart);
void exec_mret(Hart *hart);

#endif // SYSTEM_H

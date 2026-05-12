#ifndef DISPATCHER_ROOT_H
#define DISPATCHER_ROOT_H

#include "../hart/unprivileged.h"

/**
 * @brief 执行一条指令，并改变pc
 *
 * @param hart 硬件线程指针
 * @param inst 指令
 * @return 是否执行成功
 *
 * @note 执行失败pc不变
 */
int hart_dispatch(Hart *hart, Instruction inst);

#endif // DISPATCHER_ROOT_H

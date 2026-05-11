#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "../vm.h"
#include "../instruction.h"

/**
 * @brief 从虚拟机内存中获取PC指向的指令
 *
 * @param vm 虚拟机实例指针
 * @return 从内存中获取的指令
 */
Instruction vm_fetch(VM *vm);

/**
 * @brief 执行一条指令，并改变pc
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 * @return 是否执行成功
 *
 * @note 执行失败pc不变
 */
int vm_dispatch(VM *vm, Instruction inst);

#endif // DISPATCHER_H

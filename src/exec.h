#ifndef EXEC_H
#define EXEC_H

#include "instruction.h"
#include "vm.h"

/**
 * @brief
 * LUI（加载高位立即数）被用于构建32位常量，它使用U类型格式。
 * LUI把32位U立即数值放在目的寄存器rd中，同时把最低的12位用零填充。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_lui(VM *vm, Instruction inst);

/**
 * @brief
 * AUIPC（加高位立即数到 pc）被用于构建 pc相对地址，它使用U类型格式。
 * AUIPC根据 U 立即数形成32位偏移量（最低12位填零），把这个偏移量加到AUIPC指令的地址，然后把结果放在寄存器rd中。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_auipc(VM *vm, Instruction inst);

/**
 * @brief
 * 跳转和链接（JAL）指令使用 J 类型格式。
 * J类型指令把J立即数以2字节的倍数编码一个有符号的偏移量。
 * 偏移量是符号扩展的，加到当前跳转指令的地址上以形成跳转目标地址。
 * 跳转可以因此到达的目标范围是±1MiB。
 * JAL把跟在JAL之后的指令的地址（pc+4）存储到寄存器rd中。
 * 标准软件调用约定使用 x1作为返回地址寄存器，使用 x5作为备选的链接寄存器。
 * 
 * @param vm 
 * @param inst 
 */
void exec_jal(VM *vm, Instruction inst);

/**
 * @brief
 * 间接跳转指令JALR（跳转和链接寄存器）使用I类型编码。
 * 通过把符号扩展的12位I立即数加到寄存器rs1来获得目标地址，然后把结果的最低有效位设置为零。
 * 紧接着跳转的指令的地址（pc+4）被写入寄存器 rd。
 * 如果不需要结果，寄存器 x0也可以被用作目的寄存器。
 * 
 * @param vm 
 * @param inst 
 */
void exec_jalr(VM *vm, Instruction inst);

void exec_addi(VM *vm, Instruction inst);

#endif // EXEC_H

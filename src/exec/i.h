#ifndef EXEC_H
#define EXEC_H

#include "../instruction.h"
#include "../vm.h"

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
 * AUIPC根据 U
 * 立即数形成32位偏移量（最低12位填零），把这个偏移量加到AUIPC指令的地址，然后把结果放在寄存器rd中。
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
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_jal(VM *vm, Instruction inst);

/**
 * @brief
 * 间接跳转指令JALR（跳转和链接寄存器）使用I类型编码。
 * 通过把符号扩展的12位I立即数加到寄存器rs1来获得目标地址，然后把结果的最低有效位设置为零。
 * 紧接着跳转的指令的地址（pc+4）被写入寄存器 rd。
 * 如果不需要结果，寄存器 x0也可以被用作目的寄存器。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_jalr(VM *vm, Instruction inst);

/**
 * @brief
 * 12位B立即数以2字节的倍数编码符号偏移量。
 * 偏移量是符号扩展的，加到分支指令的地址上以给出目标地址。
 * 条件分支的范围是±4KiB。
 * BEQ在寄存器rs1和rs2相等时采取分支。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_beq(VM *vm, Instruction inst);

/**
 * @brief
 * 12位B立即数以2字节的倍数编码符号偏移量。
 * 偏移量是符号扩展的，加到分支指令的地址上以给出目标地址。
 * 条件分支的范围是±4KiB。
 * BNE在寄存器rs1和rs2不等时采取分支。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_bne(VM *vm, Instruction inst);

/**
 * @brief
 * 12位B立即数以2字节的倍数编码符号偏移量。
 * 偏移量是符号扩展的，加到分支指令的地址上以给出目标地址。
 * 条件分支的范围是±4KiB。
 * BLT在寄存器rs1有符号小于rs2时采取分支。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_blt(VM *vm, Instruction inst);

/**
 * @brief
 * 12位B立即数以2字节的倍数编码符号偏移量。
 * 偏移量是符号扩展的，加到分支指令的地址上以给出目标地址。
 * 条件分支的范围是±4KiB。
 * BLTU在寄存器rs1无符号小于rs2时采取分支。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_bltu(VM *vm, Instruction inst);

/**
 * @brief
 * 12位B立即数以2字节的倍数编码符号偏移量。
 * 偏移量是符号扩展的，加到分支指令的地址上以给出目标地址。
 * 条件分支的范围是±4KiB。
 * BGE在寄存器rs1有符号大于等于rs2时采取分支。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_bge(VM *vm, Instruction inst);

/**
 * @brief
 * 12位B立即数以2字节的倍数编码符号偏移量。
 * 偏移量是符号扩展的，加到分支指令的地址上以给出目标地址。
 * 条件分支的范围是±4KiB。
 * BGEU在寄存器rs1无符号大于等于rs2时采取分支。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_bgeu(VM *vm, Instruction inst);

/**
 * @brief
 * 加载指令被编码为I类型格式。
 * 通过把寄存器rs1加到符号扩展的12位偏移量，可以获得有效地址。
 * LB先从内存加载一个8位的值，然后在存储到rd中之前，把它符号扩展到32位。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_lb(VM *vm, Instruction inst);

/**
 * @brief
 * 加载指令被编码为I类型格式。
 * 通过把寄存器rs1加到符号扩展的12位偏移量，可以获得有效地址。
 * LH先从内存加载一个16位的值，然后在存储到rd中之前，把它符号扩展到32位。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_lh(VM *vm, Instruction inst);

/**
 * @brief
 * 加载指令被编码为I类型格式。
 * 通过把寄存器rs1加到符号扩展的12位偏移量，可以获得有效地址。
 * LW指令从内存加载一个32位的值到rd。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_lw(VM *vm, Instruction inst);

/**
 * @brief
 * 加载指令被编码为I类型格式。
 * 通过把寄存器rs1加到符号扩展的12位偏移量，可以获得有效地址。
 * LBU先从内存加载一个8位的值，然后，在存储到rd中之前，把它用零扩展到32位。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_lbu(VM *vm, Instruction inst);

/**
 * @brief
 * 加载指令被编码为I类型格式。
 * 通过把寄存器rs1加到符号扩展的12位偏移量，可以获得有效地址。
 * LHU先从内存加载一个16位的值，然后，在存储到rd中之前，把它用零扩展到32位。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_lhu(VM *vm, Instruction inst);

/**
 * @brief
 * 存储指令被编码为S类型格式。
 * 通过把寄存器rs1加到符号扩展的12位偏移量，可以获得有效地址。
 * SB指令从寄存器rs2的低位将8位的值存储到内存。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_sb(VM *vm, Instruction inst);

/**
 * @brief
 * 存储指令被编码为S类型格式。
 * 通过把寄存器rs1加到符号扩展的12位偏移量，可以获得有效地址。
 * SH指令从寄存器rs2的低位将16位的值存储到内存。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_sh(VM *vm, Instruction inst);

/**
 * @brief
 * 存储指令被编码为S类型格式。
 * 通过把寄存器rs1加到符号扩展的12位偏移量，可以获得有效地址。
 * SW指令从寄存器rs2的低位将32位的值存储到内存。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_sw(VM *vm, Instruction inst);

/**
 * @brief
 * ADDI 将符号扩展的12位立即数加到寄存器rs1上。
 * 简单地将结果的低XLEN位当作结果，而忽略了算数溢出。
 * ADDI rd, rs1, 0 被用于实现 MV rd, rs1 汇编器伪指令。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_addi(VM *vm, Instruction inst);

/**
 * @brief
 * 如果寄存器rs1
 * 小于符号扩展的立即数（当二者都被视为有符号数时），
 * SLTI指令把值1放到寄存器rd中；否则，该指令把0写入rd中。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_slti(VM *vm, Instruction inst);

/**
 * @brief
 * 如果寄存器rs1
 * 小于符号扩展的立即数（当二者都被视为无符号数时），
 * SLTIU指令把值1放到寄存器rd中；否则，该指令把0写入rd中。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_sltiu(VM *vm, Instruction inst);

/**
 * @brief ANDI
 * 在寄存器rs1和符号扩展的12位立即数上执行按位AND并把结果放入rd的逻辑操作。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_andi(VM *vm, Instruction inst);

/**
 * @brief ORI
 * 在寄存器rs1和符号扩展的12位立即数上执行按位OR并把结果放入rd的逻辑操作。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_ori(VM *vm, Instruction inst);

/**
 * @brief XORI
 * 在寄存器rs1和符号扩展的12位立即数上执行按位XOR并把结果放入rd的逻辑操作。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_xori(VM *vm, Instruction inst);

/**
 * @brief SLLI
 * 是逻辑左移指令，被移位的操作数存放在rs1中，移位的数目被编码在I立即数域的低5位。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_slli(VM *vm, Instruction inst);

/**
 * @brief SRLI
 * 是逻辑右移指令，被移位的操作数存放在rs1中，移位的数目被编码在I立即数域的低5位。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_srli(VM *vm, Instruction inst);

/**
 * @brief SRAI
 * 是算术右移指令，被移位的操作数存放在rs1中，移位的数目被编码在I立即数域的低5位。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_srai(VM *vm, Instruction inst);

/**
 * @brief
 * ADD执行rs1和rs2的相加。
 * 忽略结果的溢出，并把结果的低XLEN位写入目的寄存器rd。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_add(VM *vm, Instruction inst);

/**
 * @brief
 * SUB执行从rs1中减去rs2。
 * 忽略结果的溢出，并把结果的低XLEN位写入目的寄存器rd。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_sub(VM *vm, Instruction inst);

/**
 * @brief
 * SLL对寄存器rs1中的值执行逻辑左移，移位的数目保持在寄存器rs2的低5位中。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_sll(VM *vm, Instruction inst);

/**
 * @brief
 * SLT执行有符号的比较，如果rs1<rs2，向rd写入1，否则写入0。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_slt(VM *vm, Instruction inst);

/**
 * @brief
 * SLTU执行无符号的比较，如果rs1<rs2，向rd写入1，否则写入0。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_sltu(VM *vm, Instruction inst);

/**
 * @brief
 * XOR对寄存器rs1和rs2中的值执行按位异或，并把结果写入rd。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_xor(VM *vm, Instruction inst);

/**
 * @brief
 * SRL对寄存器rs1中的值执行逻辑右移，移位的数目保持在寄存器rs2的低5位中。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_srl(VM *vm, Instruction inst);

/**
 * @brief
 * SRA对寄存器rs1中的值执行算术右移，移位的数目保持在寄存器rs2的低5位中。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_sra(VM *vm, Instruction inst);

/**
 * @brief
 * OR对寄存器rs1和rs2中的值执行按位或，并把结果写入rd。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_or(VM *vm, Instruction inst);

/**
 * @brief
 * AND对寄存器rs1和rs2中的值执行按位与，并把结果写入rd。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_and(VM *vm, Instruction inst);

#endif // EXEC_H

#ifndef ZICSR_EXEC_H
#define ZICSR_EXEC_H

#include "../../vm/spec/vm.h"

/**
 * @brief
 * CSRRW （原子性读/写 CSR）指令自动地交换 CSR 和整数寄存器中的值。
 * CSRRW读取 CSR 的旧值，把该值零扩展到 XLEN 位，然后把它写到整数寄存器 rd。
 * rs1中的初始值被写到 CSR。如 果
 * rd=x0，那么指令不应当读CSR，也不应当引起任何可能在读 CSR 时发生的副作用。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_csrrw(VM *vm, Instruction inst);

/**
 * @brief
 * CSRRS（原子性读和设置 CSR 位）指令读取 CSR 的值，把该值零扩展到 XLEN
 * 位，然后把它写到整数寄存器 rd。 整数寄存器 rs1
 * 中的初始值被视为位掩码，它指定要在 CSR 中设置的位的位置。 任何在 rs1
 * 中为高的位将引起 CSR 中对应的位（如果它可写的话）被设置。 CSR
 * 中的其它位不会被显式地写入。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_csrrs(VM *vm, Instruction inst);

/**
 * @brief
 * CSRRC（原子性读和清除 CSR 位）指令读取 CSR 的值，把该值零扩展到 XLEN
 * 位，然后把它写到整数寄存器 rd。 整数寄存器 rs1
 * 中的初始值被视为位掩码，它指定了要在 CSR 中被清除的位的位置。 任何在 rs1
 * 中为高的位将引起 CSR 中对应的位（如果它是可写的话）被清除。 CSR
 * 中的其它位不会被显式地写入。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_csrrc(VM *vm, Instruction inst);

/**
 * @brief
 * CSRRWI 指令与 CSRRW 类似，但使用零扩展的 5 位立即数（uimm[4:0]）替代 rs1
 * 寄存器作为写入 CSR 的源值。 读取 CSR 的旧值，把该值零扩展到 XLEN
 * 位，然后写到整数寄存器 rd。 如果 rd=x0，则不读取 CSR，也不引起任何读 CSR
 * 时可能发生的副作用。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_csrrwi(VM *vm, Instruction inst);

/**
 * @brief
 * CSRRSI 指令与 CSRRS 类似，但使用零扩展的 5 位立即数（uimm[4:0]）替代 rs1
 * 寄存器作为位掩码。 读取 CSR 的值，把该值零扩展到 XLEN 位，然后写到整数寄存器
 * rd。 立即数中为高的位将引起 CSR
 * 中对应的位（如果可写）被设置，其它位不会被显式写入。 若立即数为零，则不写入
 * CSR，也不引起任何写 CSR 时可能发生的副作用。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_csrrsi(VM *vm, Instruction inst);

/**
 * @brief
 * CSRRCI 指令与 CSRRC 类似，但使用零扩展的 5 位立即数（uimm[4:0]）替代 rs1
 * 寄存器作为位掩码。 读取 CSR 的值，把该值零扩展到 XLEN 位，然后写到整数寄存器
 * rd。 立即数中为高的位将引起 CSR
 * 中对应的位（如果可写）被清除，其它位不会被显式写入。 若立即数为零，则不写入
 * CSR，也不引起任何写 CSR 时可能发生的副作用。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_csrrci(VM *vm, Instruction inst);

#endif // ZICSR_EXEC_H
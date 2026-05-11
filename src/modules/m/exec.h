#ifndef M_EXEC_H
#define M_EXEC_H

#include "../../vm.h"
#include "../../instruction.h"

/**
 * @brief
 * MUL 在 rs1 和 rs2 上执行 XLEN 位 ×XLEN 位乘法，并将低 XLEN 位放入目的寄存器。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_mul(VM *vm, Instruction inst);

/**
 * @brief
 * MULH 针对有符号数 × 有符号数执行乘法，返回 2×XLEN 位结果的高 XLEN 位。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_mulh(VM *vm, Instruction inst);

/**
 * @brief
 * MULHSU 针对有符号数 × 无符号数执行乘法，返回 2×XLEN 位结果的高 XLEN 位。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_mulhsu(VM *vm, Instruction inst);

/**
 * @brief
 * MULHU 针对无符号数 × 无符号数执行乘法，返回 2×XLEN 位结果的高 XLEN 位。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_mulhu(VM *vm, Instruction inst);

/**
 * @brief
 * DIV 在 rs1 和 rs2 上执行 XLEN 位与 XLEN 位的有符号整数除法，结果向零取整。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_div(VM *vm, Instruction inst);

/**
 * @brief
 * DIVU 在 rs1 和 rs2 上执行 XLEN 位与 XLEN 位的无符号整数除法，结果向零取整。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_divu(VM *vm, Instruction inst);

/**
 * @brief
 * REM 在 rs1 和 rs2 上执行 XLEN 位与 XLEN 位的有符号整数取余运算，结果与 rs1
 * 的符号相同。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_rem(VM *vm, Instruction inst);

/**
 * @brief
 * REMU 在 rs1 和 rs2 上执行 XLEN 位与 XLEN 位的无符号整数取余运算，结果与 rs1
 * 的符号相同。
 *
 * @param vm 虚拟机实例指针
 * @param inst 指令
 */
void exec_remu(VM *vm, Instruction inst);

#endif // M_EXEC_H

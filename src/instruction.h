#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdint.h>

#define BITS(x, hi, lo) (((x) >> (lo)) & ((1u << ((hi) - (lo) + 1)) - 1))
#define BIT(x, n) (((x) >> (n)) & 1u)

typedef uint32_t Instruction;


/**
 * @brief 获取指令的操作数
 *
 * @param inst 指令
 * @return 操作数
 */
uint32_t inst_opcode(Instruction inst);

/**
 * @brief 获取指令的目的寄存器编号
 *
 * @param inst 指令
 * @return 目的寄存器编号
 */
uint32_t inst_rd(Instruction inst);

/**
 * @brief 获取指令的funct3字段
 *
 * @param inst 指令
 * @return funct3字段值
 */
uint32_t inst_funct3(Instruction inst);

/**
 * @brief 获取指令的源寄存器1编号
 *
 * @param inst 指令
 * @return 源寄存器1编号
 */
uint32_t inst_rs1(Instruction inst);

/**
 * @brief 获取指令的源寄存器2编号
 *
 * @param inst 指令
 * @return 源寄存器2编号
 */
uint32_t inst_rs2(Instruction inst);

/**
 * @brief 获取指令的funct7字段
 *
 * @param inst 指令
 * @return funct7字段值
 */
uint32_t inst_funct7(Instruction inst);

/**
 * @brief 获取I型指令的立即数
 *
 * @param inst 指令
 * @return 12位有符号立即数
 */
int32_t inst_imm_i(Instruction inst);

/**
 * @brief 获取S型指令的立即数
 *
 * @param inst 指令
 * @return 12位有符号立即数
 */
int32_t inst_imm_s(Instruction inst);

/**
 * @brief 获取B型指令的立即数
 *
 * @param inst 指令
 * @return 13位有符号立即数（最低位恒为0）
 */
int32_t inst_imm_b(Instruction inst);

/**
 * @brief 获取U型指令的立即数
 *
 * @param inst 指令
 * @return 32位有符号立即数
 */
int32_t inst_imm_u(Instruction inst);

/**
 * @brief 获取J型指令的立即数
 *
 * @param inst 指令
 * @return 21位有符号立即数（最低位恒为0）
 */
int32_t inst_imm_j(Instruction inst);

#endif // INSTRUCTION_H

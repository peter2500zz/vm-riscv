#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdint.h>

static inline int32_t sign_extend(uint32_t value, int bits) {
        int shift = 32 - bits;
        return (int32_t)(value << shift) >> shift;
}

#define BITS(x, hi, lo) (((x) >> (lo)) & ((1u << ((hi) - (lo) + 1)) - 1))
#define BIT(x, n) (((x) >> (n)) & 1u)

typedef uint32_t Instruction;

/**
 * @brief 获取指令的操作数
 *
 * @param inst 指令
 * @return 操作数
 */
static inline uint32_t inst_opcode(Instruction inst) {
        return BITS(inst, 6, 0);
}

/**
 * @brief 获取指令的目的寄存器编号
 *
 * @param inst 指令
 * @return 目的寄存器编号
 */
static inline uint32_t inst_rd(Instruction inst) { return BITS(inst, 11, 7); }

/**
 * @brief 获取指令的funct3字段
 *
 * @param inst 指令
 * @return funct3字段值
 */
static inline uint32_t inst_funct3(Instruction inst) {
        return BITS(inst, 14, 12);
}

/**
 * @brief 获取指令的源寄存器1编号
 *
 * @param inst 指令
 * @return 源寄存器1编号
 */
static inline uint32_t inst_rs1(Instruction inst) { return BITS(inst, 19, 15); }

/**
 * @brief 获取指令的源寄存器2编号
 *
 * @param inst 指令
 * @return 源寄存器2编号
 */
static inline uint32_t inst_rs2(Instruction inst) { return BITS(inst, 24, 20); }

/**
 * @brief 获取指令的funct7字段
 *
 * @param inst 指令
 * @return funct7字段值
 */
static inline uint32_t inst_funct7(Instruction inst) {
        return BITS(inst, 31, 25);
}

/**
 * @brief 获取I型指令的立即数
 *
 * @param inst 指令
 * @return 12位有符号立即数
 */
static inline int32_t inst_imm_i(Instruction inst) {
        return sign_extend(BITS(inst, 31, 20), 12);
}

/**
 * @brief 获取S型指令的立即数
 *
 * @param inst 指令
 * @return 12位有符号立即数
 */
static inline int32_t inst_imm_s(Instruction inst) {
        return sign_extend(BITS(inst, 31, 25) << 5 | BITS(inst, 11, 7), 12);
}

/**
 * @brief 获取B型指令的立即数
 *
 * @param inst 指令
 * @return 13位有符号立即数（最低位恒为0）
 */
static inline int32_t inst_imm_b(Instruction inst) {
        return sign_extend(BIT(inst, 31) << 12 | BIT(inst, 7) << 11 |
                               BITS(inst, 30, 25) << 5 | BITS(inst, 11, 8) << 1,
                           13);
}

/**
 * @brief 获取U型指令的立即数
 *
 * @param inst 指令
 * @return 32位有符号立即数
 */
static inline int32_t inst_imm_u(Instruction inst) {
        return (int32_t)(BITS(inst, 31, 12) << 12);
}

/**
 * @brief 获取J型指令的立即数
 *
 * @param inst 指令
 * @return 21位有符号立即数（最低位恒为0）
 */
static inline int32_t inst_imm_j(Instruction inst) {
        return sign_extend(BIT(inst, 31) << 20 | BITS(inst, 19, 12) << 12 |
                               BIT(inst, 20) << 11 | BITS(inst, 30, 21) << 1,
                           21);
}

#endif // INSTRUCTION_H

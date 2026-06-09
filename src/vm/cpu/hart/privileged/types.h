#ifndef PRIVILEGED_INST_H
#define PRIVILEGED_INST_H

#include <stdint.h>

#include "../unprivileged/types.h"

/**
 * @brief 特权级别
 *
 */
typedef enum {
        PRIV_U = 0,
        PRIV_S = 1,
        PRIV_M = 3,
} PrivMode;

typedef union {
        uint32_t addr;
} CSR;

static inline CSR inst_csr(Instruction inst) {
        return (CSR){.addr = (uint32_t)BITS(inst, 31, 20)};
}

#define CAUSE_MISALIGNED_FETCH 0x00000000    // 取指地址未对齐
#define CAUSE_FETCH_ACCESS 0x00000001        // 取指访问故障（PMP/总线拒绝）
#define CAUSE_ILLEGAL_INSTRUCTION 0x00000002 // 非法/未定义指令
#define CAUSE_BREAKPOINT 0x00000003          // ebreak 断点
#define CAUSE_MISALIGNED_LOAD 0x00000004     // load 地址未对齐
#define CAUSE_LOAD_ACCESS 0x00000005         // load 访问故障
#define CAUSE_MISALIGNED_STORE 0x00000006    // store 地址未对齐
#define CAUSE_STORE_ACCESS 0x00000007        // store 访问故障
#define CAUSE_ECALL_U 0x00000008             // U-mode 执行 ecall
#define CAUSE_ECALL_S 0x00000009             // S-mode 执行 ecall
#define CAUSE_ECALL_M 0x0000000B             // M-mode 执行 ecall
#define CAUSE_FETCH_PAGE_FAULT 0x0000000C    // 取指缺页（虚拟内存未映射）
#define CAUSE_LOAD_PAGE_FAULT 0x0000000D     // load 缺页
#define CAUSE_STORE_PAGE_FAULT 0x0000000F    // store 缺页

#define CAUSE_MSI 0x80000003 // Machine Software Interrupt  核间 IPI
#define CAUSE_MTI 0x80000007 // Machine Timer Interrupt     CLINT 定时器
#define CAUSE_MEI 0x8000000B // Machine External Interrupt  PLIC 外设中断

typedef union {
        uint32_t raw; // 直接读写整个寄存器

        struct {
                uint32_t UIE : 1;  // bit  0  U-mode 中断使能（N扩展，通常为0）
                uint32_t SIE : 1;  // bit  1  S-mode 全局中断使能
                uint32_t _r0 : 1;  // bit  2  保留
                uint32_t MIE : 1;  // bit  3  M-mode 全局中断使能
                uint32_t UPIE : 1; // bit  4  trap 前 UIE 的备份
                uint32_t SPIE : 1; // bit  5  trap 前 SIE 的备份
                uint32_t _r1 : 1;  // bit  6  保留
                uint32_t MPIE : 1; // bit  7  trap 前 MIE 的备份
                uint32_t SPP : 1;  // bit  8  trap 前 S-mode 特权级备份
                uint32_t _r2 : 2;  // bit  10:9  保留
                uint32_t MPP : 2;  // bit  12:11 trap 前 M-mode 特权级备份
                uint32_t FS : 2;   // bit  14:13 浮点单元状态
                uint32_t XS : 2;   // bit  16:15 自定义扩展状态
                uint32_t MPRV : 1; // bit  17 内存访问使用 MPP 特权级
                uint32_t SUM : 1;  // bit  18 S-mode 可访问 U-mode 页
                uint32_t MXR : 1;  // bit  19 可执行页允许读
                uint32_t TVM : 1;  // bit  20 trap 虚拟内存（拦截 sfence.vma）
                uint32_t TW : 1;   // bit  21 trap WFI（防止低权级卡死）
                uint32_t TSR : 1;  // bit  22 trap SRET
                uint32_t _r3 : 8;  // bit  30:23 保留
                uint32_t SD : 1;   // bit  31 FS/XS 为 Dirty 的汇总标志
        };
} mstatus_t;

#define CSR_MSTATUS 0x300 // 机器状态寄存器。
#define CSR_MIE 0x304     // 机器中断使能。
#define CSR_MTVEC 0x305   // 机器陷阱处理程序基地址。
#define CSR_MEPC 0x341    // 机器异常程序计数器。
#define CSR_MCAUSE 0x342  // 机器陷阱原因。
#define CSR_MTVAL 0x343   // 机器错误地址或指令。
#define CSR_MIP 0x344     // 机器中断挂起。

#define MIP_MSIP (1u << 3) // 软件中断位（mie/mip 中均为 bit 3）
#define MIP_MTIP (1u << 7) // 定时器中断位（mie/mip 中均为 bit 7）
#define MIP_MEIP (1u << 11) // 外部中断位（mie/mip 中均为 bit 11）

#endif // PRIVILEGED_INST_H

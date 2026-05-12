cmake_minimum_required(VERSION 3.20)
set(CMAKE_C_COMPILER clang)
project(vm-riscv C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(vm
        # 主函数
        src/main.c
        # 虚拟机
        src/vm/machine.c
        # CPU
        src/vm/cpu/hart/unprivileged.c
        src/vm/cpu/hart/privileged.c
        # 内存
        src/vm/memory/access.c
        # 调度器
        src/vm/cpu/dispatcher/root.c
        # 扩展
        src/vm/cpu/extensions/i/exec.c
        src/vm/cpu/extensions/m/exec.c
        src/vm/cpu/extensions/ecall/exec.c
        src/vm/cpu/extensions/zicsr/exec.c
)

target_compile_options(vm PRIVATE
        -Wall
        -Wextra
        -Wconversion
        -Wsign-conversion
        -Wdouble-promotion
        -Wformat=2
        -Wnull-dereference
        -Wshadow
        -Wundef
        -pedantic
)

enable_testing()
add_subdirectory(tests)

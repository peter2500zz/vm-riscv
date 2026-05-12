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
        src/vm/hart//unprivileged.c
        src/vm/hart//privileged.c
        # 调度器
        src/dispatcher/root.c
        # 扩展
        src/extensions/i/exec.c
        src/extensions/m/exec.c
        src/extensions/ecall/exec.c
        src/extensions/zicsr/exec.c
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

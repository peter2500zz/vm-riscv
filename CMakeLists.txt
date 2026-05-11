cmake_minimum_required(VERSION 3.20)
set(CMAKE_C_COMPILER clang)
project(vm-riscv C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(vm
        src/main.c
        src/vm.c
        src/instruction.c
        src/exec/i.c
        src/ecall.c
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

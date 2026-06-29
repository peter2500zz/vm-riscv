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
        # 设备
        src/vm/memory/mmio.c
        src/vm/devices/clint/handler.c
        # 调度器
        src/vm/cpu/dispatcher/root.c
        src/vm/cpu/dispatcher/branch.c
        src/vm/cpu/dispatcher/load.c
        src/vm/cpu/dispatcher/store.c
        src/vm/cpu/dispatcher/op.c
        src/vm/cpu/dispatcher/opimm.c
        src/vm/cpu/dispatcher/system.c
        src/vm/cpu/dispatcher/compressed.c
        # 扩展
        src/vm/cpu/extensions/i/exec.c
        src/vm/cpu/extensions/m/exec.c
        src/vm/cpu/extensions/system/exec.c
        src/vm/cpu/extensions/zicsr/exec.c
        src/vm/cpu/extensions/c/exec.c
        src/utils/mmioMap.c
        src/utils/mmioMap.h
        src/vm/memory/memory.c
        src/vm/memory/memory.h
        src/vm/memory/ram.c
        src/vm/memory/ram.h
        src/vm/memory/memory.c
        src/vm/memory/memory.h
        src/vm/devices/uart/init.c
        src/vm/devices/uart/init.h
        src/vm/devices/uart/console.c
        src/vm/devices/uart/console.h
        src/vm/devices/clint/init.c
        src/vm/devices/clint/init.h
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

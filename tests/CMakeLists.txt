set(Hart_SOURCES
        # 虚拟机
        ../src/vm/machine.c
        # CPU
        ../src/vm/cpu/hart/unprivileged.c
        ../src/vm/cpu/hart/privileged.c
        # 内存
        ../src/vm/memory/access.c
        # 调度器
        ../src/vm/cpu/dispatcher/root.c
        # 扩展
        ../src/vm/cpu/extensions/i/exec.c
        ../src/vm/cpu/extensions/m/exec.c
        ../src/vm/cpu/extensions/ecall/exec.c
        ../src/vm/cpu/extensions/zicsr/exec.c
)

file(GLOB TEST_SOURCES "test_*.c")

foreach(test_source ${TEST_SOURCES})
    get_filename_component(test_name ${test_source} NAME_WE)
    add_executable(${test_name} ${test_source} ${Hart_SOURCES})
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
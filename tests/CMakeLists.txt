set(Hart_SOURCES
        # 虚拟机
        ../src/vm/machine.c
        ../src/vm/hart/unprivileged.c
        ../src/vm/hart/privileged.c
        # 调度器
        ../src/dispatcher/root.c
        # 扩展
        ../src/extensions/i/exec.c
        ../src/extensions/m/exec.c
        ../src/extensions/ecall/exec.c
        ../src/extensions/zicsr/exec.c
)

file(GLOB TEST_SOURCES "test_*.c")

foreach(test_source ${TEST_SOURCES})
    get_filename_component(test_name ${test_source} NAME_WE)
    add_executable(${test_name} ${test_source} ${Hart_SOURCES})
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
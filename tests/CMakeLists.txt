set(VM_SOURCES
        ../src/vm/spec/vm.c
        ../src/vm/privileged/vm.c
        ../src/dispatcher/root.c
        ../src/modules/i/exec.c
        ../src/modules/m/exec.c
        ../src/modules/ecall/exec.c
        ../src/modules/zicsr/exec.c
)

file(GLOB TEST_SOURCES "test_*.c")

foreach(test_source ${TEST_SOURCES})
    get_filename_component(test_name ${test_source} NAME_WE)
    add_executable(${test_name} ${test_source} ${VM_SOURCES})
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
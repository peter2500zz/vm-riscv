set(VM_SOURCES
    ../src/exec/i.c
    ../src/exec/m.c
    ../src/vm.c
    ../src/instruction.c
    ../src/ecall.c
)

file(GLOB TEST_SOURCES "test_*.c")

foreach(test_source ${TEST_SOURCES})
    get_filename_component(test_name ${test_source} NAME_WE)
    add_executable(${test_name} ${test_source} ${VM_SOURCES})
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
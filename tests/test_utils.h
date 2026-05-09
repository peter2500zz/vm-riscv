#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdio.h>

#include "../src/vm.h"

#define ASSERT_EQ(actual, expected)                                            \
        if ((actual) != (expected)) {                                          \
                printf("FAIL: %s:%d — got %d, expected %d\n", __FILE__,        \
                       __LINE__, actual, expected);                            \
                return 1;                                                      \
        } else {                                                               \
                printf("PASS: %s:%d\n", __FILE__, __LINE__);                   \
        }

#define SETUP_VM(mem_size)                                                     \
        VM *vm = vm_new(mem_size);                                             \
        if (vm == NULL) {                                                      \
                printf("FAIL: vm_new returned NULL\n");                        \
                return 1;                                                      \
        }

#endif // TEST_UTILS_H

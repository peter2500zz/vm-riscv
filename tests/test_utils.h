#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <inttypes.h>
#include <stdio.h>

#define ASSERT_EQ(actual, expected)                                            \
        if ((actual) != (expected)) {                                          \
                printf("FAIL: %s:%d — got %" PRIu32 ", expected %" PRIu32      \
                       "\n",                                                   \
                       __FILE__, __LINE__, (uint32_t)(actual),                 \
                       (uint32_t)(expected));                                  \
                return 1;                                                      \
        } else {                                                               \
                printf("PASS: %s:%d\n", __FILE__, __LINE__);                   \
        }

#define FILL_Hart(...)                                                         \
        do {                                                                   \
                uint32_t _buf[] = {__VA_ARGS__};                               \
                if (hart_load(hart, 0x0, (uint8_t *)_buf, sizeof(_buf)) !=     \
                    0) {                                                       \
                        printf("FAIL: hart_load failed\n");                    \
                        return 1;                                              \
                }                                                              \
        } while (0)

#define EXECUTE(instruction)                                                   \
        do {                                                                   \
                if (hart_dispatch(hart, instruction) != 0) {                   \
                        printf("FAIL: hart_dispatch failed\n");                \
                        return 1;                                              \
                }                                                              \
        } while (0)

#endif // TEST_UTILS_H

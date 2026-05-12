#include "handler.h"
#include <stdint.h>
#include <stdio.h>

uint8_t lsr = 0;
uint8_t rbr = 0;

uint32_t dindex = 0;
uint8_t *data = NULL;

void handle_uart(Hart *hart, uint32_t addr, void *target, uint32_t size,
                 MemAccessType type) {
        if (data == NULL) {
                printf("Init fake data\n");

                FILE *fp = fopen("bin", "rb");
                if (fp == NULL) {
                        exit(1);
                }

                fseek(fp, 0, SEEK_END);
                long file_size = ftell(fp);
                if (file_size < 0 || file_size > (long)INT32_MAX) {
                        exit(1);
                }
                fseek(fp, 0, SEEK_SET);

                uint8_t *buffer = malloc((uint32_t)file_size);
                if (buffer == NULL) {
                        exit(1);
                }

                // 将文件大小写入data，然后追加二进制数据
                *(uint32_t *)buffer = (uint32_t)file_size;
                size_t bytes_read =
                    fread(buffer + sizeof(uint32_t), 1, (size_t)file_size, fp);
                if ((uint32_t)file_size != (uint32_t)bytes_read) {
                        free(buffer);
                        exit(1);
                }
                data = buffer;
                printf("Success\n");
        }

        switch (addr - UART_ADDR) {
        case 0x05:                                  // LSR
                *((uint8_t *)target) = 0x01; // DR位为1，表示有数据可读
                break;
        case 0x00:                                  // RBR
                *((uint8_t *)target) = data[dindex];
                dindex++;
                break;
        }
}

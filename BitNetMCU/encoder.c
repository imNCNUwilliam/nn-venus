#include <stdio.h>
#include <stdint.h>
#include "BitNetMCU_MNIST_test_data.h"
#include "BitNetMCU_model.h"
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int encode(char *filename, uint8_t *data, unsigned int row, unsigned int col, unsigned char move) {
    int num_elements = row * col * move;
    unsigned char stride = (move == 4 ? 1 : 4);
    char *symbol;
    long long sz;
    int fp = open(filename, O_RDWR | O_CREAT, 0644);
    unsigned char val;
    uint8_t *src;
    uint8_t *dst;

    if (fp < 0) {
        printf("Failed to open the file %s", filename);
        exit(-1);
    }

    symbol = (char *)malloc((num_elements * stride + 8) * sizeof(char));
    memset(symbol, '\0', sizeof(symbol));
//    printf("%x %x %x 0\n", row & 0x000000F0, row & 0x0000000F, row >> 8);
    memcpy(symbol, &row, 4);
    memcpy(symbol + 4, &col, 4);

    src = data;
    dst = symbol + 8;
    while (num_elements) {
        val = *src;
//        printf("%x\n", val);
        memcpy(dst, &val, 1);
        src++;
        dst += stride;
	num_elements--;
    }

    sz = write(fp, symbol, (row * col + 2) * 4);
    if (sz != (row * col + 2) * 4) {
        printf("Failed to write the file %s", filename);
        exit(-1);
    }

    free(symbol);
    close(fp);
    return sz;
}

int main(void) {
    encode("input", (uint8_t *)input_data_0, 16 * 16, 1, 1);
    encode("L1_weights", (uint8_t *)L1_weights, 256 * 64 * L1_bitperweight / 32, 1, 4);
    encode("L2_weights", (uint8_t *)L2_weights, 64 * 64 * L2_bitperweight / 32, 1, 4);
    encode("L3_weights", (uint8_t *)L3_weights, 64 * 64 * L3_bitperweight / 32, 1, 4);
    encode("L4_weights", (uint8_t *)L4_weights, 64 * 10 * L4_bitperweight / 32, 1, 4);

    return 0;
}


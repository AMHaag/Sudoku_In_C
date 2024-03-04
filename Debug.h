#include <stdint.h>
#include <stdio.h>

void printBinary(uint16_t num) {
    for (int i = 15; i >= 0; i--) {
        if ((i + 1) % 4 == 0) {
            printf(" ");
        }
        uint16_t mask = 1 << i;
        if (num & mask) {
            printf("1");
        }
        else {
            printf("0");
        }
    }
    printf("\n");
}
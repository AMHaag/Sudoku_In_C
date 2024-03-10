#include "Utilities.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
void PrintBin(uint16_t num) {
  for (int i = 15; i >= 0; i--) {
    if ((i + 1) % 4 == 0) {
      printf(" ");
    }
    uint16_t mask = 0;
    mask = 1 << i;
    if (num & mask) {
      printf("1");
    } else {
      printf("0");
    }
  }
  printf("\n");
}

char *PrintBinString(uint16_t num) {
  char *output = (char *)malloc(20 * sizeof(char));
  int output_index = 0;
  uint16_t mask = 0;
  mask = 1;
  if ((num & mask) == 0) {
    mask = 0;
    int x = 0;
    int n = 9;
    int cands[9] = {0};
    for (int i = 6; i < 15; i++) {
      mask = 32768 >> i;
      if (num & mask) {
        cands[x] = n;
        n--;
      } else {
        n--;
      }
      x++;
    }
    output[0] = '[';
    output_index++;
    for (int i = 0; i < 9; i++) {
      if (cands[i]) {
        output[output_index] = cands[i] + '0';
        output_index++;
        if (cands[i] != 1) {
          output[output_index] = ',';
          output_index++;
        }
      }
    }
    output[output_index] = ']';
  } else {
    mask = 0;
    int m = 9;
    for (int i = 6; i < 15; i++) {
      mask = 32768 >> i;
      if (num & mask) {
        output[output_index] = m + '0';
      } else {
        m--;
      }
    }
  }
  return output;
}

char *CreateFullOutputString(uint16_t *board) {
  char *output = (char *)malloc(1600 * sizeof(char));
  memset(output, '\0', 1600);
  int output_index = 0;
  for (int i = 1; i < 82; i++) {
    char *c = PrintBinString(board[i]);
    // printf("%d",board[i]);
    for (int j = 0; j < 19; j++) {
      if (c[j] == '\0') {
        break;
      }
      output[output_index] = c[j];
      output_index++;
    }
    output[output_index] = ',';
    output_index++;
  }
  // printf("%s", output);
  return output;
}

void FullState(char *input, uint16_t *board) {
  printf("FullStringOut: \n");
  printf("%s", CreateFullOutputString(board));
  // for (int i = 1; i < 82; i++) {
  //   printf("Input: %c     ", input[i - 1]);
  //   PrintBinString(board[i]);
  //   PrintBin(board[i]);
  // }
}
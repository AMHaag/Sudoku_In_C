#include "Utilities.h"
#include "Containers.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
char *PrintCellBin(uint16_t num) {
  char *binaryString = (char *)calloc(20, sizeof(char));
  int bsIndex = 0;
  for (int i = 15; i >= 0; i--) {
    if ((i + 1) % 4 == 0) {
      binaryString[bsIndex] = '_';
      bsIndex++;
    }
    uint16_t mask = 0;
    mask = 1 << i;
    if (num & mask) {
      binaryString[bsIndex] = '1';
      bsIndex++;
    } else {
      binaryString[bsIndex] = '0';
      bsIndex++;
    }
  }
  return binaryString;
}

char *PrintCellString(uint16_t num) {
  char *output = (char *)calloc(20, sizeof(char));
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
          output[output_index] = '.';
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
    char *c = PrintCellString(board[i]);
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
  output_index--;
  output[output_index] = '\0';
  return output;
}
int CellToInt(uint16_t cell) {
  int n = cell-1;
  switch (n) {
  case 2:
    return 1;
  case 4:
    return 2;
  case 8:
    return 3;
  case 16:
    return 4;
  case 32:
    return 5;
  case 64:
    return 6;
  case 128:
    return 7;
  case 256:
    return 8;
  case 512:
    return 9;
  default:
    return 0;
  }
}
void FullState(char *input, uint16_t *board) {
  printf("FullStringOut: \n");
  printf("%s", CreateFullOutputString(board));
  // for (int i = 1; i < 82; i++) {
  //   printf("Input: %c     ", input[i - 1]);
  //   PrintCellString(board[i]);
  //   PrintCellBin(board[i]);
  // }
}

void PrintPosition(int n) {
  Position pzzz;
  pzzz = GetPosition(n);
  printf(
      "Input: %d Row(x): %d Col(y): %d Box(z): %d Zx: %d Zy: %d Zindex: %d\n",
      n, pzzz.x, pzzz.y, pzzz.z, pzzz.zx, pzzz.zy, pzzz.zindex);
}

void changeLastChar(char *str, char newChar) {
  int length = strlen(str);
  if (length > 0) {
    str[length - 1] = newChar;
  }
}
void PrintSimpleMates(HandOff *H) {
  char rowMates[25] = {'\0'};
  char colMates[25] = {'\0'};
  char boxMates[25] = {'\0'};
  for (int i = 0; i < 8; i++) {
    char r[10], c[10], b[10];
    sprintf(r, "%d", CellToInt(H->rmX[i]));
    sprintf(c, "%d", CellToInt(H->cmX[i]));
    sprintf(b, "%d", CellToInt(H->bxX[i]));
    strcat(rowMates, r);
    strcat(colMates, c);
    strcat(boxMates, b);
    strcat(rowMates, ",");
    strcat(colMates, ",");
    strcat(boxMates, ",");
  }
  changeLastChar(rowMates, '\0');
  changeLastChar(colMates, '\0');
  changeLastChar(boxMates, '\0');
  printf("RowMates: %s\n", rowMates);
  printf("ColMates: %s\n", colMates);
  printf("BoxMates: %s\n", boxMates);
}

void PrintMates(HandOff *H) {
  char rowMates[1600] = {'\0'};
  char colMates[1600] = {'\0'};
  char boxMates[1600] = {'\0'};
  for (int i = 0; i < 8; i++) {
    char *r = PrintCellString(H->rmX[i]);
    char *c = PrintCellString(H->cmX[i]);
    char *b = PrintCellString(H->bxX[i]);
    strcat(r, ",");
    strcat(c, ",");
    strcat(b, ",");
    strcat(rowMates, r);
    strcat(colMates, c);
    strcat(boxMates, b);
  }
  printf("RowMates: %s\n", rowMates);
  printf("ColMates: %s\n", colMates);
  printf("BoxMates: %s\n", boxMates);
}

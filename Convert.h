#include "Containers.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
Cell char_to_bitmask(char c) {
  if (c == '0') {
    uint16_t unknown = 0b0000001111111110;
    return unknown; // 0000001111111110
  }
  uint16_t c_int = c - '0';

  uint16_t mask = 0b0000000000000000;
  mask |= (1 << 0);
  mask |= (1 << c_int);
  return mask;
}
char *PrintCellBin(Cell num, bool display) {
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
  if(display){
    printf("%s\n", binaryString);
  }
  return binaryString;
}
char *PrintCellString(Cell num, bool display) {
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
  if(display){
    printf("%s\n", output);
  }
  return output;
}
char *CreateFullOutputString(Cell *board, bool display) {
  char *output = (char *)malloc(1600 * sizeof(char));
  memset(output, '\0', 1600);
  int output_index = 0;
  for (int i = 1; i < 82; i++) {
    char *c = PrintCellString(board[i],false);
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
  if(display){
    printf("%s\n", output);
  }
  return output;
}
int CellToInt(Cell cell) {
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
  printf("%s", CreateFullOutputString(board,false));
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
    char *r = PrintCellString(H->rmX[i],false);
    char *c = PrintCellString(H->cmX[i],false);
    char *b = PrintCellString(H->bxX[i],false);
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
int convert_input(char *input, Board board) {
  int k = 0;
  for (int i = 1; i < 82; i++) {
    if (input[i - 1] == '0') {
      k++;
    }
    board[i] = char_to_bitmask(input[i - 1]);
  }
  return k;
}
char *read_file(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Failed to open the file.\n");
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  rewind(file);

  char *buffer = (char *)malloc(file_size + 1);
  if (buffer == NULL) {
    printf("Failed to allocate memory.\n");
    fclose(file);
    return NULL;
  }

  size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
  if (bytes_read != file_size) {
    printf("Failed to read the file.\n");
    free(buffer);
    fclose(file);
    return NULL;
  }

  buffer[file_size] = '\0'; // Add null terminator

  fclose(file);
  return buffer;
}
void UnitForEach(Unit *unit, void (*f)(uint16_t *)) {
  for (int i = 0; i < 9; i++) {
    f(&(*unit)[i]);
  }
}
void BoardForEach(Board *board, void (*f)(uint16_t *)) {
  for (int i = 1; i < 82; i++) {
    f(&(*board)[i]);
  }
}
void PrintInt(uint16_t *n) { printf("%d", *n); }
void PrintnaXToString(HandOff *H) {
  printf("naX: ");
  char c[1600] = {'\0'};
  for (int i = 0; i < 9; i++) {
    char *r = PrintCellString(H->naX[i],false);
    strcat(c, r);
    strcat(c, ",");
  }
  printf("%s", c);
}
void PrintnbXToString(HandOff *H) {
  printf("nbX: ");
  char c[1600] = {'\0'};
  for (int i = 0; i < 9; i++) {
    char *r = PrintCellString(H->nbX[i],false);
    strcat(c, r);
    strcat(c, ",");
  }
  printf("%s", c);
}
void PrintncXToString(HandOff *H) {
  printf("ncX: ");
  char c[1600] = {'\0'};
  for (int i = 0; i < 9; i++) {
    char *r = PrintCellString(H->ncX[i],false);
    strcat(c, r);
    strcat(c, ",");
  }
  printf("%s", c);
  free(c);
}
void PrintnaXToInt(HandOff *H) {
  printf("naX: ");
  for (int i = 0; i < 9; i++) {
    printf("%d,", CellToInt(H->naX[i]));
  }
  printf("\n");
}
void PrintnbXToInt(HandOff *H) {
  printf("nbX: ");
  for (int i = 0; i < 9; i++) {
    printf("%d,", CellToInt(H->nbX[i]));
  }
  printf("\n");
}
void PrintncXToInt(HandOff *H) {
  printf("ncX: ");
  for (int i = 0; i < 9; i++) {
    printf("%d,", H->ncX[i]);
  }
  printf("\n");
}
void BR(){
  printf("\n");
}










//end
#include "Conversion.h"
#include "Solvers.h"
#include <stdint.h>
#include <stdio.h>

// Global Variables
// char input[81] =
// "301086504046521070500000001400800002080347900009050038004090200008734090007208103";
// char input[81] =
// "6795182435437296188216349577943521863584617292168975344852763"
//                  "91962183475137945862";
char input[81] = {0};
uint16_t rowMates[8] = {0};
uint16_t colMates[8] = {0};
uint16_t boxMates[8] = {0};
uint16_t nineArry[9] = {0};
int convert_input(char *input, uint16_t *board) {
  int k = 0;

  for (int i = 1; i < 82; i++) {
    if (input[i - 1] == '0') {
      k++;
    }
    board[i] = char_to_bitmask(input[i - 1]);
    if (CellSolveTest(board[i])) {
    }
  }
  // printf("k:%d", k);
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

int main(int argc, char *argv[]) {
  // for(int i=1;i<82;i++){
  //   PrintPosition(i);
  // };
  // return ;
  if (argc > 1) {
    for (int i = 0; i < 81; i++) {
      input[i] = argv[1][i];
    }
  }
#pragma region Specific Puzzle
  // Specific Puzzle Version
  //  uint16_t board[82] = {0};
  //  int unknowns = convert_input(input, board);
  //  printf("There are %d unknowns in this puzzle.\n", unknowns);
  //  assert(unknowns < 81);
  //  Globals G = {0};
  //  HandOff H = {0};
  //  G.bxG = boxMates;
  //  G.rmG = rowMates;
  //  G.cmG = colMates;
  //  int x = Solve(board, &G, unknowns);
  //  FullState(input, board);
  // return 0;
#pragma endregion

#pragma region Batch Loop
  long canSolve = 0;
  long notSolve = 0;
  char *batch = read_file("./Sudoku_Data_Set/test_batch_1.txt");
  // printf("%zd",strlen(batch));
  for (long i = 0; i < 500000; i++) {
    for (long j = 0; j < 81; j++) {
      if (batch[i * 81 + j+i] == '\n') {
        batch[i * 81 + j+i] = '*';
      }
      input[j] = batch[i * 81 + j+i];
    }
    // i++;
    // printf("=====Puzzle %d=====\n", i + 1);
    // printf("%s\n", input);

  uint16_t board[82] = {0};
  int unknowns = convert_input(input, board);
  // printf("There are %d unknowns in this puzzle.\n", unknowns);
  assert(unknowns < 81);
  Globals G = {0};
  HandOff H = {0};
  G.bxG = boxMates;
  G.rmG = rowMates;
  G.cmG = colMates;
  int x = Solve(board, &G, unknowns);
  // FullState(input, board);
  if (x) {
    // printf("Solved in: %d\n",x);
    canSolve++;
  }else{
    // printf("Not Solved: %d\n",x);
    notSolve++;
  }
}
printf("Can Solve: %ld\n",canSolve);
printf("Not Solve: %ld\n",notSolve);
double percent = (double)canSolve/(double)(canSolve+notSolve);
printf("Solved: %.2f %% ",percent*100.0);
return 0;
#pragma endregion
}

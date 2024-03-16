#include "Solvers.h"
#include <stdint.h>
#include <stdio.h>

// Global Variables
//clang-format off
// char input[81] =
// "7020893001000000000042000000460900009070065200030000090713209"
//                  "00620004007400010200";
char input[81] = "8001349020410960800050700100086050004063100090230408605007090"
                 "00010080040000401006";
//clang-format on
//  char input[81] = {0};
uint16_t rowMates[8] = {0};
uint16_t colMates[8] = {0};
uint16_t boxMates[8] = {0};
Unit nineArryA;
Unit nineArryB;

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
  if (false) {
    Board board = {0};
    printf("%s\n",input);
    int unknowns = convert_input(input, board);
    printf("There are %d unknowns in this puzzle.\n", unknowns);
    assert(unknowns < 81);
    Globals G = {0};
    HandOff H = {0};
    G.bxG = boxMates;
    G.rmG = rowMates;
    G.cmG = colMates;
    G.naG = &nineArryA;
    G.nbG = &nineArryB;
    int x = Solve(board, &G, unknowns);
    if (x > 0) {
      printf("\nSolved in: %d\n", x);
      // printf("%s", CreateFullOutputString(board));
    } else {
      printf("\nNot Solved: %d\n", x*-1);
    }
    return 0;
  }
#pragma endregion
// end Specific Puzzle
#pragma region Batch Loop
  long canSolve = 0;
  long notSolve = 0;
  int most = 0;
  int least = 1000;
  char *batch = read_file("./Sudoku_Data_Set/test_batch_1.txt");
  // printf("%zd",strlen(batch));
  for (long i = 0; i < 1000000; i++) {
    for (long j = 0; j < 81; j++) {
      if (batch[i * 81 + j + i] == '\n') {
        batch[i * 81 + j + i] = '*';
      }
      input[j] = batch[i * 81 + j + i];
    }
    // printf("\n=====Puzzle %d=====\n", i + 1);
    // printf(input);
    Board board[82] = {0};
    int unknowns = convert_input(input, board);
    // printf("There are %d unknowns in this puzzle.\n", unknowns);
    assert(unknowns < 81);
    Globals G = {0};
    HandOff H = {0};
    G.bxG = boxMates;
    G.rmG = rowMates;
    G.cmG = colMates;
    G.naG = &nineArryA;
    G.nbG = &nineArryB;
    int x = Solve(board, &G, unknowns);
    // FullState(input, board);
    if (x > 0) {
      most < x ? most = x : most;
      least > x ? least = x : least;
    }
    if (x>0) {
      // printf("Solved in: %d\n",x);
      canSolve++;
    }
     if(x<0) {
      //  printf("\n\n\n\n");
      //  printf(input);
      //  printf("\n\n\nFailed to solve Puzzle# %d in %d Iterations\n", i + 1,x*-1);
       // printf("Not Solved: %d\n",x);
       notSolve++;
    }
    if(x==0){
      printf("Failed to Solve Puzzle# %d Within Iteration Limit\n", i + 1);
    }
  }
  printf("Most: %d\n", most);
  printf("Least: %d\n", least);
  printf("Can Solve: %ld\n", canSolve);
  printf("Not Solve: %ld\n", notSolve);
  double percent = (double)canSolve / (double)(canSolve + notSolve);
  printf("Solved: %.2f %% ", percent * 100.0);
  return 777;

#pragma endregion
  // end
}

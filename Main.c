#include <stdint.h>
#include <stdio.h>

#include "Conversion.h"
#include "Solvers.h"

// Global Variables
char input[81] = "8613594723791248005246789134129673007532816499865431276487925"
                 "31235816794197435268";
// char input[81] =
// "6795182435437296188216349577943521863584617292168975344852763"
//                  "91962183475137945862";
uint16_t rowMates[8] = {0};
uint16_t colMates[8] = {0};
uint16_t boxMates[8] = {0};
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
  printf("k:%d", k);
  return k;
}
int main(int argc, char *argv[]) {
  // for(int i=1;i<82;i++){
  //   PrintPosition(i);
  // };  
  // return ;
  if(argc > 1){
    for(int i=0;i<81;i++){
      input[i] = argv[1][i];
    }
  }
  uint16_t board[82] = {0};
  int unknowns = convert_input(input, board);
  printf("There are %d unknowns in this puzzle.\n", unknowns);
  assert(unknowns <81);
  Globals G = {0};
  HandOff H = {0};
  G.bxG = boxMates;
  G.rmG = rowMates;
  G.cmG = colMates;
  int x = Solve(board, &G, unknowns);
  FullState(input, board);
  if(x){
    printf("Solved\n");
  }
  return 0;
}

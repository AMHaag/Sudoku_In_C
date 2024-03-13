#include <stdint.h>
#include <stdio.h>

#include "Conversion.h"
#include "Solvers.h"

// Global Variables
char input[81] = "0700000430400096108006349000940520003584600200008005300800700"
                 "91902100005007040802";
// char input[81] =
// "6795182435437296188216349577943521863584617292168975344852763"
//                  "91962183475137945862";
uint16_t rowMates[8] = {0};
uint16_t colMates[8] = {0};
uint16_t boxMates[8] = {0};
int convert_input(char *input, uint16_t *board) {
  int unknowns = 0;
  for (int i = 1; i < 82; i++) {
    board[i] = char_to_bitmask(input[i - 1]);
    if (CellSolveTest(board[i])) {
      unknowns++;
    }
  }
  return unknowns;
}

void test(int n) {}
// Main
int main() {
  uint16_t board[82] = {0};
  int unknowns = 0;
  unknowns = convert_input(input, board);
  Globals G = {0};
  HandOff H = {0};
  G.bxG = boxMates;
  G.rmG = rowMates;
  G.cmG = colMates;
  Solve(board, &G, unknowns);
  FullState(input, board);

  return 0;
}

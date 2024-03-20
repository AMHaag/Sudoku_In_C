
#include "Solvers.h"
#include <stdint.h>
#include <stdio.h>
char input[81] = "0040080500000500920000000000002750810500002000023045000000960"
                 "20241000360060420810";
uint16_t rowMates[8] = {0};
uint16_t colMates[8] = {0};
uint16_t boxMates[8] = {0};
Unit nineArryA;
Unit nineArryB;
int nineArryC;

int main() {
  Board board = {0};
  printf("%s\n", input);
  int unknowns = convert_input(input, board);
  assert(unknowns < 81);
  Globals G = {0};
  HandOff H = {0};
  G.bxG = boxMates;
  G.rmG = rowMates;
  G.cmG = colMates;
  G.naG = nineArryA;
  G.nbG = nineArryB;
  G.ncG = &nineArryC;
  HandOff data;
  int solved = 0;
  int elimed = 0;
  data.bX = board;
  data.rmX = G.rmG;
  data.cmX = G.cmG;
  data.bxX = G.bxG;
  data.naX = G.naG;
  data.nbX = G.nbG;
  data.ncX = G.ncG;
  data.unsolvedX = 0;
  FindBasicCandidates(&data);
  IterateGroups(&data);
  return 0;
}

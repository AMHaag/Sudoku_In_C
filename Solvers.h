#include "Convert.h"
#include <assert.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

bool CellSolveTest(uint16_t n) {
  switch (n) {
  case 2:
    return true;
  case 4:
    return true;
  case 8:
    return true;
  case 16:
    return true;
  case 32:
    return true;
  case 64:
    return true;
  case 128:
    return true;
  case 256:
    return true;
  case 512:
    return true;
  default:
    return false;
  }
}
bool IsCellSolved(int n) { return (n & 1) == 1; }
void FillMates(int n, HandOff(*H)) {
  if (IsCellSolved(H->bX[n])) {
    return;
  }
  for (int i = 0; i < 8; i++) {
    H->rmX[i] = 0;
    H->cmX[i] = 0;
    H->bxX[i] = 0;
  }
  Position Position;
  Position = GetPosition(n);
  //* Row Mates
  int xskip = 0;
  int rowStart = ((Position.x - 1) * 9) + 1;
  for (int j = 0; j < 8; j++) {
    if (j == Position.y - 1) {
      xskip++;
    }
    H->rmX[j] = H->bX[rowStart + j + xskip];
  }
  //* Col Mates
  int yskip = 0;
  int colStart = Position.y;
  for (int i = 1; i < 10; i++) {
    if (i == Position.x) {
      yskip++;
      continue;
    }
    int colIdx = colStart + ((i - 1 + yskip) * 9);

    H->cmX[i - 1] = H->bX[colIdx];
  }
  //* Box Mates
  int z = GetZStart(Position.z);
  int jmp = 0;
  bool zskip = 0; // 6-1=5
  for (int i = 0; i < 9; i++) {
    if (i % 3 == 0 && i != 0) {
      jmp += 6;
    }
    int n = z + i + jmp;
    if (i == Position.zindex - 1) {
      zskip++;
      continue;
    }
    H->bxX[i - zskip] = H->bX[n];
  }
}
int FindBasicCandidates(HandOff(*H)) {
  int finds = 0;
  for (int i = 1; i < 82; i++) {
    assert(H->bX[i] > 0);
    assert(H->bX[i] < 1024);
    int cell = CellToInt(H->bX[i]);
    if (H->bX[i] & 1) {
      continue;
    }
    if (CellSolveTest(H->bX[i]) == true && H->bX[i] > 0) {
      H->bX[i] |= 1;
      H->unsolvedX--;
      finds++;
      continue;
    }
    Position p = GetPosition(i);
    FillMates(i, H);
    // printf("-------------------------------------------------------------\n");
    // PrintPosition(i);
    // PrintMates(H);
    uint16_t elimed = 0;
    for (int j = 0; j < 8; j++) {
      // printf("Index: %d\n", j + 1);
      uint16_t r = H->rmX[j] & 1 ? H->rmX[j] - 1 : 0;
      uint16_t c = H->cmX[j] & 1 ? H->cmX[j] - 1 : 0;
      uint16_t b = H->bxX[j] & 1 ? H->bxX[j] - 1 : 0;
      // printf("=: %s\n", PrintCellBin(H->bX[i]));
      // printf("R: %s\n", PrintCellBin(r));
      // printf("C: %s\n", PrintCellBin(c));
      // printf("Z: %s\n", PrintCellBin(c));
      uint16_t newElims = r | c | b;
      // printf("n: %s\n\n", PrintCellBin(newElims));
      elimed = elimed | r | c | b;
      // printf("E: %s\n\n", PrintCellBin(elimed));
    }
    // PrintSimpleMates(H);

    char *_elimed = PrintCellBin(elimed);
    // printf("Cell#%d\n", i);
    // printf("Elimed: %s\n", _elimed);

    if (elimed) {
      // printf("Before: %s\n", PrintCellBin(H->bX[i]));
      // printf("&:      %s\n", PrintCellBin(H->bX[i] & ~elimed));
      H->bX[i] &= ~elimed;
      // printf("After:  %s\n", PrintCellBin(H->bX[i]));
      if (CellToInt(H->bX[i]) == cell) {
        finds++;
      }
    }
    if (CellSolveTest(H->bX[i])&& H->bX[i] > 0) {
      H->bX[i] |= 1;

      H->unsolvedX--;
      finds++;
      continue;
    }
  }
  return finds;
}

void SolveCells(HandOff H) { 3 - 4; }

void FindAdvCandidates(HandOff H) {}

int GetZStart(int z) {
  int n = z - 1;
  int x = n % 3;
  int y = n / 3;
  return (x * 3) + (y * 27) + 1;
}

int Solve(uint16_t(*board), Globals *globals, int unknown) {
  // Variables
  HandOff data;
  int solved = 0;
  int elimed = 0;
  data.bX = board;
  data.rmX = globals->rmG;
  data.cmX = globals->cmG;
  data.bxX = globals->bxG;
  data.unsolvedX = unknown;
  int f = 0;
  //* Do/While
  // printf("Iterataions: 0\n");
  // for (int i = 1; i < 82; i++) {
  //   printf("Cell: %d %s - %s\n", i, PrintCellBin(data.bX[i]),
  //          PrintCellString(data.bX[i]));
  // }
  do {
    f = 0;
    // Part A: Check for Solved or Mistakes

    // Part B: Solve Cells
    f = FindBasicCandidates(&data);
    // Part C: Eliminate candidates
    // Part D: increment iteration count
    data.bX[0]++;
    // printf("==================================================================="
    //        "=======\n");
    // printf("Iterataion: %d\n", data.bX[0]);
    // printf("Findings:%d\n", f);
    for (int i = 1; i < 82; i++) {
      //   printf("Cell: %d %s - %s\n", i, PrintCellBin(data.bX[i]),
      //          PrintCellString(data.bX[i]));
    }
    // printf("%s\n\n\n", CreateFullOutputString(data.bX));
    // printf("Unsolved Remainging: %d\n", data.unsolvedX);
    if (data.unsolvedX == 0) {
      data.solved = true;
      return data.bX[0];
    }
    if (data.bX[0] > 50) {
      return 0;
    }

  } while (f);
  //   // Part E: Recurse / MultiThread-Guess
  // FullState(input, board);
  return 0;
}

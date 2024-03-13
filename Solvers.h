#include "Debug.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief This struct is used to hold the state of the board. It is pertinent
 * only to the function that invokes it.
 */
typedef struct {
  uint16_t(*bX);  // The board
  uint16_t(*rmX); // The Reusable RowMates Array
  uint16_t(*cmX); // The Reusable ColumnMates Array
  uint16_t(*bxX); // The Reusable BoxMates Array
  int *unsolvedX; // The number of unknown cells
  int *solvX;     // The number of cells solved
  int *elimX;     // The number of candidates eliminated
  bool *solved;   // The state of the board
} HandOff;
/**
 * @brief  This struct represents the position of a cell in the overall board.
 */
typedef struct {
  int x;      /** The Row the cell is in  (1-based)*/
  int y;      /** The Column the cell is in (1-based)*/
  int z;      /** The Box the cell is in (1-based)*/
  int zx;     /** The 'Row' within the box the cell is in */
  int zy;     /** The 'Column' within the box the cell is in */
  int zindex; /** The index number of the cell within the Box*/
} Position;
typedef struct {
  uint16_t(*rmG);
  uint16_t(*cmG);
  uint16_t(*bxG);
} Globals;
/// @brief Returns the Row, Column, Box, and box coordinates of a the input
/// cell.
/// @param i The Index you are ascertaining the position of.
/// @return struct Position
/// @see Position
Position GetPosition(int n) {
  Position out;
  out.y = (n - 1) % 9 + 1;
  out.x = (n - 1) / 9 + 1;
  out.z = 3 * ((out.x - 1) / 3) + ((out.y - 1) / 3) + 1;
  out.zx = (out.x - 1) % 3;
  out.zy = (out.y - 1) % 3;
  out.zindex = 3 * out.zx + out.zy + 1;
  out.zx++;
  out.zy++;
  return out;
}
void PrintPosition(int n) {
  Position pzzz;
  pzzz = GetPosition(n);
  printf("Input: %d Row: %d Col: %d Box: %d Zx: %d Zy: %d Zindex: %d\n\n", n,
         pzzz.x, pzzz.y, pzzz.z, pzzz.zx, pzzz.zy, pzzz.zindex);
}
bool CellSolveTest(int n) {
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
  for (int i = 0; i < 8; i++) {
    if (IsCellSolved(H->bX[i])) {
      continue;
    }
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
    // printf("r %d\n", rowStart + j + xskip);
    H->rmX[j] = H->bX[rowStart + j + xskip];
  }
  int yskip = 0;
  int colStart = Position.y - 1;
  for (int i = 0; i < 9; i++) {
    if (i == Position.x - 1) {
      yskip++;
      continue;
    }
    // printf("c %d\n", colStart + (i * 9) + yskip);
    H->cmX[i] = H->bX[colStart + (i * 9) + yskip];
  }
  int ost = GetZStart(Position.z);
  for (int i = 0; i < 9; i++) {
    int zskip = 0;
    if (i == Position.zindex - 1) {
      zskip++;
      continue;
    }
    // printf("z %d\n", ost + i + zskip);
    H->bxX[i] = H->bX[ost + i + zskip];
    if ((i + 1) % 3 == 0) {
      ost += 6;
    }
  }
}
char *PrintMates(HandOff *H) {
  char rowMates[1600] = {'\0'};
  char colMates[1600] = {'\0'};
  char boxMates[1600] = {'\0'};
  for (int i = 0; i < 8; i++) {
    char *r = PrintCellString(H->rmX[i]);
    char *c = PrintCellString(H->cmX[i]);
    char *b = PrintCellString(H->bxX[i]);
    strcat(rowMates, r);
    strcat(colMates, c);
    strcat(boxMates, b);
  }
  printf("RowMates: %s\n", rowMates);
  printf("ColMates: %s\n", colMates);
  printf("BoxMates: %s\n", boxMates);
  return "fix this shit";
}

int FindBasicCandidates(HandOff(*H)) {
  printf("ding");
  int finds = 0;
  for (int i = 1; i < 82; i++) {
    if (!H->bX[i] > 0) {
      printf("Cell#%d is somehow 0", i);
    };
    assert(H->bX[i] > 0);
    if (H->bX[i] & 1) {
      continue;
    }
    if (CellSolveTest(H->bX[i]) == true && H->bX[i] > 0) {
      H->bX[i] |= 1;
      finds++;
      continue;
    }
    Position p = GetPosition(i);
    FillMates(i, H);
    // PrintPosition(i);
    // PrintMates(H);
    uint16_t elimed = 0;
    for (int j = 0; j < 8; j++) {
      uint16_t r = H->rmX[j] & 1 ? H->rmX[j] - 1 : 0;
      uint16_t c = H->cmX[j] & 1 ? H->cmX[j] - 1 : 0;
      uint16_t b = H->bxX[j] & 1 ? H->bxX[j] - 1 : 0;
      // printf("R: %s\n", PrintCellBin(r));
      // printf("C: %s\n", PrintCellBin(c));
      // printf("Z: %s\n", PrintCellBin(c));

      elimed = elimed | r | c | b;
      // printf("E: %s\n\n", PrintCellBin(elimed));
    }
    char *_elimed = PrintCellBin(elimed);
    // printf("Cell#%d\n", i);
    // printf("Elimed: %s\n", _elimed);

    if (elimed) {
      // printf("Before: %s\n", PrintCellBin(H->bX[i]));
      H->bX[i] &= ~elimed;

      finds++;
    }
    // printf("After:  %s\n", PrintCellBin(H->bX[i]));
  }
  // printf("done");
  return finds;
}

bool CheckStatus(uint16_t cell) {
  if (cell & 1) {
    return true;
  } else {
    return false;
  }
}

void SolveCells(HandOff H) {}

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
  data.unsolvedX = &unknown;
  data.elimX = &elimed;
  data.solvX = &solved;
  int f = 0;
  //* Do/While
  // printf("Iterataions: 0\n");
  // for (int i = 1; i < 82; i++) {
  //   printf("Cell: %d %s - %s\n", i, PrintCellBin(data.bX[i]),
  //          PrintCellString(data.bX[i]));
  // }
  do {
    data.solvX = 0;
    data.elimX = 0;
    f = 0;
    // Part A: Check for Solved or Mistakes

    // Part B: Solve Cells
    f = FindBasicCandidates(&data);
    // Part C: Eliminate candidates
    // Part D: increment iteration count
    data.bX[0]++;
    // printf("Findings:%d", f);
    printf("Iterataions: %d\n", data.bX[0]);
    for (int i = 1; i < 82; i++) {
      printf("Cell: %d %s - %s\n", i, PrintCellBin(data.bX[i]),
             PrintCellString(data.bX[i]));
    }
    printf("\n\n\n");
    printf("%s\n\n\n", CreateFullOutputString(data.bX));
    if (data.bX[0] > 50) {
      break;
    }
  } while (f);
  //   // Part E: Recurse / MultiThread-Guess
  // FullState(input, board);
  return 0;
}

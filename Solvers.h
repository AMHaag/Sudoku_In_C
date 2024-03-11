#include "Debug.h"
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

void FillMates(int n, HandOff(*H)) {
  for (int i = 0; i < 8; i++) {
    H->rmX[i] = 0;
    H->cmX[i] = 0;
    H->bxX[i] = 0;
  }
  Position Position;
  Position = GetPosition(n);
  for (int i = 0; i < 8; i++) {
    int skip = 0;
    int rowStart = ((Position.x - 1) * 9) + 1;
    for (int j = 0; j < 8; j++) {
      if (j == Position.y - 1) {
        skip++;
      }
      H->rmX[j] = H->bX[rowStart + j + skip];
    }
    skip = 0;
    int colStart = Position.y - 1;
    for (int j = 0; j < 8; j++) {
      if (j == Position.x - 1) {
        skip++;
      }
      H->cmX[j] = H->bX[colStart + (j * 9) + skip];
    }
    skip = 0;
  }

  int ost = GetZStart(Position.z);
  for (int i = 0; i < 8; i++) {
    int skip = 0;
    if (i == Position.zindex - 1) {
      skip++;
      continue;
    }
    H->bxX[i] = H->bX[ost + i + skip];
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

void FindBasicCandidates(HandOff *H) {
  for (int i = 1; i < 82; i++) {
    Position p = GetPosition(i);
    FillMates(i, H);
    PrintPosition(i);
    PrintMates(H);
    return;
    uint16_t elimed = 0;
    for (int j = 0; j < 8; j++) {
      uint16_t r = H->rmX[j] & 1 ? H->rmX[j] : 0;
      uint16_t c = H->cmX[j] & 1 ? H->cmX[j] : 0;
      uint16_t b = H->bxX[j] & 1 ? H->bxX[j] : 0;
      elimed = r | c | b;
      char *_elimed = PrintCellBin(elimed);
      printf("Elimed: %s\n", _elimed);
      elimed--;
    }

    if (elimed) {
      //   H->bX[i] &= ~elimed;
      //   printf("Eliminated %d from cell %d\n", elimed, i);
      //   (*H->elimX)++;
    }
  }
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

// int Solve(uint16_t(*puzzle)) {

//   // Variables
//   HandOff myHandOff;
//   uint16_t board[82];
//   memcpy(board, puzzle, sizeof(uint16_t) * 82);
//   int solveCount = 0;
//   int elimCount = 0;
//   myHandOff.bX = (&board[82]);

//   myHandOff.solvX = &solveCount;
//   myHandOff.elimX = &elimCount;
//   myHandOff.solved = false;

//   //* Do/While
//   do {
//     solveCount = 0;
//     elimCount = 0;
//     // Part A: Check for Solved or Mistakes

//     // Part B: Solve Cells
//     SolveCells(myHandOff);
//     // Part C: Eliminate candidates
//     FindAdvCandidates(myHandOff);
//   } while (solveCount + elimCount != 0);
//   // Part D: Find Best Cell for first Guess
//   // Part E: Recurse / MultiThread-Guess
// }

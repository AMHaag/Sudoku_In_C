#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
  uint16_t *bX;
  uint16_t *rmX;
  uint16_t *cmX;
  uint16_t *bxX;
  int *sX;
  int *eX;
  bool *solved;
} HandOff;

void FindBasicCandidates(uint16_t *board) {
  for (int i = 1; i < 82; i++) {
    if (board[i] & 1) {
      continue;
    }
    Position p = GetPosition(board[i]);
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

/**
 * @struct Position
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

/// @brief Returns the Row, Column, Box, and box coordinates of a the input cell.
/// @param i The Index you are ascertaining the position of.
/// @return struct Position
/// @see Position
Position GetPosition(int n) {
  Position out;
  out.y = (n - 1) % 9 + 1;
  out.x = (n - 1) / 9 + 1;
  out.z = 3 * ((out.x - 1) / 3) + ((out.y - 1) / 3) + 1;
  out.zx = (out.x - 1) % 3;
  out.zy = (out.x - 1) % 3;
  out.zindex = 3 * out.zx + out.zy + 1;
  out.zx++;
  out.zy++;
  return out;
}

void FillMates(int n, HandOff H) {
  Position Position;
  Position = GetPosition(n);
  for (int i = 0; i < 8; i++) {
    int skip = 0;
    int rowStart = ((Position.x - 1) * 9) + 1;
    for (int j = 0; j < 9; i++) {
      if (j == Position.y) {
        skip++;
      }
      H.rmX[j] = H.bX[rowStart + j + skip];
    }
    skip = 0;
    int colStart = Position.y;
    for (int j = 0; j < 9; i++) {
      if (j == Position.x) {
        skip++;
      }
      H.cmX[j] = H.bX[colStart + (j * 9) + skip];
    }
    skip = 0;
  }
}

int Solve(uint16_t *puzzle) {

  // Variables
  HandOff myHandOff;
  uint16_t board[82] = puzzle;
  int solveCount = 0;
  int elimCount = 0;
  myHandOff.bX = &board;

  myHandOff.sX = &solveCount;
  myHandOff.eX = &elimCount;
  myHandOff.solved = false;

  //* Do/While
  do {
    solveCount = 0;
    elimCount = 0;
    // Part A: Check for Solved or Mistakes

    // Part B: Solve Cells
    SolveCells(myHandOff);
    // Part C: Eliminate candidates
    FindAdvCandidates(myHandOff);
  } while (solveCount + elimCount != 0);
  // Part D: Find Best Cell for first Guess
  // Part E: Recurse / MultiThread-Guess
}

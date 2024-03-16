#include "Convert.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool CellIsSolvable(Cell n) {
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
  return false;
}
bool IsCellSolved(Cell n) { return (n & 1) == 1; }
int GetZStart(Box z) {
  int n = z - 1;
  int x = n % 3;
  int y = n / 3;
  return (x * 3) + (y * 27) + 1;
}
void GetRow(Row n, HandOff(*H)) {
  for (int i = 0; i < 9; i++) {
    H->naX[i] = H->bX[n + i];
  }
}
void GetCol(Col n, Board board, Unit dest) {
  for (int i = 0; i < 9; i++) {
    dest[i] = board[n + (i * 9)];
  }
}
void GetBox(Box n, Board board, Unit dest) {
  int z = GetZStart(n);
  int jmp = 0;
  for (int i = 0; i < 9; i++) {
    if (i % 3 == 0 && i != 0) {
      jmp += 6;
    }
    dest[i] = board[z + i + jmp];
  }
}
void FindDuplicates(HandOff(*H)) {
  for (int i = 0; i < 9; i++) {
    if (IsCellSolved(H->naX[i])) {
      continue;
    }
    uint16_t duplicates = 0;
    for (int j = 0; j < 9; j++) {
      if (i != j && H->naX[i] == H->naX[j]) {
        duplicates++;
      }
      H->nbX[i] = duplicates;
    }
    if (!duplicates) {
      continue;
    }
    H->nbX[i] = duplicates;
  }
}
int CountCandidates(Cell cell) {
  if (IsCellSolved(cell)) {
    return 0;
  }
  int count = 0;
  for (int i = 1; i < 10; i++) {
    if (cell & (1 << i)) {
      count++;
    }
  }
  return count;
}
bool FindUsableDuplicates(HandOff(*H)) {
  bool result = false;
  FindDuplicates(H);
  for (int i = 0; i < 9; i++) {
    if (!H->nbX[i]) {
      continue;
    }
    int count = CountCandidates(H->naX[i]);
    if (CountCandidates(H->naX[i]) == H->nbX[i] - 1) {
      // printf("Index %d has %d usable duplicate(s)\n Count: %d Candidates: %s "
      //        "\n Row: ",
      //        i, H->nbX[i], count, PrintCellString(H->naX[i]));
      // for (int j = 0; j < 9; j++) {
      //   printf("%s,", PrintCellString(H->naX[j]));
      // }
      // printf("\n");
      result = true;
    } else {
      H->nbX[i] = 0;
    }
  }
  return result;
}
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
    if (CellIsSolvable(H->bX[i]) == true && H->bX[i] > 0) {
      H->bX[i] |= 1;
      // printf("Index# %d solved, Answer is %d == PRE-FIND\n", i,
      //        CellToInt(H->bX[i]));
      H->unsolvedX--;
      finds++;
      continue;
    }
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
      // uint16_t newElims = r | c | b;
      // printf("n: %s\n\n", PrintCellBin(newElims));
      elimed = elimed | r | c | b;
      // printf("E: %s\n\n", PrintCellBin(elimed));
    }
    // PrintSimpleMates(H);

    // printf("Elimed: %s\n", _elimed);
    bool findAdded = false;
    if (elimed) {
      // printf("Before: %s\n", PrintCellBin(H->bX[i]));
      // printf("&:      %s\n", PrintCellBin(H->bX[i] & ~elimed));
      H->bX[i] &= ~elimed;
      // printf("After:  %s\n", PrintCellBin(H->bX[i]));
      if (CellToInt(H->bX[i]) != cell) {
        findAdded = true;
        finds++;
      }
    }
    if (CellIsSolvable(H->bX[i]) == true && H->bX[i] > 0) {
      H->bX[i] |= 1;
      H->unsolvedX--;
      // printf("Index# %d solved, Answer is %d == POSTFIND %d Unknowns Remain\n",
      //        i, CellToInt(H->bX[i]), H->unsolvedX);
      if (!findAdded) {
        finds++;
      }
      continue;
    }
  }
  return finds;
}
int FindNakedPartners(HandOff(*H)) {
  memset(H->naX, 0, 9 * sizeof(uint16_t));
  for (int i = 1; i < 10; i++) {
    // TODO Set naX to row, col or box
    GetRow(i, H);
    // bool a = FindUsableDuplicates(&H->naX, &H->nbX);
    bool a = FindUsableDuplicates(H);

    // if (a) {
    //   // printf("Row %d has usable duplicates\n", i);
    // }

    // TODO Check if naX has and 1/2 duplicate groups
    // TODO Use Bwise Ops to remove the Partnered Candidates from the other
    // cells
  }
  return 0;
}
void FindAdvCandidates(HandOff H) {}

int Solve(uint16_t(*board), Globals *globals, int unknown) {
  // Variables
  HandOff data;
  int solved = 0;
  int elimed = 0;
  data.bX = board;
  data.rmX = globals->rmG;
  data.cmX = globals->cmG;
  data.bxX = globals->bxG;
  data.naX = globals->naG;
  data.nbX = globals->nbG;
  data.unsolvedX = unknown;
  // printf("Unknowns: %d\n", data.unsolvedX);
  int finds = 0;
  //* Do/While
  // printf("Iterataions: 0\n");
  // for (int i = 1; i < 82; i++) {
  //   printf("Cell: %d %s - %s\n", i, PrintCellBin(data.bX[i]),
  //          PrintCellString(data.bX[i]));
  // }
  do {
    // printf("%d\n", finds);
    // Part A: Check for Solved or Mistakes

    // Part B: Solve Cells
    finds = FindBasicCandidates(&data);
    // FindNakedPartners(&data);
    // Part C: Eliminate candidates
    // Part D: increment iteration count
    data.bX[0]++;
    // printf("==================================================================="
    //        "=======\n");
    // printf("Iterataion: %d\n", data.bX[0]);
    // printf("Findings:%d\n", f);
    // for (int i = 1; i < 82; i++) {
    //   printf("Cell: %d %s - %s\n", i, PrintCellBin(data.bX[i]),
    //          PrintCellString(data.bX[i]));
    // }
    // printf("%s\n\n\n", CreateFullOutputString(data.bX));
    // printf("~Unsolved Remainging: %d\n", data.unsolvedX);
    if (data.unsolvedX == 0) {
      data.solved = true;
      // printf("ding");

      return data.bX[0];
    }
    if (data.bX[0] > 50000) {
      printf("Limit Hit: 50,000");
      // printf(CreateFullOutputString(data.bX));
      return 0;
    }
    // printf("%d\n",finds);
  } while (finds > 0);
  // printf("dong");
  // FullState(input, board);
  return data.bX[0]*-1;
}

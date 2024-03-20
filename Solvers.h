#include "Convert.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma region Helper Funcs
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
/// @brief Writes to naX the 9 cells of the row and the index of each cell to
/// ncX
/// @param n Row Number
/// @param H HandOff Struct
void GetRow(Row n, HandOff(*H)) {
  memset(H->naX, 0, 9 * sizeof(uint16_t));
  memset(H->ncX, 0, 9 * sizeof(int));
  int rs = (n - 1) * 9 + 1;

  for (int i = 0; i < 9; i++) {
    H->naX[i] = H->bX[rs + i];
    H->ncX[i] = (rs + i);
    if (H->ncX[i] > 81) {
      printf("Get Row Error: %d\n", H->ncX[i]);
    }
  }
}
/// @brief Writes to naX the 9 cells of the column and the index of each cell to
/// ncX
/// @param n Column Number
/// @param H handOff Struct
void GetCol(Col n, HandOff(*H)) {
  memset(H->naX, 0, 9 * sizeof(uint16_t));
  memset(H->ncX, 0, 9 * sizeof(int));
  for (int i = 0; i < 9; i++) {
    H->naX[i] = H->bX[n + (i * 9)];
    H->ncX[i] = (n + (i * 9));
    if (H->ncX[i] > 81) {
      printf("Get Col Error: %d\n", H->ncX[i]);
    }
  }
}
/// @brief Writes to naX the 9 cells of the box and the index of each cell to
/// ncX
/// @param n
/// @param board
/// @param dest
void GetBox(Box n, HandOff(*H)) {
  memset(H->naX, 0, 9 * sizeof(uint16_t));
  memset(H->ncX, 0, 9 * sizeof(int));
  int z = GetZStart(n);
  int jmp = 0;
  for (int i = 0; i < 9; i++) {
    if (i % 3 == 0 && i != 0) {
      jmp += 6;
    }
    H->naX[i] = H->bX[z + i + jmp];
    H->ncX[i] = z + i + jmp;
    if (H->ncX[i] > 81) {
      printf("Get Col Error: %d\n", H->ncX[i]);
      assert(false);
    }
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
      printf("Index %d has %d usable duplicate(s)\n Count: %d Candidates: %s "
             "\n Row: ",
             i, H->nbX[i], count, PrintCellString(H->naX[i],false));
      for (int j = 0; j < 9; j++) {
        printf("%s,", PrintCellString(H->naX[j],false));
      }
      printf("\n");
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
#pragma endregion
int AnswerCellFromGroupCandidates(HandOff(*H)) {
  int solves = 0;
  memset(H->nbX, 0, 9 * sizeof(uint16_t));
  for (int i = 0; i < 9; i++) {
    // if the cell is solved mark that number as solved

    if (H->naX[i] & 1) {
      H->nbX[CellToInt(H->naX[i]) - 1] = 8192;
      continue;
    }
    for (int j = 1; j < 10; j++) {
      // if numeral is solved skip
      if (H->nbX[j - 1] > 83) {
        continue;
      }
      // if Ax could be numeral j
      if (H->naX[i] & (1 << j)) {
        // if this is the first time we've seen this numeral set nbX to the
        // index of naX
        if (H->nbX[j - 1] == 0) {
          H->nbX[j - 1] = H->ncX[i-1];
          continue;
        }
        // if we've seen this numeral before set nbX to 8192 to ignore it in
        // future loops
        if (H->nbX[j - 1] < 83) {
          H->nbX[j - 1] = 8192;
          continue;
        }
      }
    }
  }
  PrintnaXToInt(H);
  PrintnbXToInt(H);
  PrintncXToInt(H);
  for (int i = 1; i < 10; i++) {
    if (H->nbX[i-1] > 0 && H->nbX[i-1] < 83) {
      uint16_t temp = 0;
      temp = 2 ^ (i + 2);
      PrintCellBin(H->nbX[i-1],true);
      printf("%d Index# %d solved, Answer is %d\n",i, H->ncX[i-1],
             CellToInt(temp + 1));

             getchar();
      H->bX[H->nbX[i-1]] = temp + 1;
      solves++;
      H->unsolvedX--;
    }
  }
  return solves;
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
      uint16_t newElims = r | c | b;
      // printf("n: %s\n\n", PrintCellBin(newElims));
      elimed = elimed | r | c | b;
      // printf("E: %s\n\n", PrintCellBin(elimed));
    }
    // PrintSimpleMates(H);
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
      // printf("Index# %d solved, Answer is %d == POSTFIND %d Unknowns
      // Remain\n",
      //        i, CellToInt(H->bX[i]), H->unsolvedX);
      if (!findAdded) {
        finds++;
      }
      continue;
    }
  }
  return finds;
}
int IterateGroups(HandOff(*H)) {
  int ig_finds = 0;
  uint16_t eliminated = 0;
  uint16_t temp = 0;
  memset(H->nbX, 0, 9 * sizeof(uint16_t));
  memset(H->ncX, 0, 9 * sizeof(int));
  for (int i = 1; i < 10; i++) {
    GetRow(i, H);
    eliminated = 0;
    for (int j = 0; j < 9; j++) {
      if (IsCellSolved((H->naX[j]))) {
        eliminated |= (H->naX[j] - 1);
      }
    }
    for (int j = 0; j < 9; j++) {
      if (!IsCellSolved(H->naX[j])) {
        temp = H->bX[H->ncX[j]];
        H->bX[H->ncX[j]] &= ~eliminated;
        if (temp != H->bX[H->ncX[j]]) {
          ig_finds++;
          if (CellIsSolvable(H->bX[i]) == true && H->bX[i] > 0) {
            H->bX[i] |= 1;
            H->unsolvedX--;
          }
        }
      }
    }
    ig_finds += AnswerCellFromGroupCandidates(H);
    GetCol(i, H);
    eliminated = 0;
    temp = 0;
    for (int j = 0; j < 9; j++) {
      if (IsCellSolved((H->naX[j]))) {
        eliminated |= (H->naX[j] - 1);
      }
    }
    for (int j = 0; j < 9; j++) {
      if (!IsCellSolved((H->naX[j]))) {
        temp = H->bX[H->ncX[j]];
        H->bX[H->ncX[j]] &= ~eliminated;
        if (temp != H->bX[H->ncX[j]]) {
          ig_finds++;
          if (CellIsSolvable(H->bX[i]) == true && H->bX[i] > 0) {
            H->bX[i] |= 1;
            H->unsolvedX--;
          }
        }
      }
    }
    ig_finds += AnswerCellFromGroupCandidates(H);
    GetBox(i, H);
    eliminated = 0;
    temp = 0;
    for (int j = 0; j < 9; j++) {
      if (IsCellSolved((H->naX[j]))) {
        eliminated |= (H->naX[j] - 1);
      }
    }
    for (int j = 0; j < 9; j++) {
      if (!IsCellSolved((H->naX[j]))) {
        temp = H->bX[H->ncX[j]];
        H->bX[H->ncX[j]] &= ~eliminated;
        if (temp != H->bX[H->ncX[j]]) {
          ig_finds++;
          if (CellIsSolvable(H->bX[i]) == true && H->bX[i] > 0) {
            H->bX[i] |= 1;
            H->unsolvedX--;
          }
        }
      }
    }
    ig_finds += AnswerCellFromGroupCandidates(H);
  }
  return ig_finds;
}
int Solve(Board board, Globals *globals, int unknown) {
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
  data.ncX = globals->ncG;
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
    // printf("Finds: %d\n", finds);
    if (finds == 0) {
      finds = IterateGroups(&data);
    }
    // printf("Finds: %d\n", finds);
    // getchar();
    // FindNakedPartners(&data);
    // Part C: Eliminate candidates
    // Part D: increment iteration count
    data.bX[0]++;
    // printf("==================================================================="
    //        "=======\n");
    printf("Iterataion: %d\n", data.bX[0]);
    // printf("Findings:%d\n", finds);
    // for (int i = 1; i < 82; i++) {
    //   printf("Cell: %d %s - %s\n", i, PrintCellBin(data.bX[i]),
    //          PrintCellString(data.bX[i]));
    // }
    printf("%s\n\n\n", CreateFullOutputString(data.bX,false));
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
  return data.bX[0] * -1;
}

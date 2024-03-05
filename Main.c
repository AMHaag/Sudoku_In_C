#include <stdio.h>
#include <stdint.h>

#include "Conversion.h"
#include "Solvers.h"
#include "Debug.h"

// Global Variables
char input[81] = "070000043040009610800634900094052000358460020000800530080070091902100005007040802";

void convert_input(char input[81], uint16_t board[82])
{
    for (int i = 1; i < 82; i++)
    {
        board[i] = char_to_bitmask(input[i - 1]);
    }
}

void test(int n)
{
    struct Position a;
    a = GetPosition(n);
    printf("Input: %d Row: %d Col: %d Box: %d Zx: %d Zy: %d Zindex: %d\n\n", n, a.x, a.y, a.z, a.zx, a.zy, a.zindex);
}

// Main
int main()
{
    uint16_t board[82] = {0};
    convert_input(input, board);

    for (int i = 1; i < 82; i++)
    {
        if (input[i - 1] == '0')
        {
            continue;
        }
        printf("Input: %c     ", input[i - 1]);
        PrintBinString(board[i]);
        PrintBin(board[i]);
    }
    // printf("Input: %c\n", input[1]);
    // PrintBin(board[2]);
    // PrintBinString(board[2]);

    // printf("Input: %c\n", input[7]);
    // PrintBin(board[8]);
    // PrintBinString(board[8]);

    // printf("Input: %c\n", input[8]);
    // PrintBin(board[9]);
    // PrintBinString(board[9]);
    // test(21);
    // test(35);
    // test(44);
    // test(68);
}

int Solve(uint16_t puzzle)
{

    // Variables
    struct HandOff HandOff;
    uint16_t board[82] = {0};
    uint16_t rowMates[8] = {0};
    uint16_t colMates[8] = {0};
    uint16_t boxMates[8] = {0};
    int solveCount = 0;
    int elimCount = 0;
    HandOff.bX = &board;
    HandOff.rmX = &rowMates;
    HandOff.cmX = &colMates;
    HandOff.bxX = &boxMates;
    HandOff.sX = &solveCount;
    HandOff.eX = &elimCount;

    // Prework: First Pass find candidates
    FindBasicCandidates(HandOff);

    //* Do/While
    do
    {
        solveCount = 0;
        elimCount = 0;
        // Part A: Check for Solved or Mistakes
        CheckStatus(HandOff);
        // Part B: Solve Cells
        SolveCells(HandOff);
        // Part C: Eliminate candidates
        FindAdvCandidates(HandOff);
    } while (solveCount + elimCount != 0);
    // Part D: Find Best Cell for first Guess
    // Part E: Recurse / MultiThread-Guess
}

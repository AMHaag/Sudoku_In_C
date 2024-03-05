#include <stdio.h>
#include <stdint.h>
#include "Debug.h"
#include "Access.h"
#include "Conversion.h"

// Global Variables
char input[82] = "070000043040009610800634900094052000358460020000800530080070091902100005007040802";
uint16_t board[82] = {0};
uint16_t rowMates[8] = {0};
uint16_t colMates[8] = {0};
uint16_t boxMates[8] = {0};
void convert_input(char input[81])
{
    for (int i = 1; i < 82; i++)
    {
        board[i] = char_to_bitmask(input[i-1]);
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
    convert_input(input);
    test(21);
    test(44);
    test(68);
}
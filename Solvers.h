#include <stdio.h>
#include <stdint.h>

struct HandOff
{
    uint16_t *bX;
    uint16_t *rmX;
    uint16_t *cmX;
    uint16_t *bxX;
    int *sX;
    int *eX;
};
void FindBasicCandidates(struct HandOff H) {
    for(int i = 1;i<82;i++){
        uint16_t cell = H.bX[i];

    }
}

void CheckStatus(struct HandOff H) {}

void SolveCells(struct HandOff H) {}

void FindAdvCandidates(struct HandOff H) {}

/**
 * @struct Position
 * @brief  This struct represents the position of a cell in the overall board.
 */
struct Position
{
    int x;      /** The Row the cell is in  (1-based)*/
    int y;      /** The Column the cell is in (1-based)*/
    int z;      /** The Box the cell is in (1-based)*/
    int zx;     /** The 'Row' within the box the cell is in */
    int zy;     /** The 'Column' within the box the cell is in */
    int zindex; /** The index number of the cell within the Box*/
};

/// @brief Returns the Row, Column, Box, and box coordinates of a the input cell.
/// @param i The Index you are ascertaining the position of.
/// @return struct Position
/// @see Position
struct Position GetPosition(int n)
{
    struct Position out;
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

void FillMates(int n, struct HandOff H)
{
    struct Position Position;
    Position = GetPosition(n);
    for(int i = 0;i<8;i++){
        int skip = 0;
        int rowStart = ((Position.x-1)*9)+1;
        for(int j = 0;j<9;i++){
            if(j==Position.y){skip++;}
            H.rmX[j] = H.bX[rowStart+j+skip];
        }
        skip=0;
        int colStart = Position.y;
        for(int j = 0;j<9;i++){
            if(j==Position.x){skip++;}
            H.cmX[j] = H.bX[colStart+(j*9)+skip];
        }
        skip=0;
    }
}
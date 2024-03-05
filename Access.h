#include <stdint.h>
#include <stdlib.h>
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
struct Position GetPosition(int i)
{
    struct Position out;
    out.y = (i - 1) % 9 + 1;
    out.x = (i - 1) / 9 + 1;
    out.z = 3 * ((out.x - 1) / 3) + ((out.y - 1) / 3) + 1;
    out.zx = (out.x - 1) % 3;
    out.zy = (out.x - 1) % 3;
    out.zindex = 3 * out.zx + out.zy + 1;
    out.zx++;
    out.zy++;
    return out;
}
int* FillMates(int* board, int* rowMates, int* colMates, int* boxMate, struct Position p){
    
    
}


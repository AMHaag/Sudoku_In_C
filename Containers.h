#include <stdbool.h>
#include <stdint.h>
#include <string.h>
/**
 * @brief This struct is used to hold the state of the board. It is pertinent
 * only to the function that invokes it.
 */
typedef struct {
  uint16_t(*bX);  // The board
  uint16_t(*rmX); // The Reusable RowMates Array
  uint16_t(*cmX); // The Reusable ColumnMates Array
  uint16_t(*bxX); // The Reusable BoxMates Array
  uint16_t(*naX); // The Reusable NineArray
  int unsolvedX; // The number of unknown cells
  bool solved;   // The state of the board
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
  uint16_t(*naG);
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

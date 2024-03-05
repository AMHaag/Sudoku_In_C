#include <stdint.h>

uint16_t char_to_bitmask(char c)
{
    if (c == '0')
    {
        uint16_t unknown = 0b0000001111111110;
        return unknown; // 0000001111111110
    }
    int c_int = c - '0';
    uint16_t mask;
    mask |= (1 << c_int);
    mask |= (1 << 0);
    return mask;
}


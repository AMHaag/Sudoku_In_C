#include <stdint.h>
#include <stdlib.h>

int lowToHigh(const void *a, const void *b)
{
    // Cast the void pointers to the appropriate type
    const int *num1 = (const int *)a;
    const int *num2 = (const int *)b;

    // The comparison
    // If num1 is less than num2, return -1
    if (*num1 < *num2)
        return -1;

    // If num1 is greater than num2, return 1
    if (*num1 > *num2)
        return 1;

    // If num1 is equal to num2, return 0
    return 0;
}



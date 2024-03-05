#include <stdint.h>
#include <stdio.h>

void PrintBin(uint16_t num)
{
    for (int i = 15; i >= 0; i--)
    {
        if ((i + 1) % 4 == 0)
        {
            printf(" ");
        }
        uint16_t mask = 0;
        mask = 1 << i;
        if (num & mask)
        {
            printf("1");
        }
        else
        {
            printf("0");
        }
    }
    printf("\n");
}

void PrintBinString(uint16_t num)
{
    uint16_t mask = 0;
    mask = 1 << 0;
    if (!num & mask)
    {
        mask = 0;
        int x = 0;
        int n = 9;
        int cands[9] = {0};
        for (int i = 6; i < 15; i++)
        {
            mask = 32768 >> i;
            if (num & mask)
            {
                cands[x] = n;
            }
            else
            {
                n--;
            }
            x++;
        }
        printf("Converted: ");
        for (int i = 0; i < 9; i++)
        {
            if (cands[i] !=)
        }
    }
    mask = 0;
    int n = 9;
    for (int i = 6; i < 15; i++)
    {
        mask = 32768 >> i;
        if (num & mask)
        {
            printf("Converted: %d", n);
        }
        else
        {
            n--;
        }
    }
}
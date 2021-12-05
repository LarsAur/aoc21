#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INDEX(x, y) ((x) + (y) * max_x)
#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)

int hydrothermal_venture(bool useDiagonals);

int main()
{

    int a = hydrothermal_venture(false);
    int b = hydrothermal_venture(true);

    printf("A: %d overlaps\n", a);
    printf("B: %d overlaps\n", b);

    return 0;
}

int hydrothermal_venture(bool useDiagonals)
{
    FILE* fp = fopen("./inputs.txt", "r");
    if(fp == NULL)
    {
        perror("Unable to open file");
        exit(1);
    }

    int x1, x2, y1, y2;
    int max_x = 0;
    int max_y = 0;
    while(fscanf(fp, "%d,%d -> %d,%d", &x1, &y1, &x2, &y2) != EOF)
    {
        if(MAX(x1, x2) > max_x) max_x = MAX(x1, x2);
        if(MAX(y1, y2) > max_y) max_y = MAX(y1, y2);
    }

    // Increment by 1 because there can be a zero index
    max_x++;
    max_y++;

    int *arr = calloc(max_x * max_y, sizeof(int));

    rewind(fp);
    while(fscanf(fp, "%d,%d -> %d,%d", &x1, &y1, &x2, &y2) != EOF)
    {
        if(x1 == x2)
        {
            for(int i = MIN(y1, y2); i <= MAX(y1, y2); i++)
            {
                arr[INDEX(x1, i)]++;
            }
        }
        else if(y1 == y2)
        {
            for(int i = MIN(x1, x2); i <= MAX(x1, x2); i++)
            {
                arr[INDEX(i, y1)]++;
            }
        }
        // Diagonal lines are always 45 degrees
        else if(useDiagonals)
        {
            int len = abs(x1 - x2);
            int dx = x1 < x2 ? 1 : -1;
            int dy = y1 < y2 ? 1 : -1;
            for(int i = 0; i <= len; i++)
            {
                arr[INDEX(x1 + i * dx, y1 + i * dy)]++;
            }
        }
    }

    // Count the number of overlaps
    int overlaps = 0;
    for(int i = 0; i < max_x * max_y; i++)
    {
        if(arr[i] > 1) overlaps++;
    }

    return overlaps;
}
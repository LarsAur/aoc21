#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define WIDTH 100
#define HEIGHT 100

int find_basin_size(uint8_t field[WIDTH][HEIGHT], int x, int y);
void dfs_size(uint8_t field[WIDTH][HEIGHT], uint8_t marks[WIDTH][HEIGHT], int x, int y, int *size);

int main()
{
    FILE *fp = fopen("./inputs.txt", "r");
    if (fp == NULL)
    {
        perror("Unable to open file");
        exit(1);
    }


    uint8_t field[WIDTH][HEIGHT];
    for(int i = 0; i < HEIGHT; i++)
    {
        for(int j = 0; j < WIDTH; j++)
        {
            fscanf(fp, "%1hhd", &field[j][i]);
        }
    }

    int total_risk = 0;
    int top_basin_sizes[3];
    memset(top_basin_sizes, 0, sizeof(int) * 3);
    for(int i = 0; i < HEIGHT; i++)
    {
        for(int j = 0; j < WIDTH; j++)
        {
            if(i > 0 && field[j][i] >= field[j][i-1]) continue;                  // Check up
            if(i < HEIGHT - 1 && field[j][i] >= field[j][i+1]) continue;         // Check down
            if(j > 0 && field[j][i] >= field[j - 1][i]) continue;                // Check left
            if(j < WIDTH - 1 && field[j][i] >= field[j+1][i]) continue;          // Check right

            total_risk += field[j][i] + 1;

            int basin_size = find_basin_size(field, j, i);
            // Quick and dirty way of keeping a top 3
            if(basin_size > top_basin_sizes[2])
            {
                top_basin_sizes[0] = top_basin_sizes[1];
                top_basin_sizes[1] = top_basin_sizes[2];
                top_basin_sizes[2] = basin_size;
            }
            else if(basin_size > top_basin_sizes[1])
            {
                top_basin_sizes[0] = top_basin_sizes[1];
                top_basin_sizes[1] = basin_size; 
            }
            else if(basin_size > top_basin_sizes[0])
            {
                top_basin_sizes[0] = basin_size;
            }
        }
    }

    printf("Total risk: %d\n", total_risk);
    printf("Product of 3 largest basins: %d\n", top_basin_sizes[0] * top_basin_sizes[1] * top_basin_sizes[2]);
}

int find_basin_size(uint8_t field[WIDTH][HEIGHT], int x, int y)
{
    uint8_t marks[WIDTH][HEIGHT];
    memset(marks, 0, WIDTH * HEIGHT);
    int size = 0;
    dfs_size(field, marks, x, y, &size);
    printf("%d\n", size);
    return size;
}

void dfs_size(uint8_t field[WIDTH][HEIGHT], uint8_t marks[WIDTH][HEIGHT], int x, int y, int *size)
{
    if(x < 0) return;
    if(y < 0) return;
    if(x > WIDTH- 1) return;
    if(y > HEIGHT - 1) return;
    if(field[x][y] == 9) return;
    if(marks[x][y]) return;

    marks[x][y] = 1;
    (*size)++;
    bfs_size(field, marks, x + 1, y, size);
    bfs_size(field, marks, x - 1, y, size);
    bfs_size(field, marks, x, y + 1, size);
    bfs_size(field, marks, x, y - 1, size);
}

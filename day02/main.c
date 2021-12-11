#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dive_a();
void dive_b();

typedef struct 
{
    int x;
    int y; 
    int aim;   
} position;

int main()
{
    dive_a();
    dive_b();
    return 0;
}

void dive_a()
{
    FILE* fp = fopen("./inputs.txt", "r");

    if(fp == NULL)
    {
        perror("Unable to open file");
        fclose(fp);
        exit(1);
    }

    // Longest word is "forward"
    char dir[7];
    int dist;

    position pos = {
        .x = 0,
        .y = 0
    };

    while(fscanf(fp, "%s %d", dir, &dist) != EOF)
    {
        if(strcmp(dir, "forward") == 0) pos.x += dist;
        else if(strcmp(dir, "down") == 0) pos.y += dist;
        else if(strcmp(dir, "up") == 0) pos.y -= dist;
    }

    fclose(fp);

    printf("A: %d\n", pos.x * pos.y);
}

void dive_b()
{
    FILE* fp = fopen("./inputs.txt", "r");

    if(fp == NULL)
    {
        perror("Unable to open file");
        fclose(fp);
        exit(1);
    }

    // Longest word is "forward"
    char dir[7];
    int dist;

    position pos = {
        .x = 0,
        .y = 0,
        .aim = 0
    };

    while(fscanf(fp, "%s %d", dir, &dist) != EOF)
    {
        if(strcmp(dir, "forward") == 0) {pos.x += dist; pos.y += pos.aim * dist;}
        else if(strcmp(dir, "down") == 0) pos.aim += dist;
        else if(strcmp(dir, "up") == 0) pos.aim -= dist;
    }

    fclose(fp);

    printf("B: %d\n", pos.x * pos.y);
}
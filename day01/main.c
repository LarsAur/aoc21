#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

// Perform task a by setting the window size to 1
#define WINDOW_SIZE_A 1
#define WINDOW_SIZE_B 3

int get_num_increases(int window);

int main()
{
    printf("A: %d\n", get_num_increases(WINDOW_SIZE_A));
    printf("B: %d\n", get_num_increases(WINDOW_SIZE_B));
    return 0;
}

int get_num_increases(int window)
{
    FILE* fp = fopen("./inputs.txt", "r");

    if(fp == NULL)
    {
        perror("Unable to open file");
        exit(1);
    }

    int value_buffer[window + 1];
    int counter = 0;
    
    // Initiate the buffer to INT_MAX to avoid incrementations while the buffer is filling
    for(int i = 0; i < window + 1; i++) value_buffer[i] = INT_MAX;

    while (fscanf(fp, "%d", &value_buffer[0]) != EOF)
    {
        // If the value going out of the window is less then the value going in, the som is increased
        if(value_buffer[0] > value_buffer[window]) counter++;
        
        for(int i = window-1; i >= 0; i--)
        {
            value_buffer[i + 1] = value_buffer[i];
        }
    }

    fclose(fp);

    return counter;
}
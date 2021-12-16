#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

uint8_t *parse_input(int *width, int *height);
int get_min_distance_vertex(bool *queue, int *dist, int size);
int dijkstra(uint8_t *weights, int width, int height);
void x5_input(uint8_t *weights, uint8_t *weights2, int width, int height);

int main()
{
    int width, height;
    uint8_t *weights = parse_input(&width, &height);
    uint8_t weights2[width*5 * height*5];

    x5_input(weights, weights2, width, height);

    int cost1 = dijkstra(weights, width, height);
    printf("Cost: %d\n", cost1);
    int cost2 = dijkstra(weights2, width*5, height*5);
    printf("Cost: %d\n", cost2);

    free(weights);
    return 0;
}

void x5_input(uint8_t *weights, uint8_t *weights2, int width, int height)
{
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            for(int k = 0; k < 5; k++)
            {
                weights2[i * width * 5 + k * width + j] = (weights[i * width + j] + k) % 9;
                if(weights2[i * width * 5 + k * width + j] == 0) weights2[i * width * 5 + k * width + j] = 9;
            }
        }
    }

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            for(int k = 0; k < 5; k++)
            {
                for(int l = 0; l < 5; l++)
                {
                    weights2[l * width * 5 * height + i * width * 5 + k * width + j] = (weights2[i * width * 5 + k * width + j] + l) % 9;
                    if(weights2[l * width * 5 * height + i * width * 5 + k * width + j] == 0) weights2[l * width * 5 * height + i * width * 5 + k * width + j] = 9;
                }
            }
        }
    }
}

/* 
    Note this version of dijkstra could be made faster by terminating when the end node is out of the queue 
    and by using a queue which can give the smallest values quicker
*/
int dijkstra(uint8_t *weights, int width, int height)
{
    bool queue[width * height];
    int dist[width * height];
    for (int i = 0; i < width * height; i++)
    {
        dist[i] = INT_MAX;
        queue[i] = true;
    }
    dist[0] = 0;

    for (int i = 0; i < width * height; i++)
    {
        dist[i] = INT_MAX;
        queue[i] = true;
    }
    dist[0] = 0;

    int v_index;
    while ((v_index = get_min_distance_vertex(queue, dist, height * width)) != -1)
    {
        queue[v_index] = false;

        // For each neighbour still in queue
        int vx = v_index % width;
        int vy = v_index / width;

        int neighbour = vy * width + (vx - 1);
        if (vx > 0 && queue[neighbour])
        {
            int alt = dist[v_index] + weights[neighbour];
            if (alt < dist[neighbour]) dist[neighbour] = alt;
        }

        neighbour = (vy - 1) * width + vx;
        if (vy > 0 && queue[neighbour])
        {
            int alt = dist[v_index] + weights[neighbour];
            if (alt < dist[neighbour]) dist[neighbour] = alt;
        }

        neighbour = vy * width + (vx + 1);
        if (vx < height - 1 && queue[neighbour])
        {
            int alt = dist[v_index] + weights[neighbour];
            if (alt < dist[neighbour]) dist[neighbour] = alt;
        }

        neighbour = (vy + 1) * width + vx;
        if (vy < width - 1 && queue[neighbour])
        {
            int alt = dist[v_index] + weights[neighbour];
            if (alt < dist[neighbour]) dist[neighbour] = alt;
        }
    }

    return dist[width * height - 1];
}

int get_min_distance_vertex(bool *queue, int *dist, int size)
{
    int min = INT_MAX;
    int min_index = -1;

    for (int i = 0; i < size; i++)
    {
        if (queue[i] && dist[i] < min)
        {
            min = dist[i];
            min_index = i;
        }
    }

    return min_index;
}

uint8_t *parse_input(int *width, int *height)
{
    FILE *fp = fopen("./inputs.txt", "r");
    if (fp == NULL)
    {
        perror("Unable to open file");
        fclose(fp);
        exit(1);
    }

    // Find the dimentions of the input
    *width = 0;
    *height = 0;
    while (fgetc(fp) != '\n')
        (*width)++;
    fseek(fp, 0, SEEK_END);
    *height = ftell(fp) / *width;
    rewind(fp);

    uint8_t *arr = malloc((*width) * (*height) * sizeof(uint8_t));
    int i = 0;
    while (fscanf(fp, "%1hhd", &arr[i++]) != EOF)
        ;

    printf("Width: %d, Height: %d\n", *width, *height);

    return arr;
}
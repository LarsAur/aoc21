#include <stdio.h>
#include <stdlib.h>

#define SPAWN_TIME 7
#define NEW_SPAWN_TIME 2

#define TIME_SPAN_B 256
#define TIME_SPAN_A 80

long get_population_size(const int time);

int main()
{
    long pop_a = get_population_size(TIME_SPAN_A);
    long pop_b = get_population_size(TIME_SPAN_B);
    printf("A population size: %ld\n", pop_a);
    printf("B population size: %ld\n", pop_b);

    return 0;
}

long get_population_size(const int time)
{
    FILE *fp = fopen("./inputs.txt", "r");
    if (fp == NULL)
    {
        perror("Unable to open file");
        exit(1);
    }

    // Initialize the population
    long population[SPAWN_TIME] = {0};
    int input;
    while (fscanf(fp, "%d,", &input) != EOF)
    {
        population[input]++;
    }

    // Loop over all days and double the part of the population which have an internal timer of 0
    // the new fish are inserted into the "new_population" which wait for 2 days before being added into "mature" population
    long new_population[NEW_SPAWN_TIME] = {0};
    for (int i = 0; i < time; i++)
    {
        long tmp = population[i % SPAWN_TIME];
        population[i % SPAWN_TIME] += new_population[NEW_SPAWN_TIME - 1];
        for (int j = 0; j < NEW_SPAWN_TIME - 1; j++)
        {
            new_population[j + 1] = new_population[j];
        }
        new_population[0] = tmp;
    }

    // Sum the mature and new population
    long population_size = 0;
    for(int i = 0; i < SPAWN_TIME; i++)
    {
        population_size += population[i];
    }
    for(int i = 0; i < NEW_SPAWN_TIME; i++)
    {
        population_size += new_population[i];
    }

    return population_size;
}
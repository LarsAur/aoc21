#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

#define WIDTH 10
#define HEIGHT 10
#define ITERATIONS 100

typedef struct
{
    uint8_t x;
    uint8_t y;
} coord_t;

typedef struct
{
    void *arr;
    uint8_t byte_size;
    size_t size;
    size_t _alloc_size;
} list_t;

void add_all_to_inc(list_t *to_inc);
void perform_flash(list_t *to_inc, list_t *to_reset, uint8_t energy[WIDTH][HEIGHT], bool flash[WIDTH][HEIGHT]);
void reset_flashing(list_t *to_reset, uint8_t energy[WIDTH][HEIGHT], bool flash[WIDTH][HEIGHT]);
void print(uint8_t energy[WIDTH][HEIGHT], bool flash[WIDTH][HEIGHT]);
void *pop(list_t *list);
void append(list_t *list, void *element);

int main()
{
    uint8_t energy[WIDTH][HEIGHT];
    bool flash[WIDTH][HEIGHT];
    int total_flashes = 0;

    list_t to_inc =
    {
        .arr = malloc(sizeof(coord_t)),
        .byte_size = sizeof(coord_t),
        .size = 0,
        ._alloc_size = 1,
    };

    list_t to_reset =
    {
        .arr = malloc(sizeof(coord_t)),
        .byte_size = sizeof(coord_t),
        .size = 0,
        ._alloc_size = 1,
    };

    FILE *fp = fopen("./inputs.txt", "r");
    if (fp == NULL)
    {
        perror("Unable to open file");
        fclose(fp);
        exit(1);
    }

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (fscanf(fp, "%1hhu", &energy[j][i]) == EOF)
            {
                printf("Input format error");
                exit(1);
            }
        }
    }

    fclose(fp);

    memset(flash, false, WIDTH * HEIGHT);
    print(energy, flash);

    for (int i = 0; i < ITERATIONS; i++)
    {
        add_all_to_inc(&to_inc);
        perform_flash(&to_inc, &to_reset, energy, flash);
        total_flashes += to_reset.size;
        reset_flashing(&to_reset, energy, flash);
    }
    printf("Total flashes: %d\n", total_flashes);

    int i = ITERATIONS;
    while(to_reset.size != WIDTH * HEIGHT)
    {
        reset_flashing(&to_reset, energy, flash);
        add_all_to_inc(&to_inc);
        perform_flash(&to_inc, &to_reset, energy, flash);
        i++;
    }
    printf("All flash at: %d\n", i);

    free(to_inc.arr);
    free(to_reset.arr);

    return 0;
}

void add_all_to_inc(list_t *to_inc)
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            coord_t coord = {.x = j, .y = i};
            append(to_inc, &coord);
        }
    }
}

void perform_flash(list_t *to_inc, list_t *to_reset, uint8_t energy[WIDTH][HEIGHT], bool flash[WIDTH][HEIGHT])
{
    while (to_inc->size != 0)
        {
            coord_t coord = *((coord_t *)pop(to_inc));
            energy[coord.x][coord.y]++;

            if (!flash[coord.x][coord.y] && energy[coord.x][coord.y] > 9)
            {
                flash[coord.x][coord.y] = true;
                append(to_reset, &coord);

                if (coord.x > 0) {coord_t c = {coord.x - 1, coord.y}; append(to_inc, &c);}
                if (coord.y > 0) {coord_t c = {coord.x, coord.y - 1}; append(to_inc, &c);}
                if (coord.x < WIDTH - 1) {coord_t c = {coord.x + 1, coord.y}; append(to_inc, &c);}
                if (coord.y < HEIGHT - 1) {coord_t c = {coord.x, coord.y + 1}; append(to_inc, &c);}

                if (coord.x > 0 && coord.y > 0) {coord_t c = {coord.x - 1, coord.y - 1}; append(to_inc, &c);}
                if (coord.x > 0 && coord.y < HEIGHT - 1) {coord_t c = {coord.x - 1, coord.y + 1}; append(to_inc, &c);}
                if (coord.x < WIDTH - 1 && coord.y > 0) {coord_t c = {coord.x + 1, coord.y - 1}; append(to_inc, &c);}
                if (coord.x < WIDTH - 1 && coord.y < HEIGHT - 1) {coord_t c = {coord.x + 1, coord.y + 1}; append(to_inc, &c);}
            }
        }
}

void reset_flashing(list_t *to_reset, uint8_t energy[WIDTH][HEIGHT], bool flash[WIDTH][HEIGHT])
{
    while (to_reset->size != 0)
    {
        coord_t coord = *((coord_t *)pop(to_reset));
        energy[coord.x][coord.y] = 0;
        flash[coord.x][coord.y] = false;
    }

}

void append(list_t *list, void *element)
{
    if (list->size == list->_alloc_size)
    {
        list->_alloc_size *= 2;
        list->arr = realloc(list->arr, list->_alloc_size * list->byte_size);
    }

    list->size++;
    memcpy(list->arr + list->byte_size * list->size, element, list->byte_size);
}

void *pop(list_t *list)
{
    if (list->size == 0)
        return NULL;
    list->size--;
    return list->arr + list->byte_size * (list->size + 1);
}

void print(uint8_t energy[WIDTH][HEIGHT], bool flash[WIDTH][HEIGHT])
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (energy[j][i] == 0)
            {
                printf("\033[0;31m");
            }
            printf("%1hhu", energy[j][i]);
            printf("\033[0m");
        }
        printf("\n");
    }
}

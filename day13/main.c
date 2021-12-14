#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX(a, b) ((a) < (b) ? (b) : (a))

typedef struct
{
    int x;
    int y;
} point_t;

typedef struct
{
    int pos;
    char axis;
} fold_t;

void parseInput(point_t **points, int* n_points, fold_t **folds, int *n_folds);
void fold(point_t *points, int n_points, fold_t fold);
int count_unique(point_t *points, int n_points);
void print_points(point_t *points, int n_points);

int main()
{
    point_t *points = NULL;
    fold_t *folds = NULL;
    int n_points = 0;
    int n_folds = 0;

    parseInput(&points, &n_points, &folds, &n_folds);

    fold(points, n_points, folds[0]);

    int a = count_unique(points, n_points);
    printf("Unique after one fold: %d\n", a);

    for(int i = 0; i < n_folds; i++)
    {
        fold(points, n_points, folds[i]);
    }

    print_points(points, n_points);

    free(points);
    free(folds);

    return 0;
}

void print_points(point_t *points, int n_points)
{
    int xmax = 0;
    int ymax = 0;

    for(int i = 0; i < n_points; i++)
    {
        xmax = MAX(xmax, points[i].x);
        ymax = MAX(ymax, points[i].y);
    }

    xmax++;
    ymax++;

    char string[(xmax + 1) * (ymax + 1)];
    memset(string, '.', (xmax + 1) * (ymax+1));

    for(int i = 0; i < n_points; i++)
    {
        string[points[i].x + 1 + (xmax + 1) * points[i].y] = '#';
    }
    for(int y = 0; y < ymax; y++)
    {
        string[(xmax + 1) * (y)] = '\n';
    }
    string[(xmax + 1) * ymax] = '\0';
    printf("%s\n", string);
}

int count_unique(point_t *points, int n_points)
{
    point_t uniques[n_points];
    int n_unique = 0;

    for(int i = 0; i < n_points; i++)
    {
        bool unique = true;
        for(int j = 0; j < n_unique; j++)
        {
            if(points[i].x == uniques[j].x && points[i].y == uniques[j].y)
            {
                unique = false;
                break;
            }
        }

        if(unique)
        {
            uniques[n_unique++] = points[i];
        }
    }

    return n_unique;
}

void fold(point_t *points, int n_points, fold_t fold)
{
    if(fold.axis == 'x')
    {
        for(int i = 0; i < n_points; i++)
        {
            if(points[i].x >= fold.pos)
            {
                points[i].x = 2 * fold.pos - points[i].x;
            }
        }
    }
    else
    {
        for(int i = 0; i < n_points; i++)
        {
            if(points[i].y >= fold.pos)
            {
                points[i].y = 2 * fold.pos - points[i].y;
            }
        }
    }
}

void parseInput(point_t **points, int* n_points, fold_t **folds, int *n_folds)
{
    *n_points = 0;
    *n_folds = 0;

    FILE* fp = fopen("./inputs.txt", "r");
    if(fp == NULL)
    {
        perror("Unable to open file");
        fclose(fp);
        exit(1);
    }

    char str[100];
    while(fgets(str, 100, fp) != NULL)
    {
        if(str[0] == '\n') break;

        int i = 0;
        while(str[i++] != ',');
        str[i-1] = '\0';
        
        *points = realloc(*points, ++(*n_points) * sizeof(point_t));
        (*points)[(*n_points) - 1].x = atoi(str);
        (*points)[(*n_points) - 1].y = atoi(str + i);
    }

    while(fgets(str, 100, fp) != NULL)
    {
        int i = 0;
        while(str[++i] != 'x' && str[i] != 'y');
        
        *folds = realloc(*folds, ++(*n_folds) * sizeof(fold_t));
        (*folds)[(*n_folds) - 1].axis = str[i];
        (*folds)[(*n_folds) - 1].pos = atoi(str + i + 2);
    }

    fclose(fp);
}
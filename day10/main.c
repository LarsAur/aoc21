#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>

#define MAX_STR_LEN 120

char pairs[256] = {0};
typedef struct
{
    long *arr;
    int size;
    int _alloc_size;
} list_t;

typedef struct
{
    char arr[MAX_STR_LEN];
    int top;
} stack_t;

double median(long *arr, int size);
long kthSmallest(long *arr, int l, int r, int k);
long partition(long *arr, int l, int r);
int find_first_corruption(char *str);
long complete(char *str);

int main()
{
    FILE *fp = fopen("./inputs.txt", "r");

    if (fp == NULL)
    {
        perror("Unable to open file");
        fclose(fp);
        exit(1);
    }

    memset(pairs, '\0', MAX_STR_LEN);
    pairs['('] = ')';
    pairs['['] = ']';
    pairs['<'] = '>';
    pairs['{'] = '}';

    list_t list = {
        .arr = malloc(sizeof(int)),
        .size = 0,
        ._alloc_size = 1,
    };

    int corrupt_points = 0;
    char str[MAX_STR_LEN];
    while (fgets(str, MAX_STR_LEN, fp) != NULL)
    {
        int c = find_first_corruption(str);
        corrupt_points += c;

        if(c == 0)
        {
            long score = complete(str);

            if (list.size == list._alloc_size)
            {
                list._alloc_size *= 2;
                list.arr = realloc(list.arr, list._alloc_size * sizeof(long));
            }

            list.arr[list.size] = score;
            list.size++;
        }
    }

    fclose(fp);
    free(list.arr);

    printf("Corrupt Points: %d\n", corrupt_points);
    printf("Complete Points: %ld\n", (long)median(list.arr, list.size));

    return 0;
}

long complete(char *str)
{
    stack_t stack =
    {
        .arr = {0},
        .top = 0,
    };

    while (str[0] != '\0')
    {
        if (pairs[(uint8_t)str[0]])
        {
            stack.arr[stack.top++] = str[0];
        }
        else
        {
            stack.top--;
        }
        str++;
    }

    long score = 0;
    for (int i = stack.top; i >= 0; i--)
    {
        score = score * 5;
        switch (stack.arr[i])
        {
        case '(': score = score + 1; break;
        case '[': score = score + 2; break;
        case '{': score = score + 3; break;
        case '<': score = score + 4; break;
        }
    }

    return score;
}

int find_first_corruption(char *str)
{
    stack_t stack =
    {
        .arr = {0},
        .top = 0,
    };

    while (str[0] != '\0')
    {
        if (pairs[(uint8_t)str[0]])
        {
            stack.arr[stack.top++] = str[0];
        }
        else if(str[0] == pairs[(uint8_t)stack.arr[stack.top - 1]])
        {
            stack.top--;
        }
        else
        {
            break;
        }
        str++;
    }

    switch (str[0])
    {
        case ')': return 3;
        case ']': return 57;
        case '}': return 1197;
        case '>': return 25137;
        default: return 0;
    }
}

double median(long *arr, int size)
{
    if (size % 2 == 0)
    {
        int k1 = size / 2 + 1;
        int k2 = size / 2;
        int m1 = kthSmallest(arr, 0, size - 1, k1);
        int m2 = kthSmallest(arr, 0, size - 1, k2);
        return (m1 + m2) / (double)2;
    }
    else
    {
        return (double)kthSmallest(arr, 0, size - 1, size / 2 + 1);
    }
}

long kthSmallest(long *arr, int l, int r, int k)
{
    if (k <= 0 || k > r - l + 1)
        return INT_MAX;

    int index = partition(arr, l, r);
    if (index - l == k - 1)
        return arr[index];
    if (index - l > k - 1)
        return kthSmallest(arr, l, index - 1, k);
    return kthSmallest(arr, index + 1, r, k - index + l - 1);
}

long partition(long *arr, int l, int r)
{
    long pivot = arr[r];
    int i = l;

    for (int j = l; j <= r - 1; j++)
    {
        if (arr[j] <= pivot)
        {
            long tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
        }
    }

    long tmp = arr[i];
    arr[i] = arr[r];
    arr[r] = tmp;

    return i;
}
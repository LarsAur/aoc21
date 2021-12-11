#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

typedef struct
{
    int *arr;
    int size;
    int _alloc_size;
} list_t;

float average(int *arr, int size);
float median(int *arr, int size);
int kthSmallest(int *arr, int l, int r, int k);
int partition(int *arr, int l, int r);

int main()
{
    FILE *fp = fopen("./inputs.txt", "r");
    if (fp == NULL)
    {
        perror("Unable to open file");
        fclose(fp);
        exit(1);
    }

    list_t list = {
        .arr = malloc(sizeof(int)),
        .size = 0,
        ._alloc_size = 1,
    };

    int input;
    while(fscanf(fp, "%d,", &input) != EOF)
    {
        if(list.size == list._alloc_size)
        {
            list._alloc_size *= 2;
            list.arr = realloc(list.arr, list._alloc_size * sizeof(int));
        }

        list.arr[list.size] = input;
        list.size++;
    }

    fclose(fp);

    printf("List size: %d\n", list.size);
    printf("Alloc size: %d\n", list._alloc_size);

    int m = (int) median(list.arr, list.size);
    int fuel = 0;
    for(int i = 0; i < list.size; i++)
    {
        fuel += abs(m - list.arr[i]);
    }

    printf("A fuel: %d\n", fuel);

    int a = (int) average(list.arr, list.size);
    fuel = 0;
    for(int i = 0; i < list.size; i++)
    {
        int diff = abs(a - list.arr[i]);
        fuel += (diff + 1) * diff / 2; // triangle sum
    }

    printf("B fuel: %d\n", fuel);

    free(list.arr);

    return 0;
}

float average(int *arr, int size)
{
    int sum = 0;
    for(int i = 0; i < size; i++)
    {
        sum += arr[i];    
    }

    return sum / (float) size;
}

float median(int *arr, int size)
{
    if(size % 2 == 0)
    {
        int k1 = size / 2 + 1;
        int k2 = size / 2;
        int m1 = kthSmallest(arr, 0, size-1, k1);
        int m2 = kthSmallest(arr, 0, size-1, k2);
        return (m1 + m2) / (float) 2;
    }
    else
    {
        return (float) kthSmallest(arr, 0, size-1, size / 2 + 1);
    }
}

int kthSmallest(int *arr, int l, int r, int k)
{
    if(k <= 0 || k > r - l + 1) return INT_MAX;

    int index = partition(arr, l, r);
    if(index - l == k - 1) return arr[index];
    if(index - l > k - 1) return kthSmallest(arr, l, index - 1, k);
    return kthSmallest(arr, index + 1, r, k - index + l - 1);
}

int partition(int *arr, int l, int r)
{
    int pivot = arr[r];
    int i = l;

    for(int j = l; j <= r - 1; j++)
    {
        if(arr[j] <= pivot)
        {
            int tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
        }
    }

    int tmp = arr[i];
    arr[i] = arr[r];
    arr[r] = tmp;

    return i;
}

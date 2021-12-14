#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct rule_t
{
    long count;
    char left;
    char right;
    char insert;
} rule_t;

typedef struct rule_list_t
{
    rule_t *arr;
    int size;
    int _alloc_size;
} rule_list_t;

void parseInput(rule_list_t *rules, long char_count[256]);
void step(rule_list_t *rules, long char_count[256]);
long calc_result(long char_count[256]);

int main()
{
    rule_list_t rules = 
    {
        .arr = malloc(sizeof(rule_t)),
        .size = 0,
        ._alloc_size = 1,
    };

    long char_count[256] = {0};
    parseInput(&rules, char_count);

    for(int i = 0; i < 10; i++)
    {
        step(&rules, char_count);
    }
    printf("Max - min = %ld\n", calc_result(char_count));
    
    for(int i = 0; i < 30; i++)
    {
        step(&rules, char_count);
    }
    printf("Max - min = %ld\n", calc_result(char_count));

    free(rules.arr);

    return 0;
}

long calc_result(long char_count[256])
{
    long max = 0;
    long min = LONG_MAX;
    for(int i = 0; i < 256; i++)
    {
        max = max < char_count[i] ? char_count[i] : max;
        min = min > char_count[i] && char_count[i] > 0 ? char_count[i] : min; 
    }

    printf("Max: %ld, Min: %ld\n", max, min);

    return max - min;
}

void step(rule_list_t *rules, long char_count[256])
{
    long counts[rules->size];
    for(int i = 0; i < rules->size; i++)
    {
        counts[i] = rules->arr[i].count;
    }

    for(int i = 0; i < rules->size; i++)
    {
        char_count[(u_int8_t)rules->arr[i].insert] += counts[i];
        // Find the two potential new rules
        for(int j = 0; j < rules->size; j++)
        {
            // Matching left
            if(rules->arr[j].left == rules->arr[i].left && rules->arr[j].right == rules->arr[i].insert)
            {
                rules->arr[j].count += counts[i];
            }

            // Matching right
            if(rules->arr[j].left == rules->arr[i].insert && rules->arr[j].right == rules->arr[i].right)
            {
                rules->arr[j].count += counts[i];
            }
        }

        rules->arr[i].count -= counts[i];
    }
}


void parseInput(rule_list_t *rules, long char_count[256]){
    FILE* fp = fopen("./inputs.txt", "r");
    if(fp == NULL)
    {
        perror("Unable to open file");
        fclose(fp);
        exit(1);
    }

    char str[100];
    if(fscanf(fp, "%s", str) == EOF) exit(1);
    fseek(fp, 2, SEEK_CUR);
    
    char left;
    char right;
    char insert;
    while (fscanf(fp, "%c%c %*c%*c %c%*c", &left, &right, &insert) != EOF)
    {
        if(rules->size == rules->_alloc_size)
        {
            rules->_alloc_size *= 2;
            rules->arr = realloc(rules->arr, rules->_alloc_size * sizeof(rule_t));
        }

        rules->arr[rules->size].left = left;
        rules->arr[rules->size].right = right;
        rules->arr[rules->size].insert = insert;
        rules->arr[rules->size].count = 0;
        rules->size++;
    }

    int len = strlen(str);
    for(int i = 0; i < len - 1; i++)
    {
        char_count[(__uint8_t)str[i]]++;
        for(int j = 0; j < rules->size; j++)
        {
            if(rules->arr[j].left == str[i] && rules->arr[j].right == str[i+1])
            {
                rules->arr[j].count++;
            }
        }
    }
    char_count[(__uint8_t)str[len-1]]++;


    fclose(fp);
}
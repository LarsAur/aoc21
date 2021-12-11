#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// All inputs are 12 bits
#define BIT_COUNT 12
#define LINE_COUNT 1000
#define OFFSET(line, bit) (line * (BIT_COUNT + 1) + bit)

void binary_diagnostic_a();
void binary_diagnostic_b();
int filter_bit_criteria();
int oxygen_bit_criteria(char bit, int one_counts, int num_lines);
int co2_bit_criteria(char bit, int one_counts, int num_lines);

int main()
{
    binary_diagnostic_a();
    binary_diagnostic_b();
    return 0;
}

int oxygen_bit_criteria(char bit, int one_counts, int num_lines)
{
    return (bit == '1') == (one_counts * 2 >= num_lines);
}

int co2_bit_criteria(char bit, int one_counts, int num_lines)
{
    return (bit == '1') == (one_counts * 2 < num_lines);
}

void binary_diagnostic_b()
{
    int oxygen_rating = filter_bit_criteria(oxygen_bit_criteria);
    int co2_rating = filter_bit_criteria(co2_bit_criteria);

    printf("co2 = %d\n", co2_rating);
    printf("O = %d\n", oxygen_rating);
    printf("oxygen * co2 = %d\n", oxygen_rating * co2_rating);
}

int filter_bit_criteria(int (*criteria)(char, int, int))
{
    FILE* fp = fopen("./inputs.txt", "r");

    if(fp == NULL)
    {
        perror("Unable to open file");
        fclose(fp);
        exit(1);
    }

    char bin_str1[LINE_COUNT * (BIT_COUNT + 1)] = {0};
    char bin_str2[LINE_COUNT * (BIT_COUNT + 1)] = {0};

    char *rbin_str = (char *) bin_str1;
    char *wbin_str = (char *) bin_str2;

    int input_size = 0;
    while(fscanf(fp, "%s", &rbin_str[OFFSET(input_size, 0)]) != EOF)
    {
        input_size++;
    }

    int bit_index = 0;
    while (bit_index < BIT_COUNT && rbin_str[OFFSET(1, 0)] != '\0')
    {    
        int rline = 0;
        int wline = 0;
        int one_counts = 0;

        // Count the bit values
        while(rbin_str[OFFSET(rline, 0)] != '\0' && rline < LINE_COUNT)
        {
            one_counts += rbin_str[OFFSET(rline, bit_index)] == '1';
            rline++;
        }

        // Only include the values corresponding to the bit-criteria
        int num_lines = rline;
        rline = 0;
        wline = 0;
        while(rbin_str[OFFSET(rline, 0)] != '\0' && rline < num_lines)
        {
            if(criteria(rbin_str[OFFSET(rline, bit_index)], one_counts, num_lines))
            {
                memcpy(&wbin_str[OFFSET(wline, 0)], &rbin_str[OFFSET(rline, 0)], sizeof(char) * BIT_COUNT);
                wline++;
            }
            rline++;
        }
        wbin_str[OFFSET(wline, 0)] = '\0';

        // Swap pointers
        char *tmp = rbin_str;
        rbin_str = wbin_str;
        wbin_str = tmp;

        bit_index++;
    }

    // Convert the binary string to int
    int value = 0;
    for(int i = 0; i < BIT_COUNT; i++)
    {
        value = (value << 1) | (rbin_str[OFFSET(0, i)] == '1');
    }

    fclose(fp);

    return value;
}

void binary_diagnostic_a()
{
    FILE* fp = fopen("./inputs.txt", "r");
    if(fp == NULL)
    {
        perror("Unable to open file");
        fclose(fp);
        exit(1);
    }

    char bin_str[BIT_COUNT + 1] = {0};
    int one_counts[BIT_COUNT] = {0};
    int input_size = 0;

    while(fscanf(fp, "%s", bin_str) != EOF){
        for(int i = 0; i < BIT_COUNT; i++)
        {
            if(bin_str[i] == '1')
            {
                one_counts[i] += 1;
            }
        }
        input_size++;
    }

    fclose(fp);

    uint32_t gamma_rate = 0;
    for(int i = 0; i < BIT_COUNT; i++)
    {
        gamma_rate = (gamma_rate << 1) | (one_counts[i] >= (input_size >> 1));
    }

    uint32_t epsilon_rate = (~gamma_rate) & ((1 << BIT_COUNT) - 1);
    printf("gamma = %d\n", gamma_rate);
    printf("epsilon = %d\n", epsilon_rate);
    printf("gamma * epsilon = %d\n", gamma_rate * epsilon_rate);
}
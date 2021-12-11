#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define ONE_INDEX 0
#define FOUR_INDEX 1
#define SEVEN_INDEX 2
#define EIGHT_INDEX 3

int num_matching_segment(char *segments1, char *segments2);
void simple_counting();
void full_decode();

int main()
{
    simple_counting();
    full_decode();
}

void full_decode()
{
    FILE *fp = fopen("./inputs.txt", "r");
    if (fp == NULL)
    {
        perror("Unable to open file");
        fclose(fp);
        exit(1);
    }

    char segments[14][8];
    char known_patterns[4][8];
    int digits[4];
    int sum = 0;
    while (
        fscanf(fp, "%s %s %s %s %s %s %s %s %s %s | %s %s %s %s",
        (char *)&segments[0],
        (char *)&segments[1],
        (char *)&segments[2],
        (char *)&segments[3],
        (char *)&segments[4],
        (char *)&segments[5],
        (char *)&segments[6],
        (char *)&segments[7],
        (char *)&segments[8],
        (char *)&segments[9],
        (char *)&segments[10],
        (char *)&segments[11],
        (char *)&segments[12],
        (char *)&segments[13]
        ) != EOF)
    {

        // Find the digits with a unique number of segments enabled
        // These patterns can be used to match with the other patterns to determine what digits thay are.
        for(int i = 0; i < 10; i++)
        {
            if(strlen(segments[i]) == 2) strcpy(known_patterns[ONE_INDEX], segments[i]); // 1
            else if(strlen(segments[i]) == 4) strcpy(known_patterns[FOUR_INDEX], segments[i]); // 4
            else if(strlen(segments[i]) == 3) strcpy(known_patterns[SEVEN_INDEX], segments[i]); // 7
            else if(strlen(segments[i]) == 7) strcpy(known_patterns[EIGHT_INDEX], segments[i]); // 8
        }
        
        // Go through the input digits and determine what number thay are, based on the number of segments enabled and how many segements are matching with a known digit.
        for(int i = 0; i < 4; i++)
        {
            int len = strlen(segments[i + 10]);
            if(len == 2) digits[i] = 1; // 1
            else if(len == 4) digits[i] = 4; // 4
            else if(len == 3) digits[i] = 7; // 7
            else if(len == 7) digits[i] = 8; // 8
            else if(len == 6) // 6, 9, 0 
            {
                if(num_matching_segment(segments[i + 10], known_patterns[ONE_INDEX]) == 2)
                {
                    if(num_matching_segment(segments[i + 10], known_patterns[FOUR_INDEX]) == 4) digits[i] = 9;
                    else digits[i] = 0;
                }
                else digits[i] = 6;
            }
            else if(len == 5) // 2, 5, 3
            {
                if(num_matching_segment(segments[i + 10], known_patterns[ONE_INDEX]) == 2) digits[i] = 3;
                else
                {
                    if(num_matching_segment(segments[i + 10], known_patterns[FOUR_INDEX]) == 3) digits[i] = 5;
                    else digits[i] = 2;
                }
            }
        }

        // Calculate the number from the digits
        int number = 0;
        for(int i = 0; i < 4; i++)
        {
            number += digits[i] * (int) pow(10, 3 - i);
        }
        sum += number;
    }

    fclose(fp);

    printf("Sum = %d\n", sum);
}

int num_matching_segment(char *segments1, char *segments2)
{
    int len1 = strlen(segments1);
    int len2 = strlen(segments2);
    int matches = 0;

    for(int i = 0; i < len1; i++)
    {
        for(int j = 0; j < len2; j++)
        {
            if(segments1[i] == segments2[j])
            {
                matches++;
            }
        }
    }

    return matches;
}

void simple_counting()
{
    FILE *fp = fopen("./inputs.txt", "r");
    if (fp == NULL)
    {
        perror("Unable to open file");
        fclose(fp);
        exit(1);
    }

    /*
        When counting the number of 1, 4, 7 and 8s we can count the number of digits using their respective number
        of segments, as these numbers use a unique number of segments
    */

    char segments[4][8];
    int matching_digits = 0;
    while (fscanf(fp, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s | %s %s %s %s", (char *)&segments[0], (char *)&segments[1], (char *)&segments[2], (char *)&segments[3]) != EOF)
    {
        for(int i = 0; i < 4; i++)
        {
            if(strlen(segments[i]) == 2) matching_digits++; // 1
            if(strlen(segments[i]) == 4) matching_digits++; // 4
            if(strlen(segments[i]) == 3) matching_digits++; // 7
            if(strlen(segments[i]) == 7) matching_digits++; // 8
        }
    }

    fclose(fp);

    printf("Matching digits = %d\n", matching_digits);
}
#include <stdio.h>
#include <stdlib.h>

#define N_BOARDS 100
#define N_NUMBERS 100
#define BOARD_SIZE 5

int unmarked_sum(int board[BOARD_SIZE][BOARD_SIZE]);

void win_first();
void win_last();

int main()
{
    printf("Win first\n");
    win_first();
    printf("Win last\n");
    win_last();

    return 0;
}

void win_last()
{

    FILE* fp = fopen("./inputs.txt", "r");

    if(fp == NULL)
    {
        perror("Unable to open file");
        exit(1);
    }

    int num_boards_won = 0;
    int won_boards[N_BOARDS];
    int drawn_numbers[N_NUMBERS];
    int boards[N_BOARDS][BOARD_SIZE][BOARD_SIZE];

    // Fetch drawn numbers
    for(int i = 0; i < N_NUMBERS; i++)
    {
        if(fscanf(fp, "%d,", &drawn_numbers[i]) == EOF)
        {
            printf("Input data incorrect");
            exit(1);
        }
    }

    // Fetch bingo boards
    for(int i = 0; i < N_BOARDS; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
        {
            for(int k = 0; k < BOARD_SIZE; k++)
            {
                if(fscanf(fp, "%d,", &boards[i][j][k]) == EOF)
                {
                    printf("Input data incorrect");
                    exit(1);
                }
            }
        }
        won_boards[i] = 0;
    }

    // Loop through drawn numbers and mark them off in the boards
    for(int i = 0; i < N_NUMBERS; i++)
    {
        for(int j = 0; j < N_BOARDS; j++)
        {
            for(int k = 0; k < BOARD_SIZE; k++)
            {
                for(int l = 0; l < BOARD_SIZE; l++)
                {
                    if(boards[j][k][l] == drawn_numbers[i])
                    {
                        boards[j][k][l] = -1;
                        break;
                    }
                }
            }
        }

        // Check if the board is finished
        for(int j = 0; j < N_BOARDS; j++)
        {
            if(!won_boards[j])
            {
                for(int k = 0; k < BOARD_SIZE; k++)
                {
                    int prod_x = 1;
                    int prod_y = 1;
                    for(int l = 0; l < BOARD_SIZE; l++)
                    {
                        prod_x *= boards[j][k][l];
                        prod_y *= boards[j][l][k];
                    }

                    if((prod_x == -1 || prod_y == -1) && !won_boards[j])
                    {
                        int sum = unmarked_sum(boards[j]);
                        int called_num = drawn_numbers[i];
                        won_boards[j] = 1;
                        num_boards_won += 1;

                        if(num_boards_won == N_BOARDS)
                        {
                            printf("Result: %d\n", sum * called_num);
                            return;
                        }
                    }
                }
            }
        }
    }
}

void win_first()
{
    FILE* fp = fopen("./inputs.txt", "r");

    if(fp == NULL)
    {
        perror("Unable to open file");
        exit(1);
    }

    int drawn_numbers[N_NUMBERS];
    int boards[N_BOARDS][BOARD_SIZE][BOARD_SIZE];

    // Fetch drawn numbers
    for(int i = 0; i < N_NUMBERS; i++)
    {
        if(fscanf(fp, "%d,", &drawn_numbers[i]) == EOF)
        {
            printf("Input data incorrect");
            exit(1);
        }
    }

    // Fetch bingo boards
    for(int i = 0; i < N_BOARDS; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
        {
            for(int k = 0; k < BOARD_SIZE; k++)
            {
                if(fscanf(fp, "%d,", &boards[i][j][k]) == EOF)
                {
                    printf("Input data incorrect");
                    exit(1);
                }
            }
        }    
    }

    // Loop through drawn numbers and mark them off in the boards
    for(int i = 0; i < N_NUMBERS; i++)
    {
        for(int j = 0; j < N_BOARDS; j++)
        {
            for(int k = 0; k < BOARD_SIZE; k++)
            {
                for(int l = 0; l < BOARD_SIZE; l++)
                {
                    if(boards[j][k][l] == drawn_numbers[i])
                    {
                        boards[j][k][l] = -1;
                        break;
                    }
                }
            }
        }

        // Check if the board is finished
        for(int j = 0; j < N_BOARDS; j++)
        {
            for(int k = 0; k < BOARD_SIZE; k++)
            {
                int prod_x = 1;
                int prod_y = 1;
                for(int l = 0; l < BOARD_SIZE; l++)
                {
                    prod_x *= boards[j][k][l];
                    prod_y *= boards[j][l][k];
                }

                if(prod_x == -1 || prod_y == -1)
                {
                    int sum = unmarked_sum(boards[j]);
                    int called_num = drawn_numbers[i];
                    printf("Result %d\n", sum * called_num);
                    return;
                }
            }
        }
    }
}

int unmarked_sum(int board[BOARD_SIZE][BOARD_SIZE])
{
    int sum = 0;
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
        {
            if(board[i][j] != -1) sum += board[i][j];
        }   
    }

    return sum;
}

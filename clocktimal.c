#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "clocktimal.h"

#include <string.h>
#include <ctype.h>



int is_valid_number(const char *str) {
    if (*str == '-' || *str == '+') str++;  // Allow signed numbers
    if (!*str) return 0;  // Empty string is invalid
    while (*str) {
        if (!isdigit(*str)) return 0;  // Reject non-digit characters
        str++;
    }
    return 1;
}


int get_scramble(int *scramble, int size) {
    char buffer[BUFFER_SIZE];
    int count = 0;

    printf("Enter scramble: ");
    
    if (!fgets(buffer, BUFFER_SIZE, stdin)) {
        printf("Error reading input try again\n");
        return 1;
    }

    char *token = strtok(buffer, " \t\n");

    while (token && count < size) {
        if (is_valid_number(token)) {
            scramble[count] = ((strtol(token, NULL, 10) % 12) + 12) % 12;
        } else {
            printf("Invalid input\n");
            return 1;
        }
        count++;
        token = strtok(NULL, " \t\n");
    }

    if (count < size) {
        printf("Please input 14 numbers\n");
        return 1;
    }

    return 0;

}


int main() {
    // Data arrays
    int *unique_rows;
    int n_unique_rows;
    int *pinsets;
    int n_pinsets;
    int *pinset_mappings;
    int n_pinset_mappings;
    int *moves;
    SOLUTION_T *solution_info;

    solution_info = (SOLUTION_T *)malloc(sizeof(SOLUTION_T));
    if (solution_info == NULL) {
        perror("solution memory allocation failed");
        exit(1);
    }

    read_data(&unique_rows, &n_unique_rows, &pinsets, &n_pinsets, &pinset_mappings, &n_pinset_mappings);
    

    for (int i = 0; i < 5; i++) {
        int scramble[PINSET_LENGTH];
        // int scramble[PINSET_LENGTH] = {11, 6, 6, 6, 10, 9, 11, 11, 3, 10, 2, 2, 4, 7};
    
        if (get_scramble(scramble, PINSET_LENGTH)) {
            continue;
        }

        calculate_all_moves(scramble, &moves, unique_rows, n_unique_rows);

        find_all_optimal(scramble, solution_info, moves, n_unique_rows, pinsets, n_pinsets, pinset_mappings);

        print_solutions(solution_info, moves, pinsets, pinset_mappings);

    }

    // free memory
    free(solution_info);
    free(moves);
    free(unique_rows);
    free(pinsets);
    free(pinset_mappings);

}


void calculate_all_moves(int *scramble, int **moves, int *unique_rows, int n_unique_rows) {
    int i, j, c;

    // allocate new array for all moves
    *moves = (int *)malloc(n_unique_rows * sizeof(int));
    if (moves == NULL) {
        perror("error allocating moves array");
        exit(1);
    }

    // calculate all unique moves
    for (i = 0; i < n_unique_rows; i += 1) {
        c = 0;
        for (j = 0; j < PINSET_LENGTH; j++) {
            c += unique_rows[(i * PINSET_LENGTH) + j] * scramble[j];
        }

        c %= 12;
        (*moves)[i] = c < 0 ? c + 12 : c;
    }
}

void find_all_optimal(int *scramble, SOLUTION_T *solution_info, int *moves, int n_unique_rows, int *pinsets, int n_pinsets, int *pinset_mappings) {


    // Variables for optimal solving
    int i, j;
    int min_moves = __INT_MAX__;
    int move_pinset;
    int movecount;
    int move;

    int min_ticks = __INT_MAX__;
    int tick_pinset;
    int tickcount;

    int tick_counts[12] = {0,1,2,3,4,5,6,5,4,3,2,1};

    for (i = 0; i < n_pinsets; i++) {
        movecount = 0;
        tickcount = 0;

        for (j = 0; j < PINSET_LENGTH; j++) {
            move = moves[ pinset_mappings[(i * PINSET_LENGTH) + j] ];

            if (move != 0) movecount++;
            tickcount += tick_counts[move];
        }


        if (movecount < min_moves) {
            min_moves = movecount;
            move_pinset = i;
        }

        if (tickcount < min_ticks) {
            min_ticks = tickcount;
            tick_pinset = i;
        }

    }

    solution_info->optmoves = min_moves;
    solution_info->move_pinset = move_pinset;

    solution_info->optticks = min_ticks;
    solution_info->tick_pinset = tick_pinset;

}
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "utils.h"
#include "clocktimal.h"

#include <string.h>
#include <ctype.h>


int main() {
    DATA_T *program_data;

    program_data = (DATA_T *)malloc(sizeof(DATA_T));
    if (program_data == NULL) {
        perror("Program data allocation failed");
        exit(1);
    }

    read_data(program_data);

    // // int scramble[PINSET_LENGTH];
    // int scramble[PINSET_LENGTH] = {11, 6, 6, 6, 10, 9, 11, 11, 3, 10, 2, 2, 4, 7};


    // calculate_all_moves(scramble, program_data);

    // find_all_optimal(scramble, program_data);

    // print_solutions(program_data);



    while(1) {
        int scramble[PINSET_LENGTH];
        // int scramble[PINSET_LENGTH] = {11, 6, 6, 6, 10, 9, 11, 11, 3, 10, 2, 2, 4, 7};
    
        if (get_scramble(scramble, PINSET_LENGTH)) {
            continue;
        }

        calculate_all_moves(scramble, program_data);

        find_all_optimal(scramble, program_data);

        print_solutions(program_data);

    }

    // free memory
    cleanup(program_data);
}


// void calculate_all_moves(int *scramble, int **moves, int *unique_rows, int n_unique_rows) {
void calculate_all_moves(int *scramble, DATA_T *program_data) {
    int i, j, c;

    // allocate new array for all moves
    program_data->moves = (int *)malloc(program_data->n_unique_rows * sizeof(int));
    if (program_data->moves == NULL) {
        perror("error allocating moves array");
        exit(1);
    }

    // calculate all unique moves
    for (i = 0; i < program_data->n_unique_rows; i += 1) {
        c = 0;
        for (j = 0; j < PINSET_LENGTH; j++) {
            c += (program_data->unique_rows)[(i * PINSET_LENGTH) + j] * scramble[j];
        }

        c %= 12;
        (program_data->moves)[i] = c < 0 ? c + 12 : c;
    }
}

// void find_all_optimal(int *scramble, SOLUTION_T *solution_info, int *moves, int n_unique_rows, int *pinsets, int n_pinsets, int *pinset_mappings) {
void find_all_optimal(int *scramble, DATA_T *program_data) {


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

    for (i = 0; i < program_data->n_pinsets; i++) {
        movecount = 0;
        tickcount = 0;

        for (j = 0; j < PINSET_LENGTH; j++) {
            move = (program_data->moves)[ (program_data->pinset_mappings)[(i * PINSET_LENGTH) + j] ];

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

    (program_data->solution_info)->optmoves = min_moves;
    (program_data->solution_info)->move_pinset = move_pinset;

    (program_data->solution_info)->optticks = min_ticks;
    (program_data->solution_info)->tick_pinset = tick_pinset;

}
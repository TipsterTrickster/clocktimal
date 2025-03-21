#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "solvers.h"


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
    int move, lastmove;
    int *pinset;


    /* Optimal Movecount */
    int min_moves = __INT_MAX__; // lowest movecount
    int min_moves_tickcount = __INT_MAX__; // lowest ticks for lowest movecount
    int move_pinset;
    int movecount;
    

    /* Optimal Tickcount */
    int min_ticks = __INT_MAX__; // Lowest tick count
    int min_ticks_movecount = __INT_MAX__; // Lowest movecount for the lowest tick count 
    int tick_pinset;
    int tickcount;

    /* Simul Optimal */
    int min_simul = __INT_MAX__;
    int simul_pinset;
    int simulcount;
    int simul_state = NOT_SIMUL;

    int tick_counts[12] = {0,1,2,3,4,5,6,5,4,3,2,1}; // accounts for stuff like the fact that 11 is actually only 1 tick
    for (i = 0; i < program_data->n_pinsets; i++) {
        movecount = 0;
        tickcount = 0;
        simulcount = 0;
        simul_state = NOT_SIMUL;

        pinset = &((program_data->pinsets)[i * PINSET_LENGTH]);

        for (j = 0; j < PINSET_LENGTH; j++) {
            move = (program_data->moves)[ (program_data->pinset_mappings)[(i * PINSET_LENGTH) + j] ];
            // move = (program_data->moves)[ pinset[j] ];

            if (move != 0) movecount++; // COUNT MOVES
            tickcount += tick_counts[move]; // COUNT TICKS

            if (j > 0) {
                if (pinset[j] % 2 == 1 && (pinset[j - 1] == pinset[j] - 1) && pinset[j] < 28) { // Last move is this moves compliment
                    if (lastmove == 0) simulcount++;
                } else {
                    if (move != 0) simulcount++;
                }
            } else {
                if (move != 0) simulcount++;
            }

            lastmove = move;

            /* SIMUL MOVE COUNTING, This one is slightly faster for some reason */
            // if ((j < PINSET_LENGTH - 1) && (pinset[j] < 28) && (simul_state == NOT_SIMUL)) { // Check move is not the last, and is neither ALL moves, these can't be simul
            //     if ((pinset[j] % 2 == 0) && (pinset[j+1] == pinset[j] + 1)) { // Checks if current move is a front move, and the next is the complement

            //         if (move != 0) { // if move isn't skipped increase simulcount
            //             simulcount++;
            //             simul_state = NEXT_SIMUL; // indicate that the nextmove is skipped
            //         } else  {
            //             simul_state = POSSIBLE_SKIP; // indicate possible full simul_move skip
            //         }

            //     } else {
            //         simulcount++;
            //     }
            // } else if (simul_state == POSSIBLE_SKIP) {
            //     if (move != 0) simulcount++;
            //     simul_state = NOT_SIMUL;
            // } else if (simul_state == NEXT_SIMUL) {
            //     simul_state = NOT_SIMUL;
            // } else {
            //     simulcount++;
            // }


        }

        // Update optimal movecount, and minimize ticks
        if ((movecount < min_moves) || ((movecount == min_moves) && (tickcount < min_moves_tickcount))) {
            min_moves = movecount;
            move_pinset = i;
        }

        // Update optimal tickcount, and minimize moves
        if ((tickcount < min_ticks) || ((tickcount == min_ticks) && (movecount < min_ticks_movecount))) {
            min_ticks = tickcount;
            tick_pinset = i;
        }

        // Update optimal simulcount
        if (simulcount < min_simul) {
            min_simul = simulcount;
            simul_pinset = i;
        }

    }

    (program_data->solution_info)->optmoves = min_moves;
    (program_data->solution_info)->move_pinset = move_pinset;

    (program_data->solution_info)->optticks = min_ticks;
    (program_data->solution_info)->tick_pinset = tick_pinset;

    (program_data->solution_info)->optsimul = min_simul;
    (program_data->solution_info)->simul_pinset = simul_pinset;

}
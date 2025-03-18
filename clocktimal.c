#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "clocktimal.h"


void find_all_optimal(int *scramble, int *unique_rows, int n_unique_rows, int *pinsets, int n_pinsets, int *pinset_mappings);

int main() {
    // Data arrays
    int *unique_rows;
    int n_unique_rows;
    int *pinsets;
    int n_pinsets;
    int *pinset_mappings;
    int n_pinset_mappings;

    // int scramble[PINSET_LENGTH];
    int scramble[PINSET_LENGTH] = {11, 6, 6, 6, 10, 9, 11, 11, 3, 10, 2, 2, 4, 7};

    read_data(&unique_rows, &n_unique_rows, &pinsets, &n_pinsets, &pinset_mappings, &n_pinset_mappings);
    

    find_all_optimal(scramble, unique_rows, n_unique_rows, pinsets, n_pinsets, pinset_mappings);


    free(unique_rows);
    free(pinsets);
    free(pinset_mappings);

}

void find_all_optimal(int *scramble, int *unique_rows, int n_unique_rows, int *pinsets, int n_pinsets, int *pinset_mappings) {
    int moves[n_unique_rows];
    int i, j, c;
    

    // loop calculate all unique moves
    for (i = 0; i < n_unique_rows; i += 1) {
        c = 0;
        for (j = 0; j < PINSET_LENGTH; j++) {
            c += unique_rows[(i * PINSET_LENGTH) + j] * scramble[j];
        }

        c %= 12;
        moves[i] = c < 0 ? c + 12 : c;
    }

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

    printf("Optimal movecount: %d\n", min_moves);
    printf("Optimal pinset: ");
    for (i = 0; i < PINSET_LENGTH; i++) {
        printf("%d ", pinsets[ (move_pinset * PINSET_LENGTH) + i ]);
    }
    printf("\n");

    printf("Optimal tickcount: %d\n", min_ticks);
    printf("Optimal pinset: ");
    for (i = 0; i < PINSET_LENGTH; i++) {
        printf("%d ", pinsets[ (tick_pinset * PINSET_LENGTH) + i ]);
    }
    printf("\n");


}
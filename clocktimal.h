#ifndef CLOCKTIMAL_H
#define CLOCKTIMAL_H

#include "utils.h"

#define PINSET_LENGTH 14
#define BUFFER_SIZE 256

struct solutioninfo {
    int optmoves;
    int move_pinset;

    int optticks;
    int tick_pinset;

    int simulmoves;
    int simul_pinset;

    int simticks;
    int simtick_pinset;

} typedef SOLUTION_T;

// Forward declaration of DATA_T
typedef struct pinset_data DATA_T;

// Function prototypes

void calculate_all_moves(int *scramble, DATA_T *program_data);


void find_all_optimal(int *scramble, DATA_T *program_data);




#endif
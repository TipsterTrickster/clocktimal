#ifndef SOLVERS_H
#define SOLVERS_H

#include "utils.h"

#define PINSET_LENGTH 14
#define BUFFER_SIZE 256

#define NOT_SIMUL 0
#define NEXT_SIMUL 1
#define POSSIBLE_SKIP 2


struct solutioninfo {
    int optmoves;
    int move_pinset;

    int optticks;
    int tick_pinset;

    int optsimul;
    int simul_pinset;

    int optsimticks;
    int simtick_pinset;

} typedef SOLUTION_T;

// Forward declaration of DATA_T
typedef struct pinset_data DATA_T;

// Function prototypes

void calculate_all_moves(int *scramble, DATA_T *program_data);


void find_all_optimal(int *scramble, DATA_T *program_data);




#endif
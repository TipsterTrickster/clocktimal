#ifndef SOLVERS_H
#define SOLVERS_H

#include "utils.h"

#define PINSET_LENGTH 14
#define BUFFER_SIZE 256

#define NOT_SIMUL 0
#define NEXT_SIMUL 1
#define POSSIBLE_SKIP 2

// Forward declaration of DATA_T
typedef struct pinset_data DATA_T;

struct thread_args {
    // input args
    int thread_num;
    DATA_T *program_data;
    int *scramble;
    int move_start;
    int move_end;
    int pinset_start;
    int pinset_end;

    // return values
    int optmoves;
    int move_pinset;
    int optticks;
    int tick_pinset;
    int optsimul;
    int simul_pinset;
    int optsimticks;
    int simtick_pinset;
};

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



// Function prototypes
void calculate_all_moves(int *scramble, DATA_T *program_data);
void *calculate_all_moves_p(void *args);

void find_all_optimal(int *scramble, DATA_T *program_data);
void *find_all_optimal_p(void *args);



#endif
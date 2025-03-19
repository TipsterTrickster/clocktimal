#ifndef CLOCKTIMAL_H
#define CLOCKTIMAL_H

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



void calculate_all_moves(int *scramble, int **moves, int *unique_rows, int n_unique_rows);
void find_all_optimal(int *scramble, SOLUTION_T *solution_info, int *moves, int n_unique_rows, int *pinsets, int n_pinsets, int *pinset_mappings);


#endif
#ifndef UTILS_H
#define UTILS_H

#include "clocktimal.h"

#define PINSET_LENGTH 14


void print_solutions(SOLUTION_T *solution_info, int *moves, int *pinsets, int *pinset_mappings);
void read_data(int **unique_rows, int *n_unique_rows, int **pinsets, int *n_pinsets, int **pinset_mappings, int *n_pinset_mappings);





#endif
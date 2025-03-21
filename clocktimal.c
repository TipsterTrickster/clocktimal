#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "utils.h"
#include "solvers.h"


int main() {
    DATA_T *program_data;

    program_data = (DATA_T *)malloc(sizeof(DATA_T));
    if (program_data == NULL) {
        perror("Program data allocation failed");
        exit(1);
    }

    read_data(program_data);

    while(1) {
        int scramble[PINSET_LENGTH];
    
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



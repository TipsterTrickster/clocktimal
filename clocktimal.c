#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

#include "utils.h"
#include "solvers.h"


int main() {

    // Multithreading
    int num_threads = 1;
	pthread_t *threads;
	struct thread_args *thread_data;
    int t, result;

    DATA_T *program_data;
    
    program_data = (DATA_T *)malloc(sizeof(DATA_T));
    if (program_data == NULL) {
        perror("Program data allocation failed");
        exit(1);
    }

    // load data from files
    read_data(program_data);

    // allocate new array for all moves
    program_data->moves = (int *)malloc(program_data->n_unique_rows * sizeof(int));
    if (program_data->moves == NULL) {
        perror("error allocating moves array");
        exit(1);
    }

    // allocate threads
	threads=calloc(num_threads, sizeof(pthread_t));
	if (threads==NULL) perror("allocating threads");

	// allocate thread data
	thread_data=calloc(num_threads,sizeof(struct thread_args));
	if (thread_data==NULL) perror("allocating thread_data");


    int size = program_data->n_unique_rows / num_threads;

    // General Thread data initialization
    for (t = 0; t < num_threads; t++) {
        thread_data[t].thread_num = t;
        thread_data[t].program_data = program_data;

        thread_data[t].move_start = t * size;
        
        if (t < num_threads - 1) {
            thread_data[t].move_end = (t + 1) * size;
        } else {
            thread_data[t].move_end = program_data->n_unique_rows;
        }
    }

    int scramble[PINSET_LENGTH] = {4, 7, 6, 6, 5, 4, 9, 9, 4, 10, 10, 7, 11, 4};

    for (t = 0; t < num_threads; t++) {
        thread_data[t].scramble = scramble;

        result = pthread_create(&threads[t], NULL, calculate_all_moves_p, (void *)&thread_data[t]);

        if (result) {
            perror("error creating thread\n");
            exit(1);
        }
    }

    for(t=0;t<num_threads;t++) {
		pthread_join(threads[t],NULL);
	}

    find_all_optimal(scramble, program_data);
    print_solutions(program_data);

    // while(1) {
    //     int scramble[PINSET_LENGTH];
    
    //     if (get_scramble(scramble, PINSET_LENGTH)) {
    //         continue;
    //     }

    //     calculate_all_moves(scramble, program_data);

    //     find_all_optimal(scramble, program_data);

    //     print_solutions(program_data);

    // }

    // free memory
    cleanup(program_data);
}



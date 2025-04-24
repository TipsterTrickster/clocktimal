#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

#include "utils.h"
#include "solvers.h"


int main(int argc, char *argv[]) {

    // Multithreading
    int num_threads = 1;
	pthread_t *threads;
	struct thread_args *thread_data;
    int t, result;

    DATA_T *program_data;
    
    int scramble[PINSET_LENGTH];

    if (argc == 2) {
        num_threads = atoi(argv[1]);
    }

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

    // chunk size for move and pinset arrays
    int move_size = program_data->n_unique_rows / num_threads;
    int pinset_size = program_data->n_pinsets / num_threads;

    // General Thread data initialization
    for (t = 0; t < num_threads; t++) {
        thread_data[t].thread_num = t;
        thread_data[t].program_data = program_data;

        thread_data[t].move_start = t * move_size;
        thread_data[t].pinset_start = t * pinset_size;
        
        if (t < num_threads - 1) {
            thread_data[t].move_end = (t + 1) * move_size;
            thread_data[t].pinset_end = (t + 1) * pinset_size;
        } else {
            thread_data[t].move_end = program_data->n_unique_rows;
            thread_data[t].pinset_end = program_data->n_pinsets;
        }
    }

    while(1) {

        if (get_scramble(scramble, PINSET_LENGTH)) {
            continue;
        }

        // parallel calculate all move
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



        // parallel find all optimal
        for (t = 0; t < num_threads; t++) {
            thread_data[t].scramble = scramble;

            result = pthread_create(&threads[t], NULL, find_all_optimal_p, (void *)&thread_data[t]);

            if (result) {
                perror("error creating thread\n");
                exit(1);
            }
        }


        (program_data->solution_info)->optmoves = __INT_MAX__;
        (program_data->solution_info)->optticks = __INT_MAX__;
        (program_data->solution_info)->optsimul = __INT_MAX__;
        (program_data->solution_info)->optsimticks = __INT_MAX__;

        for(t=0;t<num_threads;t++) {
            pthread_join(threads[t],NULL);

            if (thread_data[t].optmoves < (program_data->solution_info)->optmoves ||
                ((thread_data[t].optmoves == (program_data->solution_info)->optmoves) && (thread_data[t].optmove_tickcount < (program_data->solution_info)->optmove_tickcount))) {
                (program_data->solution_info)->optmoves = thread_data[t].optmoves;
                (program_data->solution_info)->move_pinset = thread_data[t].move_pinset;
                (program_data->solution_info)->optmove_tickcount = thread_data[t].optmove_tickcount;
            }
            if (thread_data[t].optticks < (program_data->solution_info)->optticks ||
                ((thread_data[t].optticks == (program_data->solution_info)->optticks) && (thread_data[t].opttick_movecount < (program_data->solution_info)->opttick_movecount))) {
                (program_data->solution_info)->optticks = thread_data[t].optticks;
                (program_data->solution_info)->tick_pinset = thread_data[t].tick_pinset;
                (program_data->solution_info)->opttick_movecount = thread_data[t].opttick_movecount;
            }
            if (thread_data[t].optsimul < (program_data->solution_info)->optsimul ||
                ((thread_data[t].optsimul == (program_data->solution_info)->optsimul) && (thread_data[t].optsimul_movecount < (program_data->solution_info)->optsimul_movecount)) || 
                ((thread_data[t].optsimul == (program_data->solution_info)->optsimul && thread_data[t].optsimul_movecount == (program_data->solution_info)->optsimul_movecount) && (thread_data[t].optsimul_tickcount < (program_data->solution_info)->optsimul_tickcount))) {
                (program_data->solution_info)->optsimul = thread_data[t].optsimul;
                (program_data->solution_info)->simul_pinset = thread_data[t].simul_pinset;
            }
            if (thread_data[t].optsimticks < (program_data->solution_info)->optsimticks ||
                ((thread_data[t].optsimticks == (program_data->solution_info)->optsimticks) && (thread_data[t].optsimtick_simulcount < (program_data->solution_info)->optsimtick_simulcount)) || 
                ((thread_data[t].optsimticks == (program_data->solution_info)->optsimticks && thread_data[t].optsimtick_simulcount == (program_data->solution_info)->optsimtick_simulcount) && (thread_data[t].optsimtick_movecount < (program_data->solution_info)->optsimtick_movecount))) {
                (program_data->solution_info)->optsimticks = thread_data[t].optsimticks;
                (program_data->solution_info)->simtick_pinset = thread_data[t].simtick_pinset;
            }

        }

        print_solutions(program_data);


    }
    // free memory
    cleanup(program_data);
}



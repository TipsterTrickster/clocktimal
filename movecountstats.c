#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>


#include "utils.h"
#include "solvers.h"


int main(int argc, char *argv[]) {

    // Multithreading
    int num_threads = 1;
	pthread_t *threads;
	struct thread_args *thread_data;
    int t, result;
    int scramble[PINSET_LENGTH];
    int i, j;
    int scrambles = 100;

    DATA_T *program_data;

    if (argc == 2) {
        scrambles = atoi(argv[1]);
    } else if (argc == 3) {
        scrambles = atoi(argv[1]);
        num_threads = atoi(argv[2]);
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

    // Chunk size for move and pinset arrays
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


    int movecount_stats[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    srand(time(NULL));



    for (i = 0; i < scrambles; i++) {

        // int scramble[PINSET_LENGTH] = {4, 7, 6, 6, 5, 4, 9, 9, 4, 10, 10, 7, 11, 4};
        for (j = 0; j < PINSET_LENGTH; j++) {
            scramble[j] = rand() % 12;  // Generates a number from 0 to 11
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

            result = pthread_create(&threads[t], NULL, find_move_optimal_p, (void *)&thread_data[t]);

            if (result) {
                perror("error creating thread\n");
                exit(1);
            }
        }

        (program_data->solution_info)->optmoves = __INT_MAX__;

        for(t=0;t<num_threads;t++) {
            pthread_join(threads[t],NULL);

            if (thread_data[t].optmoves < (program_data->solution_info)->optmoves) {
                (program_data->solution_info)->optmoves = thread_data[t].optmoves;
                (program_data->solution_info)->move_pinset = thread_data[t].move_pinset;
            }
        }



        printf("\rCount: %d", i + 1);
        fflush(stdout);
        movecount_stats[(program_data->solution_info)->optmoves]++;



        // print_solutions(program_data);

    }
    printf("\n");

    int sum = 0;
    for (i = 0; i < 14; i++) {
        printf("%d Movers: %d\n", i, movecount_stats[i]);
        sum += i * movecount_stats[i];
    }

    printf("Mean = %f\n", (float)sum / scrambles);



    // free memory
    cleanup(program_data);
}



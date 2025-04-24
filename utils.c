#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"
#include "solvers.h"

// Checks if string is a valid number
int is_valid_number(const char *str) {
    if (*str == '-' || *str == '+') str++;  // Allow signed numbers
    if (!*str) return 0;  // Empty string is invalid
    while (*str) {
        if (!isdigit(*str)) return 0;  // Reject non-digit characters
        str++;
    }
    return 1;
}


// Get state from command line
int get_scramble(int *scramble, int size) {
    char buffer[BUFFER_SIZE];
    int count = 0;

    printf("Enter scramble (or 'q' to quit): ");
    
    if (!fgets(buffer, BUFFER_SIZE, stdin)) {
        printf("Error reading input, try again\n");
        return 1;
    }

    char *token = strtok(buffer, " \t\n");

    // Check if the first token is "q" and exit
    if (token && strcmp(token, "q") == 0) {
        printf("Exiting program...\n");
        exit(0);
    }

    while (token && count < size) {
        if (is_valid_number(token)) {
            scramble[count] = ((strtol(token, NULL, 10) % 12) + 12) % 12;
        } else {
            printf("Invalid input\n");
            return 1;
        }
        count++;
        token = strtok(NULL, " \t\n");
    }

    if (count < size) {
        printf("Please input %d numbers\n", size);
        return 1;
    }

    return 0;
}


// Free all memory
void cleanup(DATA_T *program_data) {
    free(program_data->solution_info);
    free(program_data->moves);
    free(program_data->unique_rows);
    free(program_data->pinsets);
    free(program_data->pinset_mappings);
    free(program_data);
}



void print_noflip_solution(DATA_T *program_data, int movecount, int pinset, char *name) {
    int i, move, movetype, next_movetype, next_move;
    int simul_state = NOT_SIMUL;

    const char *move_names[30] = {"UR","UR","DR","DR","DL","DL","UL","UL","U","U","R","R","D","D","L","L",
    "ur","ur","dr","dr","dl","dl","ul","ul","/","/","\\","\\","ALL","ALL"};

    printf("Optimal %s: %d\n", name, movecount);

    printf("Optimal Solution: ");


    for (i = 0; i < PINSET_LENGTH; i++) {
        move = (program_data->moves)[ (program_data->pinset_mappings)[ (pinset * PINSET_LENGTH) + i ] ];
        movetype = (program_data->pinsets)[ (pinset * PINSET_LENGTH) + i ];

        if (i < PINSET_LENGTH - 1) {
            next_movetype = (program_data->pinsets)[ (pinset * PINSET_LENGTH) + i + 1];
            next_move = (program_data->moves)[ (program_data->pinset_mappings)[ (pinset * PINSET_LENGTH) + i + 1]];
        }

        // format moves between -5 and 6
        move *= -1;
        if (move <= -6) move += 12;
        next_move *= -1;
        if (next_move <= -6) next_move += 12;

        // front move
        if (movetype % 2 == 0) {
            if(i < PINSET_LENGTH - 1 && movetype == next_movetype - 1) { // detect simul move

                // print simul move if not skipped
                if (move != 0 || next_move != 0) {
                    printf("%s(%d,%d) ", move_names[ (program_data->pinsets)[ (pinset * PINSET_LENGTH) + i ] ], move, next_move);
                }

                simul_state = NEXT_SIMUL;
            } else {
                if (move != 0) {
                    printf("%s(%d,0) ", move_names[ (program_data->pinsets)[ (pinset * PINSET_LENGTH) + i ] ], move);
                }
            }
        } else if (simul_state == NOT_SIMUL) {
            if (move != 0) {
                printf("%s(0,%d) ", move_names[ (program_data->pinsets)[ (pinset * PINSET_LENGTH) + i ] ], move);
            }
        } else if (simul_state == NEXT_SIMUL) {
            simul_state = NOT_SIMUL;
        } 


    }
    printf("\n");

    
}

void print_flip_solution(DATA_T *program_data, int movecount, int pinset, char *name) {
    int i, move;

    const char *move_names[30] = {"UR","ul","DR","dl","DL","dr","UL","ur","U","D","R","R","D","U","L","L",
    "ur","UL","dr","DL","dl","DR","ul","UR","/","/","\\","\\","ALL","ALL"};

    printf("Optimal %s: %d\n", name, movecount);

    printf("Optimal Solution: ");

    // loop through front moves
    for (i = 0; i < PINSET_LENGTH; i++) {
        // even pins are front, so only do those
        if ((program_data->pinsets)[ (pinset * PINSET_LENGTH) + i ] % 2 == 0) {
            move = (program_data->moves)[ (program_data->pinset_mappings)[ (pinset * PINSET_LENGTH) + i ] ]; // get move
            
            if (move == 0) continue; // skip moves that are 0
            
            // format moves between -5 and 6
            move *= -1;
            if (move < -6) move += 12;

            // print movename
            printf("%s", move_names[ (program_data->pinsets)[ (pinset * PINSET_LENGTH) + i ] ]);
            
            // print move number
            if (move < 0) printf("%d- ", move * -1);
            else printf("%d+ ", move);
        }
    }

    printf("y2 "); // print Y2 to flip clock

    // loop through back moves
    for (i = 0; i < PINSET_LENGTH; i++) {
        // odd pins are front, so only do those
        if ((program_data->pinsets)[ (pinset * PINSET_LENGTH) + i ] % 2 == 1) {
            move = (program_data->moves)[ (program_data->pinset_mappings)[ (pinset * PINSET_LENGTH) + i ] ]; // get move
            
            if (move == 0) continue; // skip moves that are 0
            
            // format moves between -5 and 6
            if (move > 6) move -= 12;

            // print movename
            printf("%s", move_names[ (program_data->pinsets)[ (pinset * PINSET_LENGTH) + i ] ]);
            
            // print move number
            if (move < 0) printf("%d- ", move * -1);
            else printf("%d+ ", move);
        }
    }
    printf("\n");


}

// void print_movecount_solution(DATA_T *program_data) {
//     print_flip_solution(program_data, (program_data->solution_info)->optmoves, (program_data->solution_info)->move_pinset, "movecount");
// }

void print_solutions(DATA_T *program_data) {    


    /* OPTIMAL MOVES */
    print_flip_solution(program_data, (program_data->solution_info)->optmoves, (program_data->solution_info)->move_pinset, "movecount");
    print_flip_solution(program_data, (program_data->solution_info)->optticks, (program_data->solution_info)->tick_pinset, "tickcount");

    print_noflip_solution(program_data, (program_data->solution_info)->optsimul, (program_data->solution_info)->simul_pinset, "simul count");
    print_noflip_solution(program_data, (program_data->solution_info)->optsimticks, (program_data->solution_info)->simtick_pinset, "simtick count");
   
}

// void read_data(int **unique_rows, int *n_unique_rows, int **pinsets, int *n_pinsets, int **pinset_mappings, int *n_pinset_mappings) {
void read_data(DATA_T *program_data) {
    FILE *file;
    long file_size;
    int i;

    // Allocate solution info memory
    program_data->solution_info = (SOLUTION_T *)malloc(sizeof(SOLUTION_T));
    if (program_data->solution_info == NULL) {
        perror("solution memory allocation failed");
        exit(1);
    }


    /* UNIQUE ROWS */
    // read unique rows
    file = fopen("./data/unique_rows.bin", "rb");
    if (!file) {
        perror("Error opening unique_rows.bin");
        exit(1);
    }

    // Get filesize
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);

    // Calculate Number of unique rows
    program_data->n_unique_rows = file_size / (PINSET_LENGTH * sizeof(int));
    // printf("Number of unique rows %d\n", *n_unique_rows);

    // allocate unique row memory
    program_data->unique_rows = (int *)malloc(PINSET_LENGTH * (program_data->n_unique_rows) * sizeof(int));
    if (program_data->unique_rows == NULL) {
        perror("Error allocating memory to unique rows");
        exit(1);
    }

    // Read data into the allocated memory
    for (i = 0; i < program_data->n_unique_rows; i++) {
        if (fread(program_data->unique_rows + (i * PINSET_LENGTH), sizeof(int), PINSET_LENGTH, file) != PINSET_LENGTH) {
            perror("Error reading unique row data");
            fclose(file);
            exit(1);
        }
    }
    fclose(file);


    /* PINSETS */

    file = fopen("./data/pinsets.bin", "rb");
    if (!file) {
        perror("Error opening pinsets.bin");
        exit(1);
    }

    // Get filesize
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);

    // Calculate Number of unique rows
    program_data->n_pinsets = file_size / (PINSET_LENGTH * sizeof(int));
    // printf("Number of pinsets %d\n", *n_pinsets);

    // Allocate pinset memory
    program_data->pinsets = (int *)malloc(PINSET_LENGTH * program_data->n_pinsets * sizeof(int));
    if (program_data->pinsets == NULL) {
        perror("Error allocating memory to unique rows");
        exit(1);
    }

    // Read data into the allocated memory
    for (i = 0; i < program_data->n_pinsets; i++) {
        if (fread(program_data->pinsets + (i * PINSET_LENGTH), sizeof(int), PINSET_LENGTH, file) != PINSET_LENGTH) {
            perror("Error reading pinset data");
            fclose(file);
            exit(1);
        }
    }

    fclose(file);
    

    /* PINSET MAPPINGS */

    file = fopen("./data/pinset_mappings.bin", "rb");
    if (!file) {
        perror("Error opening pinset_mappings.bin");
        exit(1);
    }

    // Get filesize
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);

    // Calculate Number of unique rows
    program_data->n_pinset_mappings = file_size / (PINSET_LENGTH * sizeof(int));
    // printf("Number of pinsets mappings %d\n", *n_pinset_mappings);

    // Allocate pinset memory
    program_data->pinset_mappings = (int *)malloc(PINSET_LENGTH * (program_data->n_pinset_mappings) * sizeof(int));
    if (program_data->pinset_mappings == NULL) {
        perror("Error allocating memory to unique rows");
        exit(1);
    }

    // Read data into the allocated memory
    for (i = 0; i < program_data->n_pinset_mappings; i++) {
        if (fread(program_data->pinset_mappings + (i * PINSET_LENGTH), sizeof(int), PINSET_LENGTH, file) != PINSET_LENGTH) {
            perror("Error reading pinset data");
            fclose(file);
            exit(1);
        }
    }
    fclose(file);
}
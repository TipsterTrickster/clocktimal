#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "clocktimal.h"


int is_valid_number(const char *str) {
    if (*str == '-' || *str == '+') str++;  // Allow signed numbers
    if (!*str) return 0;  // Empty string is invalid
    while (*str) {
        if (!isdigit(*str)) return 0;  // Reject non-digit characters
        str++;
    }
    return 1;
}


int get_scramble(int *scramble, int size) {
    char buffer[BUFFER_SIZE];
    int count = 0;

    printf("Enter scramble: ");
    
    if (!fgets(buffer, BUFFER_SIZE, stdin)) {
        printf("Error reading input try again\n");
        return 1;
    }

    char *token = strtok(buffer, " \t\n");

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
        printf("Please input 14 numbers\n");
        return 1;
    }

    return 0;

}

void cleanup(DATA_T *program_data) {
    free(program_data->solution_info);
    free(program_data->moves);
    free(program_data->unique_rows);
    free(program_data->pinsets);
    free(program_data->pinset_mappings);
    free(program_data);
}


// void print_solutions(SOLUTION_T *solution_info, int *moves, int *pinsets, int *pinset_mappings) {
void print_solutions(DATA_T *program_data) {
    int i, move;

    char *move_names[30] = {"URf","URb","DRf","DRb","DLf","DLb","ULf","ULb","Uf","Ub","Rf","Rb","Df","Db","Lf","Lb",
        "urf","urb","drf","drb","dlf","dlb","ulf","ulb","/f","/b","\\f","\\b","ALLf","ALLb"};

    printf("Optimal movecount: %d\n", (program_data->solution_info)->optmoves);

    printf("Optimal Solution: ");
    for (i = 0; i < PINSET_LENGTH; i++) {
        move = (program_data->moves)[ (program_data->pinset_mappings)[ ((program_data->solution_info)->move_pinset * PINSET_LENGTH) + i ] ];
        if (move == 0) continue;
        move *= -1;
        if (move < -6) move += 12;

        printf("%s", move_names[ (program_data->pinsets)[ ((program_data->solution_info)->move_pinset * PINSET_LENGTH) + i ] ]);
        
        if (move < 0) printf("%d- ", move * -1);
        else printf("%d ", move);
    }
    printf("\n");

    printf("Optimal tickcount: %d\n", (program_data->solution_info)->optticks);

    printf("Optimal Solution: ");
    for (i = 0; i < PINSET_LENGTH; i++) {
        move = (program_data->moves)[ (program_data->pinset_mappings)[ ((program_data->solution_info)->tick_pinset * PINSET_LENGTH) + i ] ];
        if (move == 0) continue;
        move *= -1;
        if (move < -6) move += 12;

        printf("%s", move_names[ (program_data->pinsets)[ ((program_data->solution_info)->tick_pinset * PINSET_LENGTH) + i ] ]);
        
        if (move < 0) printf("%d- ", move * -1);
        else printf("%d ", move);
    }
    printf("\n");

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
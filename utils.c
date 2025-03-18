#include <stdio.h>
#include <stdlib.h>

#include "utils.h"


void read_data(int **unique_rows, int *n_unique_rows, int **pinsets, int *n_pinsets, int **pinset_mappings, int *n_pinset_mappings) {
    FILE *file;
    long file_size;
    int i;

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
    *n_unique_rows = file_size / (PINSET_LENGTH * sizeof(int));
    // printf("Number of unique rows %d\n", *n_unique_rows);

    // allocate unique row memory
    *unique_rows = (int *)malloc(PINSET_LENGTH * (*n_unique_rows) * sizeof(int));
    if (unique_rows == NULL) {
        perror("Error allocating memory to unique rows");
        exit(1);
    }

    // Read data into the allocated memory
    for (i = 0; i < *n_unique_rows; i++) {
        if (fread(*unique_rows + (i * PINSET_LENGTH), sizeof(int), PINSET_LENGTH, file) != PINSET_LENGTH) {
            perror("Error reading unique row data");
            free(*unique_rows);
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
    *n_pinsets = file_size / (PINSET_LENGTH * sizeof(int));
    // printf("Number of pinsets %d\n", *n_pinsets);

    // Allocate pinset memory
    *pinsets = (int *)malloc(PINSET_LENGTH * (*n_pinsets) * sizeof(int));
    if (pinsets == NULL) {
        perror("Error allocating memory to unique rows");
        exit(1);
    }

    // Read data into the allocated memory
    for (i = 0; i < *n_pinsets; i++) {
        if (fread(*pinsets + (i * PINSET_LENGTH), sizeof(int), PINSET_LENGTH, file) != PINSET_LENGTH) {
            perror("Error reading pinset data");
            free(*pinsets);
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
    *n_pinset_mappings = file_size / (PINSET_LENGTH * sizeof(int));
    // printf("Number of pinsets mappings %d\n", *n_pinset_mappings);

    // Allocate pinset memory
    *pinset_mappings = (int *)malloc(PINSET_LENGTH * (*n_pinset_mappings) * sizeof(int));
    if (pinset_mappings == NULL) {
        perror("Error allocating memory to unique rows");
        exit(1);
    }

    // Read data into the allocated memory
    for (i = 0; i < *n_pinset_mappings; i++) {
        if (fread(*pinset_mappings + (i * PINSET_LENGTH), sizeof(int), PINSET_LENGTH, file) != PINSET_LENGTH) {
            perror("Error reading pinset data");
            free(*pinset_mappings);
            fclose(file);
            exit(1);
        }
    }
    fclose(file);

    // if (*n_pinset_mappings != *n_pinsets) {
    //     perror
    // }
}
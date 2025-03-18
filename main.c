#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "uthash.h"

#define MEMO_SIZE 58829
#define SCRAMBLE_SIZE 14

typedef struct {
    char key[128];  // Adjust size as needed
    int64_t *values;
    size_t size;
    UT_hash_handle hh;
} HashEntry;

void find_optimal(const char *scramble, int32_t **memos, size_t memos_count, HashEntry *sets) {
    int s[SCRAMBLE_SIZE];
    int moves[MEMO_SIZE] = {0};
    
    // Parse scramble input
    char *token = strtok(strdup(scramble), " ");
    for (int i = 0; i < SCRAMBLE_SIZE && token; i++) {
        s[i] = atoi(token);
        token = strtok(NULL, " ");
    }
    
    // Compute moves array
    for (size_t i = 0; i < memos_count; i++) {
        int c = 0;
        for (int j = 0; j < SCRAMBLE_SIZE; j++) {
            c += memos[i][j] * s[j];
        }
        moves[i] = ((c % 12) + 12) % 12;
    }

    // Find the best set
    int best_moves = 100, best_ticks = 1000;
    int best_mset_ticks = 1000, best_tset_moves = 100;
    char best_mset[128] = "idk", best_tset[128] = "idk";
    int tick_counts[12] = {0,1,2,3,4,5,6,5,4,3,2,1};

    for (HashEntry *entry = sets; entry != NULL; entry = entry->hh.next) {
        int move_count = 0, ticks = 0;
        for (size_t i = 0; i < entry->size; i++) {
            if (moves[entry->values[i]] != 0) {
                move_count++;
                ticks += tick_counts[moves[entry->values[i]]];
            }
            if (move_count >= best_tset_moves && ticks >= best_mset_ticks) break;
        }
        if (ticks < best_ticks || (ticks == best_ticks && move_count < best_tset_moves)) {
            strcpy(best_tset, entry->key);
            best_ticks = ticks;
            best_tset_moves = move_count;
        }
        if (move_count < best_moves || (move_count == best_moves && ticks < best_mset_ticks)) {
            strcpy(best_mset, entry->key);
            best_moves = move_count;
            best_mset_ticks = ticks;
        }
    }

    // Print results
    if (best_ticks == best_mset_ticks) {
        printf("%d moves (%d ticks) is optimal:\n", best_moves, best_mset_ticks);
    } else {
        printf("%d moves is move optimal:\n", best_moves);
    }

    // Solution printing
    const char *move_names[12] = {"0+","1-","2-","3-","4-","5-","6+","5+","4+","3+","2+","1+"};
    HashEntry *best_entry = NULL;
    HASH_FIND_STR(sets, best_mset, best_entry);
    if (best_entry) {
        for (size_t i = 0; i < best_entry->size; i++) {
            if (moves[best_entry->values[i]] != 0) {
                printf("%s%s ", best_mset, move_names[moves[best_entry->values[i]]]);
            }
        }
        printf("\n");
    }

    if (best_ticks != best_mset_ticks) {
        printf("%d ticks is tick optimal:\n", best_ticks);
        HASH_FIND_STR(sets, best_tset, best_entry);
        if (best_entry) {
            for (size_t i = 0; i < best_entry->size; i++) {
                if (moves[best_entry->values[i]] != 0) {
                    printf("%s%s ", best_tset, move_names[moves[best_entry->values[i]]]);
                }
            }
            printf("\n");
        }
    }
}



int main() {
    printf("Data loading...\n");

    int32_t **memos;
    size_t memos_count;
    HashEntry *sets = NULL;

    load_data("./data/matrixRows.bin", &memos, &memos_count, "./data/pinSets.bin", &sets);
    printf("Data loaded\n");

    // char scramble[256];
    // while (1) {
    //     printf("Enter a scramble state:\n");
    //     if (!fgets(scramble, sizeof(scramble), stdin)) break;

    //     find_optimal(scramble, memos, memos_count, sets);
    // }

    return 0;
}

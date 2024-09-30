// utils.c
#include <stdlib.h>

// Durstenfeld shuffle algorithm
void shuffle_operations(char* operations, int* values, int m) {
    for (int i = m - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        // Swap operations
        char temp_op = operations[i];
        operations[i] = operations[j];
        operations[j] = temp_op;
        int temp_val = values[i];
        values[i] = values[j];
        values[j] = temp_val;
    }
}

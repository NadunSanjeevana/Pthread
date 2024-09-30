// serial_optimized.c
#include "linked_list.h"
#include <stdlib.h>
#include <time.h>
#include "utils.h"



unsigned long test_serial_run(int case_num) {
    int m = 10000; // Number of operations
    float mMember, mInsert, mDelete;

    // Determine operation proportions based on case number
    switch(case_num) {
        case 1:
            mMember = 0.99;
            mInsert = 0.005;
            mDelete = 0.005;
            break;
        case 2:
            mMember = 0.90;
            mInsert = 0.05;
            mDelete = 0.05;
            break;
        case 3:
            mMember = 0.50;
            mInsert = 0.25;
            mDelete = 0.25;
            break;
        default:
            mMember = 0.80;
            mInsert = 0.10;
            mDelete = 0.10;
    }

    int memberOps = m * mMember;
    int insertOps = m * mInsert;
    int deleteOps = m * mDelete;

    // Allocate memory for operations and corresponding values
    char* operations = malloc(m * sizeof(char));  // Operations array
    int* values = malloc(m * sizeof(int));        // Corresponding values array

    // Fill operations array
    for (int i = 0; i < memberOps; i++) operations[i] = 'M';
    for (int i = memberOps; i < memberOps + insertOps; i++) operations[i] = 'I';
    for (int i = memberOps + insertOps; i < m; i++) operations[i] = 'D';

    // Fill values array with random values
    for (int i = 0; i < m; i++) values[i] = rand() % 65536;

    // Shuffle the operations and values using Durstenfeld shuffle
    shuffle_operations(operations, values, m);

    InitializeList(1000); // Initial population with 1000 unique random values

    clock_t start = clock();  // Start timing the operations

    // Sequentially perform each operation based on the shuffled order
    for (int i = 0; i < m; i++) {
        int value = values[i];
        switch (operations[i]) {
            case 'M':
                Member(value);  // Member operation (no lock needed in serial)
                break;
            case 'I':
                while (!Insert(value)) {  // Insert operation (no lock needed in serial)
                    value = rand() % 65536;
                }
                break;
            case 'D':
                Delete(value);  // Delete operation (no lock needed in serial)
                break;
        }
    }

    clock_t end = clock();  // End timing the operations
    FreeList();  // Free the linked list memory

    // Free dynamically allocated arrays
    free(operations);
    free(values);

    // Return the time in microseconds
    return (unsigned long)((end - start) * 1000000 / CLOCKS_PER_SEC);
}

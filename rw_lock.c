#include "linked_list.h"
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

pthread_rwlock_t rwlock;

typedef struct {
    long rank;
    int case_num;
    char* operations;  // Operation list for this thread
    int* values;       // Corresponding values for the operations
    int num_operations; // Number of operations to perform
} thread_args;

void* rwlock_operation(void* args) {
    thread_args* my_args = (thread_args*) args;
    int num_operations = my_args->num_operations;

    // Perform each operation based on the shuffled order
    for (int i = 0; i < num_operations; i++) {
        int value = my_args->values[i];
        switch (my_args->operations[i]) {
            case 'M':
                pthread_rwlock_rdlock(&rwlock);
                Member(value);  // Member operation (read lock)
                pthread_rwlock_unlock(&rwlock);
                break;
            case 'I':
                pthread_rwlock_wrlock(&rwlock);
                while (!Insert(value)) {  // Insert operation (write lock)
                    value = rand() % 65536;
                }
                pthread_rwlock_unlock(&rwlock);
                break;
            case 'D':
                pthread_rwlock_wrlock(&rwlock);
                Delete(value);  // Delete operation (write lock)
                pthread_rwlock_unlock(&rwlock);
                break;
        }
    }
    return NULL;
}

unsigned long test_rw_lock_run(int case_num, int num_threads) {
    pthread_t* thread_handles;
    thread_handles = malloc(num_threads * sizeof(pthread_t));
    pthread_rwlock_init(&rwlock, NULL);  // Initialize the read-write lock

    int m = 10000;  // Total number of operations
    float mMember, mInsert, mDelete;

    // Set operation ratios based on the case number
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

    // Allocate memory for operations and values
    char* operations = malloc(m * sizeof(char));  // Operations array
    int* values = malloc(m * sizeof(int));        // Corresponding values array

    // Fill operations array
    for (int i = 0; i < memberOps; i++) operations[i] = 'M';
    for (int i = memberOps; i < memberOps + insertOps; i++) operations[i] = 'I';
    for (int i = memberOps + insertOps; i < m; i++) operations[i] = 'D';

    // Fill values array with random values
    for (int i = 0; i < m; i++) values[i] = rand() % 65536;

    // Shuffle the operations and corresponding values using Durstenfeld shuffle
    shuffle_operations(operations, values, m);

    InitializeList(1000);  // Initialize the list with 1000 random values

    clock_t start = clock();  // Start timing the operations

    // Allocate thread arguments and assign operations to threads
    thread_args* args = malloc(num_threads * sizeof(thread_args));
    int operations_per_thread = m / num_threads;
    int remaining_operations = m % num_threads;  // Handle remainder for uneven division

    // Assign operations to each thread
    for (long thread = 0; thread < num_threads; thread++) {
        args[thread].rank = thread;
        args[thread].case_num = case_num;

        // Each thread gets equal operations, with the remainder distributed
        args[thread].num_operations = operations_per_thread + (thread < remaining_operations ? 1 : 0);
        args[thread].operations = &operations[thread * operations_per_thread];
        args[thread].values = &values[thread * operations_per_thread];
        
        pthread_create(&thread_handles[thread], NULL, rwlock_operation, (void*)&args[thread]);
    }

    // Wait for all threads to complete
    for (long thread = 0; thread < num_threads; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }

    clock_t end = clock();  // End timing the operations

    FreeList();  // Free the linked list memory

    pthread_rwlock_destroy(&rwlock);  // Destroy the read-write lock
    free(thread_handles);  // Free thread handles memory
    free(args);  // Free thread arguments memory
    free(operations);  // Free operations array
    free(values);  // Free values array

    // Return the time in microseconds
    return (unsigned long)((end - start) * 1000000 / CLOCKS_PER_SEC);
}

#include "linked_list.h"
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

pthread_rwlock_t rwlock;

typedef struct {
    long rank;
    int case_num;
    char* operations;  
    int* values;       
    int num_operations; 
} thread_args;

// Thread function for handling operations using read-write locks
void* rwlock_operation(void* args) {
    thread_args* my_args = (thread_args*) args;
    int num_operations = my_args->num_operations;

    // Perform each operation in the shuffled order
    for (int i = 0; i < num_operations; i++) {
        int value = my_args->values[i];
        switch (my_args->operations[i]) {
            case 'M':
                pthread_rwlock_rdlock(&rwlock);  // Read lock for Member operation
                Member(value);
                pthread_rwlock_unlock(&rwlock);
                break;
            case 'I':
                pthread_rwlock_wrlock(&rwlock);  // Write lock for Insert operation
                while (!Insert(value)) {
                    value = rand() % 65536;  
                }
                pthread_rwlock_unlock(&rwlock);
                break;
            case 'D':
                pthread_rwlock_wrlock(&rwlock);  // Write lock for Delete operation
                Delete(value);
                pthread_rwlock_unlock(&rwlock);
                break;
        }
    }
    return NULL;
}

// Test function for performing operations using read-write lock
unsigned long test_rw_lock_run(int case_num, int num_threads) {
    pthread_t* thread_handles = malloc(num_threads * sizeof(pthread_t));
    pthread_rwlock_init(&rwlock, NULL);  

    int m = 10000;  
    float mMember, mInsert, mDelete;

    
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
    char* operations = malloc(m * sizeof(char));
    int* values = malloc(m * sizeof(int));
    if (!operations || !values) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    
    for (int i = 0; i < memberOps; i++) operations[i] = 'M';
    for (int i = memberOps; i < memberOps + insertOps; i++) operations[i] = 'I';
    for (int i = memberOps + insertOps; i < m; i++) operations[i] = 'D';

    
    for (int i = 0; i < m; i++) values[i] = rand() % 65536;

    // Shuffle operations and values
    shuffle_operations(operations, values, m);

    
    InitializeList(1000);

    clock_t start = clock();  

    // Allocate thread arguments and assign operations
    thread_args* args = malloc(num_threads * sizeof(thread_args));
    int operations_per_thread = m / num_threads;
    int remaining_operations = m % num_threads;
    int start_index = 0;  

    // Assign work to threads
    for (long thread = 0; thread < num_threads; thread++) {
        args[thread].rank = thread;
        args[thread].case_num = case_num;

        // Handle uneven distribution of operations
        args[thread].num_operations = operations_per_thread + (thread < remaining_operations ? 1 : 0);
        args[thread].operations = &operations[start_index];
        args[thread].values = &values[start_index];
        
        start_index += args[thread].num_operations;  

        // Create the thread
        pthread_create(&thread_handles[thread], NULL, rwlock_operation, (void*)&args[thread]);
    }

    // Wait for all threads to complete
    for (long thread = 0; thread < num_threads; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }

    clock_t end = clock();  

    FreeList();  

    // Clean up
    pthread_rwlock_destroy(&rwlock);  // Destroy the read-write lock
    free(thread_handles);
    free(args);
    free(operations);
    free(values);

    // Return the time taken in microseconds
    return (unsigned long)((end - start) * 1000000 / CLOCKS_PER_SEC);
}

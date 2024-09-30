// mutex_optimized.c
#include "linked_list.h"
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "utils.h"

pthread_mutex_t mutex;

typedef struct {
    long rank;
    int case_num;
    char* operations;  
    int* values;       
    int num_operations; 
} thread_args;

// Function to perform operations using a mutex lock
void* mutex_operation(void* args) {
    thread_args* my_args = (thread_args*) args;
    int num_operations = my_args->num_operations;

    // Perform each operation in the shuffled order
    for (int i = 0; i < num_operations; i++) {
        int value = my_args->values[i];
        pthread_mutex_lock(&mutex);  // Lock the mutex before accessing the list
        switch (my_args->operations[i]) {
            case 'M':
                Member(value);  
                break;
            case 'I':
                while (!Insert(value)) {  
                    value = rand() % 65536;  
                }
                break;
            case 'D':
                Delete(value);  
                break;
        }
        pthread_mutex_unlock(&mutex);  // Unlock the mutex after the operation
    }
    return NULL;
}

// Function to test mutex performance with shuffled operations
unsigned long test_mutex_run(int case_num, int num_threads) {
    pthread_t* thread_handles = malloc(num_threads * sizeof(pthread_t));
    pthread_mutex_init(&mutex, NULL);  

    int m = 10000;  
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

    // Allocate operation arrays
    char* operations = malloc(m * sizeof(char));  
    int* values = malloc(m * sizeof(int));  

    
    for (int i = 0; i < memberOps; i++) operations[i] = 'M';
    for (int i = memberOps; i < memberOps + insertOps; i++) operations[i] = 'I';
    for (int i = memberOps + insertOps; i < m; i++) operations[i] = 'D';

    
    for (int i = 0; i < m; i++) values[i] = rand() % 65536;

    // Shuffle the operations and corresponding values using Durstenfeld shuffle
    shuffle_operations(operations, values, m);

    InitializeList(1000);  

    clock_t start = clock();  

    // Allocate thread arguments and assign operations to threads
    thread_args* args = malloc(num_threads * sizeof(thread_args));
    int operations_per_thread = m / num_threads;
    int remaining_operations = m % num_threads;  

    
    int start_index = 0;
    for (long thread = 0; thread < num_threads; thread++) {
        args[thread].rank = thread;
        args[thread].case_num = case_num;

        // Each thread gets equal operations, with the remainder distributed
        args[thread].num_operations = operations_per_thread + (thread < remaining_operations ? 1 : 0);

        // Set the correct start index for the operations and values
        args[thread].operations = &operations[start_index];
        args[thread].values = &values[start_index];

        // Move the start_index forward by the number of operations assigned to this thread
        start_index += args[thread].num_operations;

        pthread_create(&thread_handles[thread], NULL, mutex_operation, (void*)&args[thread]);
    }

    // Wait for all threads to complete
    for (long thread = 0; thread < num_threads; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }

    clock_t end = clock();  

    FreeList();  

    pthread_mutex_destroy(&mutex);  
    free(thread_handles);  
    free(args);  
    free(operations);  
    free(values);  

    // Return the time in microseconds
    return (unsigned long)((end - start) * 1000000 / CLOCKS_PER_SEC);
}

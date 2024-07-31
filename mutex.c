// mutex.c
#include "linked_list.h"
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

pthread_mutex_t mutex;

typedef struct {
    long rank;
    int case_num;
} thread_args;

void* mutex_operation(void* args) {
    thread_args* my_args = (thread_args*) args;
    int case_num = my_args->case_num;
    int m = 10000; // Number of operations
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

    
    for (int i = 0; i < memberOps; i++) {
        int value = rand() % 65536;
        pthread_mutex_lock(&mutex);
        Member(value);
        pthread_mutex_unlock(&mutex);
    }
    for (int i = 0; i < insertOps; i++) {
        int value = rand() % 65536;
        pthread_mutex_lock(&mutex);
        while (!Insert(value)) {
            value = rand() % 65536;
        }
        pthread_mutex_unlock(&mutex);
    }
    for (int i = 0; i < deleteOps; i++) {
        int value = rand() % 65536;
        pthread_mutex_lock(&mutex);
        Delete(value);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

unsigned long test_mutex_run(int case_num, int num_threads) {
    pthread_t* thread_handles;
    thread_handles = malloc(num_threads * sizeof(pthread_t));
    pthread_mutex_init(&mutex, NULL);

    InitializeList(1000); // Initial population with 1000 unique random values

    clock_t start = clock();
    thread_args* args = malloc(num_threads * sizeof(thread_args));
    for (long thread = 0; thread < num_threads; thread++) {
        args[thread].rank = thread;
        args[thread].case_num = case_num;
        pthread_create(&thread_handles[thread], NULL, mutex_operation, (void*)&args[thread]);
    }
    for (long thread = 0; thread < num_threads; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }
    clock_t end = clock();

    FreeList();
    pthread_mutex_destroy(&mutex);
    free(thread_handles);
    free(args);

    return (unsigned long)(end - start);
}

// mutex.c
#include "linked_list.h"
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

pthread_mutex_t mutex;

void* mutex_operation(void* rank) {
    int my_rank = (long)rank;
    int m = 10000; // Number of operations
    float mMember = 0.8, mInsert = 0.1, mDelete = 0.1;
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
    for (long thread = 0; thread < num_threads; thread++) {
        pthread_create(&thread_handles[thread], NULL, mutex_operation, (void*)thread);
    }
    for (long thread = 0; thread < num_threads; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }
    clock_t end = clock();

    FreeList();
    pthread_mutex_destroy(&mutex);
    free(thread_handles);

    return (unsigned long)(end - start);
}

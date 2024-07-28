// rw_lock.c
#include "linked_list.h"
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

pthread_rwlock_t rwlock;

void* rwlock_operation(void* rank) {
    int my_rank = (long)rank;
    int m = 10000; // Number of operations
    float mMember = 0.8, mInsert = 0.1, mDelete = 0.1;
    int memberOps = m * mMember;
    int insertOps = m * mInsert;
    int deleteOps = m * mDelete;

    for (int i = 0; i < memberOps; i++) {
        int value = rand() % 65536;
        pthread_rwlock_rdlock(&rwlock);
        Member(value);
        pthread_rwlock_unlock(&rwlock);
    }
    for (int i = 0; i < insertOps; i++) {
        int value = rand() % 65536;
        pthread_rwlock_wrlock(&rwlock);
        while (!Insert(value)) {
            value = rand() % 65536;
        }
        pthread_rwlock_unlock(&rwlock);
    }
    for (int i = 0; i < deleteOps; i++) {
        int value = rand() % 65536;
        pthread_rwlock_wrlock(&rwlock);
        Delete(value);
        pthread_rwlock_unlock(&rwlock);
    }
    return NULL;
}

unsigned long test_rw_lock_run(int case_num, int num_threads) {
    pthread_t* thread_handles;
    thread_handles = malloc(num_threads * sizeof(pthread_t));
    pthread_rwlock_init(&rwlock, NULL);

    InitializeList(1000); // Initial population with 1000 unique random values

    clock_t start = clock();
    for (long thread = 0; thread < num_threads; thread++) {
        pthread_create(&thread_handles[thread], NULL, rwlock_operation, (void*)thread);
    }
    for (long thread = 0; thread < num_threads; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }
    clock_t end = clock();

    FreeList();
    pthread_rwlock_destroy(&rwlock);
    free(thread_handles);

    return (unsigned long)(end - start);
}

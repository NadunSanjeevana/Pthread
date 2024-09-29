// serial.c
#include "linked_list.h"
#include <stdlib.h>
#include <time.h>

unsigned long test_serial_run(int case_num) {
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

    InitializeList(1000); // Initial population with 1000 unique random values

    clock_t start = clock();
    for (int i = 0; i < memberOps; i++) {
        int value = rand() % 65536;
        Member(value);
    }
    for (int i = 0; i < insertOps; i++) {
        int value = rand() % 65536;
        while (!Insert(value)) {
            value = rand() % 65536;
        }
    }
    for (int i = 0; i < deleteOps; i++) {
        int value = rand() % 65536;
        Delete(value);
    }
    clock_t end = clock();
    FreeList();

    return (unsigned long)((end - start) * 1000000 / CLOCKS_PER_SEC);
}

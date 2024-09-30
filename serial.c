#include "linked_list.h"   
#include "utils.h"         
#include <stdlib.h>        
#include <time.h>          
#include <stdio.h>         


unsigned long test_serial_run(int case_num) {
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

    // Allocate memory for the operations array and corresponding values array
    char* operations = malloc(m * sizeof(char));  
    int* values = malloc(m * sizeof(int));        

    
    for (int i = 0; i < memberOps; i++) operations[i] = 'M';
    for (int i = memberOps; i < memberOps + insertOps; i++) operations[i] = 'I';
    for (int i = memberOps + insertOps; i < m; i++) operations[i] = 'D';

    
    for (int i = 0; i < m; i++) values[i] = rand() % 65536;

    // Shuffle the operations and corresponding values
    shuffle_operations(operations, values, m);

    
    InitializeList(1000); 

    
    clock_t start = clock();  

    // Sequentially perform each operation
    for (int i = 0; i < m; i++) {
        int value = values[i];
        switch (operations[i]) {
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
    }

    
    clock_t end = clock();  

    
    FreeList();
    free(operations);
    free(values);

    // Return the time taken for the operations in microseconds
    return (unsigned long)((end - start) * 1000000 / CLOCKS_PER_SEC);
}

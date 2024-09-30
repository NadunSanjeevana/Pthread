#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct node {
    int data;
    struct node* next;
} node;

int Member(int value);
int Insert(int value);
int Delete(int value);
void InitializeList(int n);
void FreeList();

#endif 
